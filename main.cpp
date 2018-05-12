#include <iostream>
#include "include/pre-processor.hpp"

int main() {
  PreProcessor * preprocessor = new PreProcessor("teste2");

  preprocessor->processFile();

  return 0;
}