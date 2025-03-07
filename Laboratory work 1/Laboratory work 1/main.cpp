#include <iostream>
#include <map>
#include <string>
#include "grammar.h"
#include "syntaxanalyzer.h"

int main() {
    setlocale(LC_ALL, "Rus");

    Grammar grammar = {
        'A',
        { '!', '+', '*', '(', ')', 'a', 'b' },
        { 'A', 'B', 'T', 'M' },
        {
            { 'A', { "!B!" } },
            { 'B', { "T", "T+B" } },
            { 'T', { "M", "M*T" } },
            { 'M', { "a", "b", "(B)" } }
        }
    };

    SyntaxAnalyzer syntaxAnalyzer(grammar);

    map<string, string> tests = {
        { "!a+b!", "1 3 4 6 2 4 7" },
        { "!a*b!", "1 2 5 6 4 7" },
        { "!(a+b)*(b+a)!", "1 2 5 8 3 4 6 2 4 7 4 8 3 4 7 2 4 6" },
        { "!b*a+a*b!", "1 3 5 7 4 6 2 5 6 4 7" },
        { "!(a+b)*a+b*a!", "1 3 5 8 3 4 6 2 4 7 4 6 2 5 7 4 6" },
        { "!(a+b*a)*(b*b+a*(a+b+a))!", "1 2 5 8 3 4 6 2 5 7 4 6 4 8 3 5 7 4 7 2 5 6 4 8 3 4 6 3 4 7 2 4 6" },
        { "!a+*b!", "" },
        { "a+b*a+b", "" },
        { "a!b", "" },
        { "!a(b+a()!", "" }
    };

    for (const auto& test : tests) {
        vector<int> analysis = syntaxAnalyzer.topDownAnalysis(test.first);
        string result;

        for (int num : analysis) {
            result += to_string(num) + " ";
        }

        if (!result.empty()) {
            result.pop_back();
        }

        cout << "Пример: " << test.first << endl;
        cout << "Результат: " << (result.empty() ? "null" : result) << endl;
        cout << "Верный ответ: " << test.second << endl;
        cout << "Проверка: " << (result == test.second) << endl << endl;
    }

    return 0;
}