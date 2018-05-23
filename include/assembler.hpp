#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "../include/instructions.hpp"
#include "../include/symbol.hpp"

using namespace std;

class Command {
  public:
    string label, operation, op1, op2;
};

class Assembler {
  public:
    Assembler(string sourceName);
    
    void buildIT();
    void printIT();

    void firstPassage();
    void secondPassage();

    void processFile();

  private:
    Instruction * IT[15];
    string sourceName;

    vector<Symbol *> TS;
};