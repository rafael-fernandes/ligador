#include "../include/assembler.hpp"

Assembler::Assembler() {
  IT[1] = new Instruction("ADD", "01", 2);
  IT[2] = new Instruction("SUB", "02", 2);
  IT[3] = new Instruction("MUL", "03", 2);
  IT[4] = new Instruction("DIV", "04", 2);
  IT[5] = new Instruction("JMP", "05", 2);
  IT[6] = new Instruction("JMPN", "06", 2);
  IT[7] = new Instruction("JMPP", "07", 2);
  IT[8] = new Instruction("JMPZ", "08", 2);
  IT[9] = new Instruction("COPY", "09", 3);
  IT[10] = new Instruction("LOAD", "10", 2);
  IT[11] = new Instruction("STORE", "11", 2);
  IT[12] = new Instruction("INPUT", "12", 2);
  IT[13] = new Instruction("OUTPUT", "13", 2);
  IT[14] = new Instruction("STOP", "14", 1);
}

void Assembler::printIT() {
  for (auto instruction:IT) {
    cout << instruction->mnemonic << endl;
  }
}

void Assembler::firstPassage() {
  
}