#include <iostream>
#include <string>

using namespace std;

class Instruction {
  public:
    Instruction(string mnemonic, string opcode, int operands, int length);

    string mnemonic, opcode;
    int operands, length;
};