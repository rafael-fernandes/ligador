#include "../include/assembler.hpp"

std::string replaceAll(std::string str, const std::string& from, const std::string& to) {
  size_t start_pos = 0;
 
  while((start_pos = str.find(from, start_pos)) != std::string::npos) {
      str.replace(start_pos, from.length(), to);
      start_pos += to.length(); // Handles case where 'to' is a substring of 'from'
  }

  return str;
}

void Assembler::buildIT() {
  IT[1] = new Instruction("ADD", "01", 1, 2);
  IT[2] = new Instruction("SUB", "02", 1, 2);
  IT[3] = new Instruction("MULT", "03", 1, 2);
  IT[4] = new Instruction("DIV", "04", 1, 2);
  IT[5] = new Instruction("JMP", "05", 1, 2);
  IT[6] = new Instruction("JMPN", "06", 1, 2);
  IT[7] = new Instruction("JMPP", "07", 1, 2);
  IT[8] = new Instruction("JMPZ", "08", 1, 2);
  IT[9] = new Instruction("COPY", "09", 2, 3);
  IT[10] = new Instruction("LOAD", "10", 1, 2);
  IT[11] = new Instruction("STORE", "11", 1, 2);
  IT[12] = new Instruction("INPUT", "12", 1, 2);
  IT[13] = new Instruction("OUTPUT", "13", 1, 2);
  IT[14] = new Instruction("STOP", "14", 0, 1);
}

Assembler::Assembler(string source) {
  this->sourceFileName = source;
  this->targetFileName = source;

  buildIT();
}

void Assembler::printIT() {
  for (auto instruction:IT) {
    cout << instruction->mnemonic << endl;
  }
}

