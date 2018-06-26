#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include "../include/instructions.hpp"
#include "../include/symbol.hpp"

using namespace std;

class Linker {
  public:
    Linker(string modA, string modB);
    
    void buildIT();
    void buildTGD();
    void printTGD();

    void linkObjects();

    int instructionOffset(string);
    Instruction * IT[15];

    int correctionFactor;
    ifstream modACode, modBCode;
    ifstream linkerTmpModA, linkerTmpModB;

    vector<string> intermediateObjectA, intermediateObjectB;

  private:
    vector<Symbol *> TGD;
};