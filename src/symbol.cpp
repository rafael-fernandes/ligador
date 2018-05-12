#include "../include/symbol.hpp"

Symbol::Symbol() {}

Symbol::Symbol(string symbol, string value) {
  this->symbol = symbol;
  this->value = value;
}