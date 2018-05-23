#include "../include/instructions.hpp"

Instruction::Instruction(string mnemonic, string opcode, int operands, int length) {
  this->mnemonic = mnemonic;
  this->opcode = opcode;
  this->operands = operands;
  this->length = length;
}