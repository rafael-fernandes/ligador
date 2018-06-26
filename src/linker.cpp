#include "../include/linker.hpp"

void Linker::buildIT() {
  IT[1] = new Instruction("ADD", "01", 1, 2);
  IT[2] = new Instruction("SUB", "02", 1, 2);
  IT[3] = new Instruction("MULT", "03", 1, 2);
  IT[4] = new Instruction("DIV", "04", 1, 2);
  IT[5] = new Instruction("JMP", "05", 1, 2);
  IT[6] = new Instruction("JMPN", "06", 1, 2);
  IT[7] = new Instruction("JMPP", "07", 1, 2);
  IT[8] = new Instruction("JMPZ", "08", 1, 2);
  IT[9] = new Instruction("COPY", "09", 2, 3);
  IT[10] = new Instruction("LOAD", "10", 1, 2);
  IT[11] = new Instruction("STORE", "11", 1, 2);
  IT[12] = new Instruction("INPUT", "12", 1, 2);
  IT[13] = new Instruction("OUTPUT", "13", 1, 2);
  IT[14] = new Instruction("STOP", "14", 0, 1);
}

Linker::Linker(string modA, string modB) {
  modACode.open("processed/" + modA + ".o");
  modBCode.open("processed/" + modB + ".o");

  buildIT();
}

void Linker::buildTGD() {
  string line, token, aux;

  int TD_Symbols;

  while (getline(modACode, line))
    intermediateObjectA.push_back(line);

  while (getline(modBCode, line))
    intermediateObjectB.push_back(line);

  string SIZE_A = intermediateObjectA[1];

  stringstream sizeA(SIZE_A);

  // discard H:
  sizeA >> token;

  // get real size
  sizeA >> aux;
  this->correctionFactor = stoi(aux) + 1;

  string TD_A = intermediateObjectA[3];
  string TD_B = intermediateObjectB[3];

  stringstream ssA(TD_A);

  // discard H:
  ssA >> token;

  // get TDA symbols count
  ssA >> aux;
  TD_Symbols = stoi(aux);

  for (int i = 0; i < TD_Symbols; i++) {
    Symbol * symbol = new Symbol();

    ssA >> token;
    symbol->setSymbol(token);

    ssA >> token;
    symbol->setValue(token);

    TGD.push_back(symbol);
  }

  stringstream ssB(TD_B);

  // discard H:
  ssB >> token;

  // get TDB symbols count
  ssB >> aux;
  TD_Symbols = stoi(aux);

  for (int i = 0; i < TD_Symbols; i++) {
    Symbol * symbol = new Symbol();

    ssB >> token;
    symbol->setSymbol(token);

    ssB >> token;
    int auxValue = stoi(token) + correctionFactor;
    symbol->setValue(to_string(auxValue));

    TGD.push_back(symbol);
  }
}

void Linker::printTGD() {
  cout << "GENERAL DEFINITION TABLE" << endl;

  for (auto symbol:TGD)
    cout << "Symbol: " << symbol->getSymbol() << "; Value: " << symbol->getValue() << endl;
}

void Linker::linkObjects() {
  ofstream outputFile("processed/object.o");

  buildTGD();
  printTGD();

  vector<string> textA;
  vector<string> textB;

  string line, token, aux;

  // get text mod A
  stringstream ssTextA(intermediateObjectA[4]);

  // discard T:
  ssTextA >> token;

  while(ssTextA >> token)
    textA.push_back(token);

  // print text mod A
  for (auto code:textA)
    cout << code << " ";

  cout << endl;

  // get text mod B
  stringstream ssTextB(intermediateObjectB[4]);

  // discard T:
  ssTextB >> token;

  while(ssTextB >> token)
    textB.push_back(token);

  // print text mod B
  for (auto code:textB)
    cout << code << " ";

  cout << endl;

  // get TUA symbols count
  // cout << intermediateObjectA[2] << endl;
  stringstream ssTUA(intermediateObjectA[2]);

  // discard H:
  ssTUA >> token;

  ssTUA >> aux;
  int TUA_Symbols = stoi(aux);

  for (int i = 0; i < TUA_Symbols; i++) {
    string symbol, value;

    ssTUA >> symbol;
    ssTUA >> value;

    for (auto token:TGD) {
      if (token->getSymbol() == symbol)
        textA[stoi(value)] = token->getValue();
    }
  }

  // print text mod A
  for (auto code:textA)
    cout << code << " ";

  cout << endl;

  // get TUB symbols count
  stringstream ssTUB(intermediateObjectB[2]);

  // discard H:
  ssTUB >> token;

  // get TUA symbols count
  ssTUB >> aux;
  int TUB_Symbols = stoi(aux);

  for (int i = 0; i < TUB_Symbols; i++) {
    string symbol, value;

    ssTUB >> symbol;
    ssTUB >> value;

    for (auto token:TGD) {
      if (token->getSymbol() == symbol)
        textB[stoi(value)] = token->getValue();
    }
  }

  // print text mod B
  for (auto code:textB)
    cout << code << " ";

  cout << endl;
}