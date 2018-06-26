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

  private:
    Instruction * IT[15];

    vector<Symbol *> TGD;
};