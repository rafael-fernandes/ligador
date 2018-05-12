#include "../include/pre-processor.hpp"

PreProcessor::PreProcessor(string file) {
  source.open(file + ".asm");

  if (!source.good()) {
    cout << "Error opening file: " << file << endl;
    exit(1);
  }

  processedSource.open("processed/" + file + "_processed.asm");
}

string PreProcessor::toUpper(string line) {
  transform(line.begin(), line.end(), line.begin(), ::toupper);
  
  return line;
}

string PreProcessor::removeComment(string line) {
  int commentPosition;

  commentPosition = line.find(";");

  if (commentPosition != -1) {
    line.erase(commentPosition, line.size());
  }

  return line;
}

bool BothAreSpaces(char lhs, char rhs) {
  return (lhs == rhs) && (lhs == ' ');
}

string PreProcessor::removeExtraSpaces(string line) {
  // remove double spaces
  string::iterator new_end = unique(line.begin(), line.end(), BothAreSpaces);
  line.erase(new_end, line.end());

  // if first char of line is a space, remove this
  if (line.front() == ' ') {
    line.erase(0, 1);
  }

  // if last char of line is a space, remove this
  if (line.back() == ' ') {
    line.pop_back();
  }

  return line;
}

string removeWhiteSpace(string str) {
  str.erase(remove(str.begin(), str.end(), ' '), str.end());

  return str;
}

bool PreProcessor::getEQU(string line) {
  string str;
  stringstream iss(line);
  vector<string> pieces;
  Symbol * symbol;

  if (line.find("EQU") != -1) {
    while (getline(iss, str, ':')) {
      pieces.push_back(str);
    }

    pieces[0] = removeWhiteSpace(pieces[0]);
    pieces[1].erase(pieces[1].find("EQU"), pieces[1].find("EQU") + 3);

    symbol = new Symbol(pieces[0], pieces[1]);

    equList.push_back(symbol);

    return true;
  }

  return false;
}

bool PreProcessor::getIF(string line) {
  string str;
  stringstream iss(line);
  vector<string> pieces;

  if (line.find("IF") != -1) {
    while (getline(iss, str, ' ')) {
      pieces.push_back(str);
    }

    ifList.push_back(new Symbol(pieces[0], pieces[1]));

    return true;
  }

  return false;
}

bool PreProcessor::evaluateIF() {
  Symbol * ifSymbol = ifList.back();

  ifList.pop_back();

  cout << "Found symbol: " << ifSymbol->getValue() << endl;

  for (list<Symbol *>::iterator it = equList.begin(); it != equList.end(); it++) {
    cout << "Scanning EQU symbol: " << (*it)->getSymbol() << " and value: " << stoi((*it)->getValue()) << endl;

    if ((*it)->getSymbol() == ifSymbol->getValue() && stoi((*it)->getValue()) == 0) {
      return true;
    }
  }
};

void PreProcessor::listEQU() {
  for (list<Symbol *>::iterator it = equList.begin(); it != equList.end(); it++) {
    cout << (*it)->getSymbol() << " " << (*it)->getValue() << endl;
  }
}

void PreProcessor::listIF() {
  for (list<Symbol *>::iterator it = ifList.begin(); it != ifList.end(); it++) {
    cout << (*it)->getSymbol() << " " << (*it)->getValue() << endl;
  }
}

void PreProcessor::processFile() {
  string line, newLine;
  bool blocked = false;

  while(getline(source, line)) {
    if (!blocked) {
      newLine = toUpper(line);
      newLine = removeComment(newLine);
      newLine = removeExtraSpaces(newLine);

      if (!getEQU(newLine) && !getIF(newLine)) {
        processedSource << newLine << endl;
      }

      if (getIF(newLine)) {
        blocked = evaluateIF();
      }
    } else {
      blocked = false;
    }
  }

  // listEQU();
  // listIF();

  source.close();
  processedSource.close();
}