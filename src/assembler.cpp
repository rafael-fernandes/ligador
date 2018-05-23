#include "../include/assembler.hpp"

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

Assembler::Assembler(string source, string target) {
  this->sourceFileName = source;
  this->targetFileName = target;

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
            cout << "Error: symbol " << command->label << "redefined" << endl;
            symbolDefined = true;
          }
        }
        
        // if symbol is not defined
        if (!symbolDefined) {
          // create symbol and insert into TS
          Symbol * symbol = new Symbol();

          symbol->setSymbol(command->label);
          symbol->setValue(to_string(positionCounter));

          TS.push_back(symbol);

          // get next token
          ss >> token;

          command->operation = token;

          // if operation is a instruction
          for (int j = 1; j <= 14; j++) {
            if (IT[j]->mnemonic == command->operation) {
              positionCounter += IT[j]->length;
              instructionFound = true;

              // get op1 and/or op2
              for (int k = 0; k < IT[j]->operands; k++) {
                // get op
                ss >> token;

                // insert op into command operands
                command->operands.push_back(token);
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

            // get op1 and/or op2
            for (int k = 0; k < IT[j]->operands; k++) {
              // get op
              ss >> token;

              // insert op into command operands
              command->operands.push_back(token);
            }
          }
        }
      }
    }

    if (textStarted)
      textSection.push_back(command);
    
    else if (dataStarted)
      dataSection.push_back(command);
  }

  // close file
  sourceFile.close();
}

void Assembler::printTS() {
  for (auto symbol:TS)
    cout << "Symbol: " << symbol->getSymbol() << "; Value: " << symbol->getValue() << endl;
}

void Assembler::printTextSection() {
  for (auto command:textSection) {
    cout << "Label: " << command->label << ", operation: " << command->operation << ", operands(" << command->operands.size() << "): ";
    
    for (auto operand:command->operands) {
      cout << operand << " ";
    }

    cout << endl;
  }
}

void Assembler::printDataSection() {
  for (auto command:dataSection) {
    cout << "Label: " << command->label << ", operation: " << command->operation << ", operands(" << command->operands.size() << "): ";
    
    for (auto operand:command->operands) {
      cout << operand << " ";
    }

    cout << endl;
  }
}

void Assembler::secondPassage() {
  ofstream outputFile("processed/" + targetFileName + ".o");

  for (auto command:textSection) {
    // output opcode
    for (int i = 1; i <= 14; i++) {
      if (IT[i]->mnemonic == command->operation) {
        outputFile << IT[i]->opcode;
        // outputFile << IT[i]->mnemonic;
        outputFile << " ";
      }
    }

    // for each operand
    for (auto operand:command->operands) {
    
      // if operand is a symbol
      for (auto symbol:TS) {
        if (symbol->getSymbol() == operand) {
          // outputFile << symbol->getSymbol();
          outputFile << symbol->getValue();
          outputFile << " ";
        }
      }
    }

    // outputFile << endl;
  }

  for (auto command:dataSection) {
    if (command->operation == "SPACE")
      for (int i = 0; i <= command->operands.size(); i++)
        outputFile << "00 ";
    
    if (command->operation == "CONST")
      outputFile << command->operands[0] << " ";
    
    // outputFile << endl;
  }

  outputFile.close();
}

void Assembler::processFile() {
  firstPassage();
  secondPassage();
}