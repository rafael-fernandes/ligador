#include <iostream>
#include "include/pre-processor.hpp"
#include "include/macro-processor.hpp"
#include "include/assembler.hpp"
#include "include/error_checker.hpp"

int main(int argc, char ** argv) {
  // input: ./main prog1
  // Só monta

  if (argc == 2) {
    
  }

  // input: ./main prog1 prog2
  // Monta e liga

  if (argc == 3) {

  }

  // string flag(argv[1]);
  // string source(argv[2]);
  // string target(argv[3]);

  // ErrorChecker * errorChecker = new ErrorChecker(source, target);
  // errorChecker->check();

  // PreProcessor * preProcessor = new PreProcessor(source, target);
  // MacroProcessor * macroProcessor = new MacroProcessor(source, target);
  // Assembler * assembler = new Assembler(source, target);

  // if (flag == "-p") {
  //   preProcessor->processFile();
  // }

  // else if (flag == "-m") {
  //   preProcessor->processFile();
  //   macroProcessor->processFile();
  // }

  // else if (flag == "-o") {
  //   preProcessor->processFile();
  //   macroProcessor->processFile();
  //   assembler->processFile();
  // }

  return 0;
}