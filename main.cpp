#include <iostream>
#include "include/pre-processor.hpp"
#include "include/macro-processor.hpp"
#include "include/assembler.hpp"
#include "include/linker.hpp"
#include "include/error_checker.hpp"

int main(int argc, char ** argv) {
  // input: ./bin/main prog1
  // Só monta

  if (argc == 2) {
    string source(argv[1]);

    ErrorChecker * errorChecker = new ErrorChecker(source);
    errorChecker->check();

    PreProcessor * preProcessor = new PreProcessor(source);
    MacroProcessor * macroProcessor = new MacroProcessor(source);
    
    Assembler * assembler = new Assembler(source);
    assembler->module = false;

    preProcessor->processFile();
    macroProcessor->processFile();
    assembler->processFile();
  }

  // input: ./bin/main prog1 prog2
  // Monta e liga

  if (argc == 3) {
    string modA(argv[1]);
    string modB(argv[2]);

    ErrorChecker * errorCheckerModA = new ErrorChecker(modA);
    ErrorChecker * errorCheckerModB = new ErrorChecker(modB);

    errorCheckerModA->check();
    errorCheckerModB->check();

    PreProcessor * preProcessorModA = new PreProcessor(modA);
    PreProcessor * preProcessorModB = new PreProcessor(modB);

    MacroProcessor * macroProcessorModA = new MacroProcessor(modA);
    MacroProcessor * macroProcessorModB = new MacroProcessor(modB);

    Assembler * assemblerModA = new Assembler(modA);
    assemblerModA->module = true;

    Assembler * assemblerModB = new Assembler(modB);
    assemblerModB->module = true;

    preProcessorModA->processFile();
    preProcessorModB->processFile();

    macroProcessorModA->processFile();
    macroProcessorModB->processFile();

    assemblerModA->processFile();
    assemblerModB->processFile();

    Linker * linker = new Linker(modA, modB);
    linker->linkObjects();

    cout << "A saida está em ./processed/object.o" << endl;
  }

  return 0;
}