#include "../include/error_checker.hpp"

ErrorChecker::ErrorChecker(string source, string target) {
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

  this->sourceFileName = source;
  this->targetFileName = target;
};

void ErrorChecker::check() {
  ifstream sourceFile(targetFileName + ".asm");

  // copy file content to string
  vector<string> intermediateCode;
  string line;

  while (getline(sourceFile, line))
    intermediateCode.push_back(line);

  // while there is source code
  for (int i = 0; i < intermediateCode.size(); i++) {
    
    // get line from source code
    line = intermediateCode[i];

    // toUpper
    transform(line.begin(), line.end(), line.begin(), ::toupper);

    // removeComments
    int commentPosition;

    commentPosition = line.find(";");

    if (commentPosition != -1)
      line.erase(commentPosition, line.size());

    // ERROR: line has 2 labels
    if (count(line.begin(), line.end(), ':') > 1)
      cout << "Line: " << i + 1 << ", \033[1;31msintatic error:\033[0m line has more than 1 label" << endl;

    stringstream ss(line);
    string token;

    while (ss >> token) {
      // token is SECTION
      if (token.find("SECTION") != -1) {
        if (ss >> token) {
          // ERROR: invalid section
          if (token != "TEXT" && token != "DATA") {
            cout << "Line: " << i + 1 << ", \033[1;31msintatic error:\033[0m invalid section" << endl;
          }
        }

        // ERROR: empty section
        else
          cout << "Line: " << i + 1 << ", \033[1;31msintatic error:\033[0m empty section" << endl;
      }

      // token is a label
      if (token.find(":") != -1) {
        token = token.substr(0, token.find(":"));

        // insert symbol into TS
        Symbol * symbol = new Symbol();
        symbol->setSymbol(token);
        TS.push_back(symbol);

        // ERROR: label has more than 20 characters
        if (token.size() > 20) {
          cout << "Line: " << i + 1 << ", \033[1;31mlexic error:\033[0m token " << token << " has more than 20 characters" << endl;
        }

        // ERROR: label has number as first character
        if ('0' <= token[0] && token[0] <= '9') {
          cout << "Line: " << i + 1 << ", \033[1;31mlexic error:\033[0m token " << token << " begin with a number" << endl;
        }

        // ERROR: label character that is not A-Z or _
        if (!('0' <= token[0] && token[0] <= '9') && token.find_first_not_of("ABCDEFGHIJKLMNOPQRSTUVWXYZ_0123456789") != -1) {
          cout << "Line: " << i + 1 << ", \033[1;31mlexic error:\033[0m token " << token << " has invalid character" << endl;
        }
      }
    }
  }

  // close file
  sourceFile.close();
}