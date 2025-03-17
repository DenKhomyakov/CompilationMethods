#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

int convertStringToInt(const string& str) {
    int result;
    istringstream iss(str);
    iss >> result;

    return result;
}

using ActionTable = map<pair<int, char>, string>;
using GotoTable = map<pair<int, char>, int>;

void reduce(stack<int>& stateStack, stack<char>& symbolStack, int ruleLength, char nonTerminal, const GotoTable& gotoTable) {
    for (int i = 0; i < ruleLength; ++i) {
        if (stateStack.empty() || symbolStack.empty()) {
            throw runtime_error("Ошибка: стек пуст при выполнении свертки.");
        }

        stateStack.pop();
        symbolStack.pop();
    }

    symbolStack.push(nonTerminal);

    auto gotoIt = gotoTable.find({ stateStack.top(), nonTerminal });
    if (gotoIt == gotoTable.end()) {
        throw runtime_error("Ошибка: нет перехода в таблице goto для состояния " + to_string(stateStack.top()) + " и символа " + nonTerminal);
    }
    int newState = gotoIt->second;
    stateStack.push(newState);
}

int main() {
    setlocale(LC_ALL, "Rus");

    ActionTable actionTable = {
        {{0, '!'}, "s2"},
        {{1, '$'}, "acc"},
        {{2, '('}, "s3"},
        {{2, 'a'}, "s4"},
        {{2, 'b'}, "s5"},
        {{3, '('}, "s3"},
        {{3, 'a'}, "s4"},
        {{3, 'b'}, "s5"},
        {{4, '!'}, "r6"},
        {{4, '+'}, "r6"},
        {{4, '*'}, "r6"},
        {{4, ')'}, "r6"},
        {{5, '!'}, "r7"},
        {{5, '+'}, "r7"},
        {{5, '*'}, "r7"},
        {{5, ')'}, "r7"},
        {{6, '!'}, "s10"},
        {{7, '!'}, "r2"},
        {{7, '+'}, "s11"},
        {{7, ')'}, "r2"},
        {{8, '!'}, "r4"},
        {{8, '+'}, "r4"},
        {{8, '*'}, "s12"},
        {{8, ')'}, "r4"},
        {{9, ')'}, "s13"},
        {{10, '$'}, "r1"},
        {{11, '('}, "s3"},
        {{11, 'a'}, "s4"},
        {{11, 'b'}, "s5"},
        {{12, '('}, "s3"},
        {{12, 'a'}, "s4"},
        {{12, 'b'}, "s5"},
        {{13, '!'}, "r8"},
        {{13, '+'}, "r8"},
        {{13, '*'}, "r8"},
        {{13, ')'}, "r8"},
        {{14, '!'}, "r3"},
        {{14, ')'}, "r3"},
        {{15, '!'}, "r5"},
        {{15, '+'}, "r5"},
        {{15, ')'}, "r5"}
    };

    GotoTable gotoTable = {
        {{0, 'A'}, 1},
        {{2, 'B'}, 6},
        {{2, 'T'}, 7},
        {{2, 'M'}, 8},
        {{3, 'B'}, 9},
        {{3, 'T'}, 7},
        {{3, 'M'}, 8},
        {{6, 'B'}, 6},
        {{7, 'T'}, 7},
        {{8, 'M'}, 8},
        {{9, 'B'}, 9},
        {{11, 'B'}, 14},
        {{11, 'T'}, 7},
        {{11, 'M'}, 8},
        {{12, 'T'}, 15},
        {{12, 'M'}, 8}
    };

    string input = "!a+b*(a+b)+a+b!";
    input += '$';

    stack<int> stateStack;
    stack<char> symbolStack;
    stateStack.push(0);

    vector<int> productions;

    map<int, int> ruleLengths = {
        {1, 3}, {2, 1}, {3, 3}, {4, 1}, {5, 3}, {6, 1}, {7, 1}, {8, 3}
    };

    map<int, char> ruleNonTerminals = {
        {1, 'A'}, {2, 'B'}, {3, 'B'}, {4, 'T'}, {5, 'T'}, {6, 'M'}, {7, 'M'}, {8, 'M'}
    };

    try {
        int i = 0;

        while (i < input.size()) {
            int currentState = stateStack.top();
            char currentSymbol = input[i];

            if (currentSymbol != '!' && currentSymbol != '+' && currentSymbol != '*' &&
                currentSymbol != '(' && currentSymbol != ')' && currentSymbol != 'a' &&
                currentSymbol != 'b' && currentSymbol != '$') {
                throw runtime_error("Ошибка: недопустимый символ в входной строке: " + string(1, currentSymbol));
            }

            auto actionIt = actionTable.find({ currentState, currentSymbol });
            if (actionIt == actionTable.end()) {
                throw runtime_error("Ошибка: нет действия для состояния " + to_string(currentState) + " и символа " + currentSymbol);
            }
            string action = actionIt->second;

            if (action[0] == 's') {
                int newState = convertStringToInt(action.substr(1));
                stateStack.push(newState);
                symbolStack.push(currentSymbol);
                i++;
            }
            else if (action[0] == 'r') {
                int ruleNumber = convertStringToInt(action.substr(1));
                productions.push_back(ruleNumber);

                auto ruleLengthIt = ruleLengths.find(ruleNumber);
                if (ruleLengthIt == ruleLengths.end()) {
                    throw runtime_error("Ошибка: неизвестный номер правила " + to_string(ruleNumber));
                }
                int ruleLength = ruleLengthIt->second;

                auto nonTerminalIt = ruleNonTerminals.find(ruleNumber);
                if (nonTerminalIt == ruleNonTerminals.end()) {
                    throw runtime_error("Ошибка: неизвестный нетерминал для правила " + to_string(ruleNumber));
                }
                char nonTerminal = nonTerminalIt->second;

                reduce(stateStack, symbolStack, ruleLength, nonTerminal, gotoTable);
            }
            else if (action == "acc") {
                cout << "Разбор завершен успешно." << endl;
                break;
            }
            else {
                throw runtime_error("Ошибка: недопустимое действие в таблице action: " + action);
            }
        }

        if (i < input.size() - 1) {
            throw runtime_error("Ошибка: входная строка не была полностью обработана.");
        }

        cout << "Номера продукций обратного правого вывода: ";
        for (int prod : productions) {
            cout << prod << " ";
        }
        cout << endl;
    }
    catch (const exception& e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}