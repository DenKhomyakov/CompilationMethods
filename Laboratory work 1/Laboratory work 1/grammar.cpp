#include "grammar.h"

bool Grammar::isNonTerminal(char symbol) const {
    return find(nonTerminalSymbols.begin(), nonTerminalSymbols.end(), symbol) != nonTerminalSymbols.end();
}

std::vector<std::string> Grammar::getRules(char nonTerminal) const {
    return rules.at(nonTerminal);
}

std::string Grammar::getRule(char nonTerminal, int index) const {
    return getRules(nonTerminal)[index];
}

std::string Grammar::getRule(const Element& elem) const {
    return getRule(elem.symbol, elem.index);
}

int Grammar::getNumber(char nonTerminal, int index) const {
    int sum = 1; // Начинаем с 1, как в задании

    for (char nonTerminalSymbol : nonTerminalSymbols) {
        if (nonTerminalSymbol == nonTerminal) {
            break;
        }

        sum += rules.at(nonTerminalSymbol).size();
    }

    return sum + index;
}

int Grammar::getNumber(const Element& elem) const {
    return getNumber(elem.symbol, elem.index);
}