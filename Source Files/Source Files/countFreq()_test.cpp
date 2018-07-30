#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
using namespace std;

int countFreq(const string &pat, const string &txt) {
	stringstream ss(pat);
	string pat1;
	string pat2;
	getline(ss, pat1, '*');
	getline(ss, pat2);
	int M = pat1.length();
	int N = txt.length();
	int res = 0;

	/* A loop to slide pat[] one by one */
	for (int i = 0; i <= N - M; i++) {
		/* For current index i, check for
		pattern match */
		int j;
		for (j = 0; j < M; j++)
			if (txt[i + j] != pat1[j])
				break;

		// if pat[0...M-1] = txt[i, i+1, ...i+M-1]
		if (j == M) {
			int t = i + j + 1;
			while (t < N && txt[t] != ' ') t++;
			if (pat2.length() == 0) res++;
			else if (t < N && txt.substr(t, pat2.length()) == pat2) res++;
		}
	}
	return res;
}

int main() {
    string text("This is a very long string and a not long string");
    string pattern("a * long string");
    cout << countFreq(pattern, text);
    return 0;
}