#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

class ProgramLine {
  public:
    int number;
    string source;
};

class Program {
  public:
    vector<ProgramLine *> lines;
};

#endif