void Assembler::firstPassage() {
  ifstream sourceFile("processed/" + targetFileName + ".mcr");

  // copy file content to string
  vector<string> intermediateCode;
  string line;

  while (getline(sourceFile, line))
    intermediateCode.push_back(line);

  int positionCounter = 0;

  bool textStarted, dataStarted;

  if (this->module) {
    if (intermediateCode[0].find("BEGIN") == -1)
      cout << "\033[1;31msemantic error:\033[0m BEGIN directive missing" << endl;
    
    if (intermediateCode[intermediateCode.size() - 1].find("END") == -1)
      cout << "\033[1;31msemantic error:\033[0m END directive missing" << endl;
  }

  // while there is source code
  for (int i = 0; i < intermediateCode.size(); i++) {
    
    // get line from source code
    line = intermediateCode[i];

    if (line.find("SECTION TEXT") != -1) {
      textStarted = true;
      dataStarted = false;
      continue;
    }
    
    if (line.find("SECTION DATA") != -1) {
      if (!textStarted)
        cout << "\033[1;31msemantic error:\033[0m SECTION DATA needs to come after SECTION TEXT" << endl;

      textStarted = false;
      dataStarted = true;
      continue;
    }

    // split elements into label, operation, operands and comments
    Command * command = new Command();

    stringstream ss(line);
    string token;

    bool instructionFound = false;
    
    // get operation or label
    if (ss >> token) {

      // if label exists
      if (token.find(":") != -1) {
        // get label
        command->label = token.substr(0, token.find(":"));

        // cout << positionCounter << " " << command->label << endl;

        bool symbolDefined = false;

        // search label in TS
        for (auto symbol:TS) {
          // if symbol is already defined
          if (symbol->getSymbol() == command->label) {
            cout << "\033[1;31msemantic error:\033[0m symbol " << command->label << " redefined" << endl;
            symbolDefined = true;
          }
        }
        
        // if symbol is not defined
        if (!symbolDefined) {
          // create symbol and insert into TS
          Symbol * symbol = new Symbol();

          symbol->setSymbol(command->label);
          symbol->setValue(to_string(positionCounter));

          // get next token
          ss >> token;

          command->operation = token;

          if (command->operation == "EXTERN") {
            symbol->externSymbol = true;
          }

          else {
            symbol->externSymbol = false;
          }

          TS.push_back(symbol);

          // if operation is a instruction
          for (int j = 1; j <= 14; j++) {
            if (IT[j]->mnemonic == command->operation) {
              positionCounter += IT[j]->length;
              instructionFound = true;

              if (command->operation == "COPY") {
                string args, newToken;
                ss >> args;
                args = replaceAll(args, ",", " ");
                stringstream css(args);

                for (int k = 0; k < IT[j]->operands; k++) {
                  if (css >> newToken)
                    command->operands.push_back(newToken);
                }
              } else {
                // get op1 and/or op2
                for (int k = 0; k < IT[j]->operands; k++) {
                  // get op
                  if (ss >> token) {
                    // insert op into command operands
                    command->operands.push_back(token);
                  }
                }
              }
            }
          }

          // if operation is not a instruction
          if (!instructionFound) {
            // if operation is directive CONST
            if (command->operation == "CONST") {
              positionCounter += 1;

              // get CONST value
              ss >> token;

              if (token.size() >= 2) {
                if (token[0] == '-' || (token[0] == '0' && token[1] == 'X'))
                  token = replaceAll(token, "0X", "");
                
                stringstream stream;

                int number;
                stringstream(token) >> number;

                stream << token;
                stream >> hex >> number;
                
                token = to_string(number);
              }

              // cout << token << endl;

              command->operands.push_back(token);
            }

            // if operation is directive SPACE
            else if (command->operation == "SPACE") {
              if (ss >> token) {
                positionCounter += stoi(token);
                command->operands.push_back(token);
              }

              else {
                positionCounter += 1;
              }
            }
            
            else if (this->module) {
            }
 
            else {
              cout << "\033[1;31msintatic error:\033[0m invalid directive " << command->operation << endl;
            }
          }
        }
      }

      // if there is no label
      else {
        command->operation = token;

        // if operation is a instruction
        for (int j = 1; j <= 14; j++) {
          if (IT[j]->mnemonic == command->operation) {
            positionCounter += IT[j]->length;
            instructionFound = true;

            if (command->operation == "COPY") {
              string args, newToken;
              ss >> args;
              args = replaceAll(args, ",", " ");
              stringstream css(args);

              for (int k = 0; k < IT[j]->operands; k++) {
                if (css >> newToken)
                  command->operands.push_back(newToken);
              }
            } else {
              // get op1 and/or op2
              for (int k = 0; k < IT[j]->operands; k++) {
                // get op
                if (ss >> token) {
                  // insert op into command operands
                  command->operands.push_back(token);
                }
              }
            }
          }
        }

        bool directiveFound = false;

        if (!instructionFound) {
          if (command->operation == "SPACE" || command->operation == "CONST" || (this->module && command->operation == "BEGIN")) {
            cout << "\033[1;31msintatic error:\033[0m missing label for " << command->operation << endl;
            directiveFound = true;
          }

          if (this->module) {
            if (command->operation == "END")
              directiveFound = true;

            if (command->operation == "PUBLIC") {
              ss >> token;
              
              // create symbol
              Symbol * symbol = new Symbol();

              symbol->setSymbol(token);
              symbol->setValue("0");

              // insert symbol into definition table
              this->TD.push_back(symbol);

              directiveFound = true;
            }
          }
        }

        if (!directiveFound && !instructionFound) {
          cout << "\033[1;31msemantic error:\033[0m invalid instruction " << command->operation << endl;
        }
      }
    }

    if (textStarted) {
      if (command->operation == "CONST" || command->operation == "SPACE")
        cout << "\033[1;31msemantic error:\033[0m directive " << command->operation << " at label " << command->label << " in wrong section" << endl;
      
      textSection.push_back(command);
    }

    if (!this->module) {
      if (i == 0 && !textStarted) {
        cout << "\033[1;31msemantic error:\033[0m missing SECTION TEXT" << endl;
      }
    }
    
    else if (dataStarted && command->operation != "END") {
      for (int j = 1; j <= 14; j++)
        if (IT[j]->mnemonic == command->operation)
          cout << "\033[1;31msemantic error:\033[0m instruction " << command->operation << " at label " << command->label << " in wrong section" << endl;

      dataSection.push_back(command);
    }
  }

  // update TD
  for (auto symbol:TS) {
    for (auto definition:TD) {
      if (definition->getSymbol() == symbol->getSymbol()) {
        definition->setValue(symbol->getValue());
      }
    }
  }

  // close file
  sourceFile.close();
}

void Assembler::printTS() {
  cout << "SYMBOL TABLE" << endl;

  for (auto symbol:TS)
    cout << "Symbol: " << symbol->getSymbol() << "; Value: " << symbol->getValue() << "; Extern: " << symbol->externSymbol << endl;
}

void Assembler::printTU() {
  cout << "USE TABLE" << endl;

  for (auto symbol:TU)
    cout << "Symbol: " << symbol->getSymbol() << "; Value: " << symbol->getValue() << endl;
}

void Assembler::printTD() {
  cout << "DEFINITION TABLE" << endl;

  for (auto symbol:TD)
    cout << "Symbol: " << symbol->getSymbol() << "; Value: " << symbol->getValue() << endl;
}

void Assembler::printTextSection() {
  cout << "TEXT SECTION" << endl;
  
  for (auto command:textSection) {
    cout << "Label: " << command->label << ", operation: " << command->operation << ", operands(" << command->operands.size() << "): ";
    
    for (auto operand:command->operands) {
      cout << operand << " ";
    }

    cout << endl;
  }
}

