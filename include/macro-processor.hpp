#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <algorithm>
#include <regex>

using namespace std;

class MacroDefinition {
  public:
    MacroDefinition();

    void inline setDefinition(string definition);
    string inline getDefinition();

  private:
    string definition;
};

string inline MacroDefinition::getDefinition() {
  return definition;
}

void inline MacroDefinition::setDefinition(string definition) {
  this->definition = definition;
}

class MacroName {
  public:
    MacroName();

    void inline setName(string name);
    void inline setArgs(int args);
    void inline setIndex(int index);

    string inline getName();
    int inline getArgs();
    int inline getIndex();

  private:
    string name;
    int args;
    int index;
};

void inline MacroName::setName(string name) {
  this->name = name;
}

void inline MacroName::setArgs(int args) {
  this->args = args;
}

void inline MacroName::setIndex(int index) {
  this->index = index;
}

string inline MacroName::getName() {
  return this->name;
}

int inline MacroName::getArgs() {
  return this->args;
}

int inline MacroName::getIndex() {
  return this->index;
}

class MacroProcessor {
  public:
    MacroProcessor(string source);

    vector<string> getMacros();
    void expandMDT();
    void expandMacroCalls(vector<string> source);

    void mountTables();
    void replaceMacros();
    void processFile();

    bool getMacroName(string line);
    void setMacroDefinition(string buffer);
    bool isEndMacro(string line);

    int inline getMacroDefinitionCounter();

    void printMNT();
    void printMDT();

    string sourceFileName, targetFileName;

  private:
    ifstream source;
    ofstream processedSource;

    vector<MacroName *> MNT;

    int macroDefinitionCounter;
    vector<MacroDefinition *> MDT;
};

int inline MacroProcessor::getMacroDefinitionCounter() {
  return this->macroDefinitionCounter;
}
