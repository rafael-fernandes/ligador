#include <iostream>
#include "include/pre-processor.hpp"
#include "include/macro-processor.hpp"
#include "include/assembler.hpp"
#include "include/program.hpp"

Program * program = new Program();

int main(int argc, char ** argv) {
  // input: ./prog -[p|m|c] sourceFileName targetFileName

  string flag(argv[1]);
  string source(argv[2]);
  string target(argv[3]);

  PreProcessor * preProcessor = new PreProcessor(source, target);
  MacroProcessor * macroProcessor = new MacroProcessor(source, target);
  Assembler * assembler = new Assembler(source, target);

  if (flag == "-p") {
    preProcessor->processFile();
  }

  else if (flag == "-m") {
    preProcessor->processFile();
    macroProcessor->processFile();
  }

  else if (flag == "-o") {
    preProcessor->processFile();
    macroProcessor->processFile();
    assembler->processFile();
  }

  return 0;
}