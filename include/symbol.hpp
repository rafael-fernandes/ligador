#ifndef SYMBOL_H
#define SYMBOL_H

#include <algorithm>
#include <iostream>

using namespace std;

class Symbol {
  private:
    string symbol;
    string value;

  public:
    Symbol();
    Symbol(string symbol, string value);

    inline string getSymbol();
    inline string getValue();

    inline void setSymbol(string symbol);
    inline void setValue(string value);
};

inline string Symbol::getSymbol() {
  return this->symbol;
}

inline string Symbol::getValue() {
  return this->value;
}

inline void Symbol::setSymbol(string symbol) {
  this->symbol = symbol;
}

inline void Symbol::setValue(string value) {
  this->value = value;
}

#endif