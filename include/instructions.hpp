#include <iostream>
#include <string>

using namespace std;

class Instruction {
  public:
    Instruction(string mnemonic, string opcode, int length);

    string mnemonic, opcode;
    int length;
};