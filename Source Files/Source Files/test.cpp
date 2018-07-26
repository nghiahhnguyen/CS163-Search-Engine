#include <iostream>
#include <string>
#include <vector>
#include "Function.h"
using namespace std;

int main() {
    string a;
    getline(cin, a);
    vector<string> tokens, exactMatch;
    splitString(a, tokens, exactMatch);
    cout << "Tokens: ";
    for (int i = 0; i < tokens.size(); ++i)
        cout << tokens[i] << " ";
    cout << "\nExact Match Phrases: ";
    for (int i = 0; i < exactMatch.size(); ++i)
        cout << exactMatch[i] << " ";
    cout << endl;
    return 0;
}