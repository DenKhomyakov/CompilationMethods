#pragma once

#ifndef GRAMMAR_H
#define GRAMMAR_H

#include <vector>
#include <map>
#include <string>
#include <algorithm>

using namespace std;

struct Element {
    char symbol;
    int index;

    Element(char symbol, int index = -1) : symbol(symbol), index(index) {}
};

class Grammar {
public:
    char startSymbol;
    vector<char> terminalSymbols;
    vector<char> nonTerminalSymbols;
    map<char, vector<string>> rules;

    bool isNonTerminal(char symbol) const;
    vector<string> getRules(char nonTerminal) const;
    string getRule(char nonTerminal, int index) const;
    string getRule(const Element& elem) const;
    int getNumber(char nonTerminal, int index) const;
    int getNumber(const Element& elem) const;
};

#endif // GRAMMAR_H