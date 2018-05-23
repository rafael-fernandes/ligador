#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

class Command {
  public:
    string label, operation;
    vector<string> operands;
};

#endif