#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "../include/instructions.hpp"
#include "../include/symbol.hpp"
#include "../include/command.hpp"

using namespace std;

class Assembler {
  public:
    Assembler(string source, string target);
    
    void buildIT();

    void printIT();
    void printTS();
    void printTU();
    void printTD();
    void printTextSection();
    void printDataSection();

    void firstPassage();
    void secondPassage();

    void processFile();

    string sourceFileName, targetFileName;

  private:
    Instruction * IT[15];

    vector<Symbol *> TS;
    vector<Symbol *> TU;
    vector<Symbol *> TD;
    vector<Command *> textSection;
    vector<Command *> dataSection;
};