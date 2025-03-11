#include "grammar.h"

bool Grammar::isNonTerminal(char symbol) const {
    bool find = false;

    for (char nonTerminalSymbol : nonTerminalSymbols) {
        if (nonTerminalSymbol == symbol) {
            find = true;

            break;
        }
    }

    return find;
}

vector<string> Grammar::getRules(char nonTerminal) const {
    return rules.at(nonTerminal);
}

string Grammar::getRule(char nonTerminal, int index) const {
    return getRules(nonTerminal)[index];
}

string Grammar::getRule(const Element& element) const {
    return getRule(element.symbol, element.index);
}

int Grammar::getNumber(char nonTerminal, int index) const {
    int sum = 1;

    for (char nonTerminalSymbol : nonTerminalSymbols) {
        if (nonTerminalSymbol == nonTerminal) {
            break;
        }

        sum += rules.at(nonTerminalSymbol).size();
    }

    return sum + index;
}

int Grammar::getNumber(const Element& element) const {
    return getNumber(element.symbol, element.index);
}