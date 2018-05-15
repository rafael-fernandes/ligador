#include <iostream>
#include "include/pre-processor.hpp"
#include "include/macro-processor.hpp"

int main() {
  PreProcessor * preProcessor = new PreProcessor("teste2");
  MacroProcessor * macroProcessor = new MacroProcessor("teste2");

  preProcessor->processFile();
  macroProcessor->processFile();

  return 0;
}