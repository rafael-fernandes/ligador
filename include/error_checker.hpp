#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include "../include/symbol.hpp"
#include "../include/instructions.hpp"
#include "../include/command.hpp"

using namespace std;

class ErrorChecker {
  public:
    ErrorChecker(string source, string target);

    void check();

    string sourceFileName, targetFileName;

    Instruction * IT[15];

    vector<Symbol *> TS;
};