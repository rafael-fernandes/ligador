#include "../include/pre-processor.hpp"

PreProcessor::PreProcessor(string file) {
  source.open(file + ".asm");

  if (!source.good()) {
    cout << "Error opening file: " << file << endl;
    exit(1);
  }

  processedSource.open("processed/" + file + ".pre");
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

bool PreProcessor::isDefined(string symbol) {
  for (list<Symbol *>::iterator it = equList.begin(); it != equList.end(); it++)
    if ((*it)->getSymbol() == symbol)
      return true;

  return false;
}

bool PreProcessor::getEQU(string line) {
  string str;
  stringstream iss(line);
  vector<string> aux;

  if (line.find("EQU") != -1) {
    while (getline(iss, str, ':')) {
      aux.push_back(str);
    }

    string symbol = removeWhiteSpace(aux[0]);
    
    string value = removeWhiteSpace(aux[1]);
    value.erase(value.find("EQU"), value.find("EQU") + 3);

    if (isDefined(symbol)) {
      cout << "Semantic error: symbol " << symbol << " already defined" << endl;
      exit(0);
    } else {
      equList.push_back(new Symbol(symbol, value));
    }

    return true;
  }

  return false;
}

bool PreProcessor::getIF(string line) {
  string str;
  stringstream iss(line);
  vector<string> aux;

  if (line.find("IF") != -1) {
    while (getline(iss, str, ' ')) {
      aux.push_back(str);
    }

    string symbol = removeWhiteSpace(aux[0]);
    string value = removeWhiteSpace(aux[1]);

    if (!isDefined(value)) {
      cout << "Semantic error: symbol " << value << " not defined" << endl;
      exit(0);
    } else {
      ifList.push_back(new Symbol(symbol, value));
    }

    return true;
  }

  return false;
}

string PreProcessor::translateEQU(string line) {
  stringstream ss(line);
  string token;
  int position;

  while (ss >> token) {
    for (list<Symbol *>::iterator it = equList.begin(); it != equList.end(); it++) {
      if ((*it)->getSymbol() == token) {
        position = line.find((*it)->getSymbol());
        line.replace(position, (*it)->getSymbol().size(), (*it)->getValue());
      }
    }
  }

  return line;
}

bool PreProcessor::evaluateIF() {
  Symbol * ifSymbol = ifList.back();

  ifList.pop_back();

  for (list<Symbol *>::iterator it = equList.begin(); it != equList.end(); it++) {
    if ((*it)->getSymbol() == ifSymbol->getValue() && stoi((*it)->getValue()) == 0) {
      return true;
    }
  }

  return false;
};

void PreProcessor::listEQU() {
  for (list<Symbol *>::iterator it = equList.begin(); it != equList.end(); it++) {
    cout << "Symbol: " << (*it)->getSymbol() << " | Value: " << (*it)->getValue() << endl;
  }
}

void PreProcessor::listIF() {
  for (list<Symbol *>::iterator it = ifList.begin(); it != ifList.end(); it++) {
    cout << "Symbol: " << (*it)->getSymbol() << " | Value: " << (*it)->getValue() << endl;
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
 
      if (!getEQU(newLine)) {
        if (!getIF(newLine)) {
          newLine = translateEQU(newLine);
          processedSource << newLine << endl;
        } else {
          blocked = evaluateIF();
        }
      }
    } else {
      blocked = false;
    }
  }

  source.close();
  processedSource.close();
}