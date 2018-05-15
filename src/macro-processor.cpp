#include "../include/macro-processor.hpp"

// macroName constructor
MacroName::MacroName() {};

// macroDefinition constructor
MacroDefinition::MacroDefinition() {};

// macroProcessor constructor
MacroProcessor::MacroProcessor(string sourceName) {
  this->sourceName = sourceName;
  macroDefinitionCounter = 0;

  for (int i = 0; i < 100; i++)
    MDT.push_back(new MacroDefinition());
}

// get all macros
vector<string> MacroProcessor::getMacros() {
  // new source without MACRO
  vector<string> newSource;

  ifstream sourceFile("processed/" + sourceName + ".pre");

  // copy file content to string
  vector<string> intermediateCode;
  string line;

  while (getline(sourceFile, line))
    intermediateCode.push_back(line);

  // close file
  sourceFile.close();

  // prepare to match MACRO
  smatch match;

  MacroName * macroName;
  MacroDefinition * macroDefinition;

  for (int i = 0; i < intermediateCode.size(); i++) {
    // cout << "code[" << i << "]" << intermediateCode[i] << endl;
    
    // if line contains MACRO
    if (regex_search(intermediateCode[i], match, regex(": MACRO"))) {
      // get MACRO name
      regex_search(intermediateCode[i], match, regex("(.+)(:)"));
      string name(match[0]);
      name.erase(remove(name.begin(), name.end(), ':'), name.end());

      // get MACRO args count
      regex_search(intermediateCode[i], match, regex("(&)"));
      int args = match.size();

      macroName = new MacroName();

      macroName->setName(name);
      macroName->setArgs(args);
      macroName->setIndex(macroDefinitionCounter);

      // push MACRO into MNT
      MNT.push_back(macroName);

      macroDefinitionCounter++;

      // get current MACRO defnition
      macroDefinition = new MacroDefinition();
      string definitionBuffer = "";

      i++;

      while (!regex_search(intermediateCode[i], match, regex("ENDMACRO"))) {
        definitionBuffer.append(intermediateCode[i] + "\n");
        i++;
      }

      macroDefinition->setDefinition(definitionBuffer);
      
      // push MACRO definition into MDT
      MDT[macroName->getIndex()] = macroDefinition;
    } else {
      newSource.push_back(intermediateCode[i]);
    }
  }

  return newSource;
}

void MacroProcessor::expandMDT() {
  string definition;

  // for each MACRO in MNT
  for (auto macro:MNT) {
    // cout << macro->getName() << endl;

    // for each MACRO definition in MDT
    for (int i = 0; i < macroDefinitionCounter; i++) {
      definition = MDT[i]->getDefinition();

      // if MACRO name is present in MDT definition
      if (definition.find(macro->getName()) != -1) {
        
        // expand it
        definition.replace(definition.find(macro->getName()), macro->getName().size(), MDT[macro->getIndex()]->getDefinition());

        // update MDT
        MDT[i]->setDefinition(definition);
      }
    }
  }
}

void MacroProcessor::expandMacroCalls(vector<string> source) {
  ofstream output("processed/" + sourceName + ".mcr");
  vector<string> outputCode;
  bool found = false;

  // for each line in source
  for (auto line:source) {

    // for each MACRO name in MNT
    for (auto macro:MNT) {
      
      // if line has MACRO call
      if (line.find(macro->getName()) != -1) {
        // cout << "Found " << macro->getName() << endl;
        
        // expand it
        outputCode.push_back(MDT[macro->getIndex()]->getDefinition());
        // output << MDT[macro->getIndex()]->getDefinition() << endl;

        found = true;
      }
    }

    if (!found)
      outputCode.push_back(line + "\n");

    found = false;
  }

  for (auto str:outputCode)
    if (str != "\n" )
      output << str;

  output.close();
}

void MacroProcessor::printMNT() {
  ofstream file("tmp/MNT.txt");

  for (vector<MacroName *>::iterator it = MNT.begin(); it != MNT.end(); it++) {
    file << (*it)->getName() << " " << (*it)->getArgs() << " " << (*it)->getIndex() << endl;
  }

  file.close();
}

void MacroProcessor::printMDT() {
  ofstream file("tmp/MDT.txt");

  for (int i = 0; i < macroDefinitionCounter; i++) {
    file << "Line " << i << ":" << endl;
    file << MDT[i]->getDefinition() << endl;
  }

  file.close();
}

void MacroProcessor::processFile() {
  vector<string> newSource = getMacros();
  expandMDT();
  expandMacroCalls(newSource);

  printMNT();
  printMDT();
}
