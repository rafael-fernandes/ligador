#include <iostream>
#include "../include/instructions.hpp"

using namespace std;

class Assembler {
  public:
    Assembler();

    void printIT();

    void firstPassage();
    void secondPassage();

  private:
    Instruction * IT[15];
};