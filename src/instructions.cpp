#include "../include/instructions.hpp"

Instruction::Instruction(string mnemonic, string opcode, int length) {
  this->mnemonic = mnemonic;
  this->opcode = opcode;
  this->length = length;
}