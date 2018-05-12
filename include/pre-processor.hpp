#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <list>
#include <vector>
#include <algorithm>
#include "../include/symbol.hpp"

using namespace std;

class PreProcessor {
  public:
    PreProcessor(string file);
    void processFile();
    
    string toUpper(string line);
    string removeComment(string line);
    string removeExtraSpaces(string line);

    bool getEQU(string line);
    void listEQU();

    bool getIF(string line);
    bool evaluateIF();
    void listIF();

  private:
    ifstream source;
    ofstream processedSource;

    list<Symbol *> equList;
    list<Symbol *> ifList;
};
