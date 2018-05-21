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
    // cout << "code[" << i << "]: " << intermediateCode[i] << endl;

    // if line contains MACRO
    if (regex_search(intermediateCode[i], match, regex(": MACRO"))) {
      // get MACRO name
      regex_search(intermediateCode[i], match, regex("(.+)(:)"));
      string name(match[0]);
      name.erase(remove(name.begin(), name.end(), ':'), name.end());

      // prepare to get MACRO args
      stringstream ss(intermediateCode[i]);
      string token;

      vector<string> argsVector;

      // tokenize
      while (ss >> token) {
        // if token is arg
        if (regex_search(token, match, regex("&"))) {
          // remove whitespace
          token.erase(remove(token.begin(), token.end(), ' '), token.end());

          // remove ;
          token.erase(remove(token.begin(), token.end(), ','), token.end());

          // push into args vector of MACRO
          argsVector.push_back(token);
        }
      }

      // create new MACRO name
      macroName = new MacroName();
      
      macroName->setName(name);
      macroName->setArgs(argsVector.size());
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

        stringstream ss(intermediateCode[i]);
        string token;

        while (ss >> token)
          for (int z = 0; z < argsVector.size(); z++)
            if (token == argsVector[z]) {
              // cout << "match: " << token << " at line " << i << endl;
              // cout << "token init at definitionBuffer: " << definitionBuffer.find(token) << endl;
              definitionBuffer.replace(definitionBuffer.find(token), token.size(), to_string(z));
            }

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

      stringstream ss(definition);
      string line;

      // split MACRO definition into lines
      while (getline(ss, line, '\n')) {

        // if MACRO name is present in MDT definition line
        if (line.find(macro->getName()) != -1) {
          // set previous line as current line
          string previousLine = line;

          stringstream ss(line);
          string token;
          vector<string> argsVector;

          // get args from MACRO call
          if (ss >> token) {
            for (int j = 0; j < macro->getArgs(); j++) {
              ss >> token;

              // remove whitespace
              token.erase(remove(token.begin(), token.end(), ' '), token.end());

              // remove ,
              token.erase(remove(token.begin(), token.end(), ','), token.end());

              argsVector.push_back(token);
              // cout << j << ": " << token << endl;
            }
          }

          // replace line by definition
          line.replace(
            0, line.size(),
            MDT[macro->getIndex()]->getDefinition()
          );

          // replace generic args by MACRO call args
          for (int j = 0; j < macro->getArgs(); j++) {
            line.replace(
              line.find(to_string(j)), 1,
              argsVector[j]
            );
          }

          // replace previous line by new line with expanded definition
          definition.replace(
            definition.find(previousLine),
            previousLine.size(),
            line
          );

          // update MDT
          MDT[i]->setDefinition(definition);
        }
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
      
      // if MACRO name is present in MDT definition line
      if (line.find(macro->getName()) != -1) {
        // set previous line as current line
        string previousLine = line;

        stringstream ss(line);
        string token;
        vector<string> argsVector;

        // get args from MACRO call
        if (ss >> token) {
          for (int j = 0; j < macro->getArgs(); j++) {
            ss >> token;

            // remove whitespace
            token.erase(remove(token.begin(), token.end(), ' '), token.end());

            // remove ,
            token.erase(remove(token.begin(), token.end(), ','), token.end());

            argsVector.push_back(token);
            // cout << j << ": " << token << endl;
          }
        }

        // replace line by definition
        line.replace(
          0, line.size(),
          MDT[macro->getIndex()]->getDefinition()
        );

        // replace generic args by MACRO call args
        for (int j = 0; j < macro->getArgs(); j++) {
          line.replace(
            line.find(to_string(j)), 1,
            argsVector[j]
          );
        }

        outputCode.push_back(line);

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
