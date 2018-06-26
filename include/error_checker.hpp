#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>
#include "../include/symbol.hpp"
#include "../include/instructions.hpp"
#include "../include/command.hpp"

using namespace std;

class ErrorChecker {
  public:
    ErrorChecker(string source);

    void check();

    string sourceFileName, targetFileName;

    Instruction * IT[15];

    vector<Symbol *> TS;
};