void Assembler::printDataSection() {
  cout << "DATA SECTION" << endl;

  for (auto command:dataSection) {
    cout << "Label: " << command->label << ", operation: " << command->operation << ", operands(" << command->operands.size() << "): ";
    
    for (auto operand:command->operands) {
      cout << operand << " ";
    }

    cout << endl;
  }
}

void Assembler::secondPassage() {
  int positionCounter = 0;
  vector<string> textObject;
  ofstream outputFile("processed/" + targetFileName + ".o");

  if (this->module) {
    outputFile << "H: " << targetFileName << endl;
  }

  for (auto command:textSection) {
    if (command->operation == "STORE")
      for (auto operand:command->operands)
        for (auto data:dataSection)
          if (data->label == operand && data->operation == "CONST")
            cout << "\033[1;31msemantic error:\033[0m STORE modifies CONST value " << data->label << endl;
    
    if (command->operation == "DIV") {
      for (auto operand:command->operands) {
        for (auto data:dataSection) {
          if (data->label == operand) {
            for (auto dataOperand:data->operands) {
              if (dataOperand == "0")
                cout << "\033[1;31msemantic error:\033[0m division by 0" << endl;
            }
          }
        }
      }
    }

    bool jmpOperandFound = false;

    if (command->operation == "JMP" || command->operation == "JMPN" || command->operation == "JMPZ" || command->operation == "JMPP") {
      for (auto operand:command->operands) {
        for (auto command:textSection) {
          if (command->label == operand)
            jmpOperandFound = true;
        }

        for (auto data:dataSection) {
          if (data->label == operand) {
            cout << "\033[1;31msemantic error:\033[0m invalid jump to SECTION DATA" << endl;
          }
        }
      }

      if (!jmpOperandFound)
        cout << "\033[1;31msemantic error:\033[0m invalid jump to missing label" << endl;
    }

    // output opcode
    for (int i = 1; i <= 14; i++) {
      if (IT[i]->mnemonic == command->operation) {
        if (IT[i]->operands != command->operands.size()) {
          cout << "\033[1;31msintatic error:\033[0m wrong arguments number for " << command->operation << endl;
        }

        // outputFile << positionCounter << " ";
        // outputFile << IT[i]->opcode;
        textObject.push_back(IT[i]->opcode);
        // outputFile << IT[i]->mnemonic;
        // outputFile << " ";

        int offset = 1;

        if (command->operation == "COPY")
          offset = 0;

        // for each operand
        for (auto operand:command->operands) {
          // if operand is a symbol
          for (auto symbol:TS) {
            if (symbol->getSymbol() == operand) {
              // outputFile << symbol->getSymbol();
              // outputFile << symbol->getValue();
              textObject.push_back(symbol->getValue());
              // outputFile << " ";

              // if symbol is extern
              if (symbol->externSymbol) {
                Symbol * usedExternSymbol = new Symbol();

                usedExternSymbol->setSymbol(symbol->getSymbol());

                if (command->operation == "COPY")
                  offset++;
                  
                usedExternSymbol->setValue(to_string(positionCounter + offset));

                this->TU.push_back(usedExternSymbol);
              }
            }
          }
        }

        positionCounter += IT[i]->length;
      }
    }

    // outputFile << endl;
  }

  for (auto command:dataSection) {
    if (command->operation == "SPACE") {
      for (int i = 0; i <= command->operands.size(); i++) {
        // outputFile << positionCounter << " ";
        textObject.push_back("00");
        // outputFile << "00 ";
        positionCounter += 1;
      }
    }
    
    if (command->operation == "CONST") {
      // outputFile << positionCounter << " ";
      textObject.push_back(command->operands[0]);
      // outputFile << command->operands[0] << " " << endl;
      positionCounter += 1;
    }
    
    // outputFile << endl;
  }

  if (this->module) {
    // object size
    outputFile << "H: " << textObject.size() << endl;
    // object use table
    outputFile << "H: ";

    for (auto symbol:TU)
      outputFile << symbol->getSymbol() << " " << symbol->getValue() << " ";
    
    outputFile << endl;

    // object definition table
    outputFile << "H: ";

    for (auto symbol:TD)
      outputFile << symbol->getSymbol() << " " << symbol->getValue() << " ";

    outputFile << endl;
    
    // object text
    outputFile << "T: ";
  }

  for (auto code:textObject)
    outputFile << code << " ";

  outputFile.close();
}

void Assembler::processFile() {
  firstPassage();
  printTextSection();
  printDataSection();
  printTS();
  printTD();
  secondPassage();
  printTU();
}