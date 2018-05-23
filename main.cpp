#include <iostream>
#include "include/pre-processor.hpp"
#include "include/macro-processor.hpp"
#include "include/assembler.hpp"

int main() {
  PreProcessor * preProcessor = new PreProcessor("teste2");
  MacroProcessor * macroProcessor = new MacroProcessor("teste2");
  Assembler * assembler = new Assembler("teste2");

  preProcessor->processFile();
  macroProcessor->processFile();
  assembler->processFile();

  return 0;
}