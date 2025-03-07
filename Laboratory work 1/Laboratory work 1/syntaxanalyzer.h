#pragma once

#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include "Grammar.h"
#include <stack>
#include <vector>

using namespace std;

class SyntaxAnalyzer {
public:
    enum Status {
        Unstarted,
        NormalDirection,
        ReverseDirection,
        Completed
    };

    SyntaxAnalyzer(const Grammar& grammar);
    vector<int> topDownAnalysis(const string& code);

private:
    Grammar grammar;
    stack<Element> left;
    stack<char> right;
    int index;
    Status status;

    bool replace(stack<char>& st, char oldChar, char newChar);
    bool replace(stack<char>& st, char oldChar, const string& newChars);
    bool replace(stack<char>& st, const string& oldChars, const string& newChars);
};

#endif // SYNTAXANALYZER_H