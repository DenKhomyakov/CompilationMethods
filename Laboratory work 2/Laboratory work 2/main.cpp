#include <iostream>
#include <stack>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

// Функция для преобразования строки в целое число
int string_to_int(const string& str) {
    int result;
    istringstream iss(str);
    iss >> result;
    return result;
}

// Определение типов для таблиц action и goto
using ActionTable = map<pair<int, char>, string>;
using GotoTable = map<pair<int, char>, int>;

// Функция для выполнения свертки
void reduce(stack<int>& stateStack, stack<char>& symbolStack, int ruleLength, char nonTerminal, const GotoTable& gotoTable) {
    for (int i = 0; i < ruleLength; ++i) {
        stateStack.pop();
        symbolStack.pop();
    }
    symbolStack.push(nonTerminal);
    int newState = gotoTable.at({ stateStack.top(), nonTerminal });
    stateStack.push(newState);
}

int main() {
    setlocale(LC_ALL, "Rus");

    // Инициализация таблиц action и goto
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

    // Входная строка
    string input = "!a+b!";
    input += '$';  // Добавляем маркер конца строки

    // Стек состояний и стек символов
    stack<int> stateStack;
    stack<char> symbolStack;
    stateStack.push(0);

    // Вектор для хранения номеров продукций
    vector<int> productions;

    int i = 0;
    while (i < input.size()) {
        int currentState = stateStack.top();
        char currentSymbol = input[i];

        // Поиск действия в таблице action
        auto actionIt = actionTable.find({ currentState, currentSymbol });
        if (actionIt == actionTable.end()) {
            cout << "Ошибка: нет действия для состояния " << currentState << " и символа " << currentSymbol << endl;
            return 1;
        }

        string action = actionIt->second;

        if (action[0] == 's') {
            // Перенос
            int newState = string_to_int(action.substr(1)); // Используем string_to_int вместо stoi
            stateStack.push(newState);
            symbolStack.push(currentSymbol);
            i++;
        }
        else if (action[0] == 'r') {
            // Свертка
            int ruleNumber = string_to_int(action.substr(1)); // Используем string_to_int вместо stoi
            productions.push_back(ruleNumber);

            // Определение длины правой части правила
            int ruleLength = 0;
            switch (ruleNumber) {
            case 1: ruleLength = 3; break;
            case 2: ruleLength = 1; break;
            case 3: ruleLength = 3; break;
            case 4: ruleLength = 1; break;
            case 5: ruleLength = 3; break;
            case 6: ruleLength = 1; break;
            case 7: ruleLength = 1; break;
            case 8: ruleLength = 3; break;
            }

            // Определение нетерминала для свертки
            char nonTerminal = '\0';
            switch (ruleNumber) {
            case 1: nonTerminal = 'A'; break;
            case 2: case 3: nonTerminal = 'B'; break;
            case 4: case 5: nonTerminal = 'T'; break;
            case 6: case 7: case 8: nonTerminal = 'M'; break;
            }

            reduce(stateStack, symbolStack, ruleLength, nonTerminal, gotoTable);
        }
        else if (action == "acc") {
            // Допуск
            cout << "Разбор завершен успешно." << endl;
            break;
        }
        else {
            // Ошибка
            cout << "Ошибка: недопустимый символ или состояние." << endl;
            return 1;
        }
    }

    // Вывод номеров продукций
    cout << "Номера продукций обратного правого вывода: ";
    for (int prod : productions) {
        cout << prod << " ";
    }
    cout << endl;

    return 0;
}