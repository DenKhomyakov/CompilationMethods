#include "syntaxanalyzer.h"

#include "SyntaxAnalyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(const Grammar& grammar) : grammar(grammar) {}

vector<int> SyntaxAnalyzer::topDownAnalysis(const string& code) {
    index = 0;
    left = stack<Element>();
    right = stack<char>();
    right.push(grammar.startSymbol);
    status = NormalDirection;

    while (status == NormalDirection || status == ReverseDirection) {
        switch (status) {
        case NormalDirection:
            if (grammar.isNonTerminal(right.top())) {
                Element element(right.top(), 0);
                left.push(element);
                replace(right, element.symbol, grammar.getRule(element));
            }
            else if (index < code.length() && code[index] == right.top()) {
                left.push(Element(right.top()));
                right.pop();
                index++;

                if (index == code.length() && right.empty()) {
                    status = Completed;
                }
                else if (index == code.length() || right.empty()) {
                    status = ReverseDirection;
                }
            }
            else {
                status = ReverseDirection;
            }

            break;

        case ReverseDirection:
            if (index == 0 && right.top() == grammar.startSymbol) {
                return {};
            }

            Element element = left.top();
            left.pop();

            if (grammar.isNonTerminal(element.symbol)) {
                if (element.index + 1 < grammar.getRules(element.symbol).size()) {
                    left.push(Element(element.symbol, element.index + 1));
                    replace(right, grammar.getRules(element.symbol)[element.index], grammar.getRule(left.top()));
                    status = NormalDirection;
                }
                else {
                    replace(right, grammar.getRule(element), string(1, element.symbol));
                }
            }
            else {
                right.push(element.symbol);
                index--;
            }

            break;
        }
    }

    vector<int> result;

    while (!left.empty()) {
        Element elem = left.top();
        left.pop();
        if (elem.index >= 0) {
            result.push_back(grammar.getNumber(elem));
        }
    }

    reverse(result.begin(), result.end());
    return result;
}

bool SyntaxAnalyzer::replace(stack<char>& st, char oldChar, char newChar) {
    if (st.empty() || st.top() != oldChar) {
        return false;
    }

    st.pop();
    st.push(newChar);

    return true;
}

bool SyntaxAnalyzer::replace(stack<char>& st, char oldChar, const string& newChars) {
    if (st.top() != oldChar) {
        return false;
    }

    st.pop();

    for (int i = newChars.length() - 1; i >= 0; i--) {
        st.push(newChars[i]);
    }

    return true;
}

bool SyntaxAnalyzer::replace(stack<char>& st, const string& oldChars, const string& newChars) {
    if (st.size() < oldChars.length()) {
        return false;
    }

    stack<char> temp;

    for (char ch : oldChars) {
        if (st.empty() || st.top() != ch) {
            while (!temp.empty()) {
                st.push(temp.top());
                temp.pop();
            }

            return false;
        }

        temp.push(st.top());
        st.pop();
    }

    for (int i = newChars.length() - 1; i >= 0; i--) {
        st.push(newChars[i]);
    }

    return true;
}