#include "../include/assembler.hpp"

void Assembler::buildIT() {
  IT[1] = new Instruction("ADD", "01", 1, 2);
  IT[2] = new Instruction("SUB", "02", 1, 2);
  IT[3] = new Instruction("MUL", "03", 1, 2);
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

Assembler::Assembler(string sourceName) {
  this->sourceName = sourceName;

  buildIT();
}

void Assembler::printIT() {
  for (auto instruction:IT) {
    cout << instruction->mnemonic << endl;
  }
}

void Assembler::firstPassage() {
  ifstream sourceFile("processed/" + sourceName + ".mcr");

  // copy file content to string
  vector<string> intermediateCode;
  string line;

  while (getline(sourceFile, line))
    intermediateCode.push_back(line);

  int positionCounter = 0;

  // while there is source code
  for (int i = 0; i < intermediateCode.size(); i++) {
    
    // get line from source code
    line = intermediateCode[i];

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
            }
          }

          // if operation is not a instruction
          if (!instructionFound) {
            // if operation is directive CONST
            if (command->operation == "CONST")
              positionCounter += 1;

            // if operation is directive SPACE
            else if (command->operation == "SPACE") {
              if (ss >> token) {
                positionCounter += stoi(token);
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
          }
        }
      }
    }
  }

  // for (int i = 0; i < TS.size(); i++)
  //   cout << "Symbol: " << TS[i]->getSymbol() << "; Value: " << TS[i]->getValue() << endl;

  // close file
  sourceFile.close();
}

void Assembler::secondPassage() {
  ifstream inputFile("processed/" + sourceName + ".mcr");
  ofstream outputFile("processed/" + sourceName + ".o");

  // copy file content to string
  vector<string> intermediateCode;
  string line;

  while (getline(sourceFile, line))
    intermediateCode.push_back(line);

  int positionCounter = 0;

  inputFile.close();
  outputFile.close();
}

void Assembler::processFile() {
  firstPassage();
  secondPassage();
}