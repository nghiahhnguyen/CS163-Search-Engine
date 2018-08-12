#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <sstream>
using namespace std;

int countFreq(const string &pat, const string &txt) {
	stringstream ss;
	if (pat.size() >= 1 && pat[0] == '*') ss << ' ';
	ss << pat;
	vector<string> patterns;
	string tmp;
	while (getline(ss, tmp, '*'))
		patterns.push_back(tmp);
	int n = txt.length();
	int m = patterns[0].length();
	int res = 0;

	/* A loop to slide pat[] one by one */
	for (int i = 0; i <= n - m; i++) {
		/* For current index i, check for
		pattern match */
		int j;
		for (j = 0; j < m; j++)
			if (txt[i + j] != patterns[0][j])
				break;
		// if patterns[0][0...m-1] = txt[i, i+1, ...i+m-1]
		if (j == m) {
			int t = i + j;
			bool found = true;
			for (int k = 1; k < patterns.size(); ++k) {
				while (t < n && txt[t] != ' ') ++t; // skip a word
				if (t == n || txt.substr(t, patterns[k].length()) != patterns[k]) {
					found = false;
					break;
				}
				++t;
			}
			if (found) ++res;
		}
	}
	return res;
}

void runTest(char* pattern, char* text, int expectedResult) {
	static int order = 0;
	string pat(pattern);
	string txt(text);
	if (countFreq(pat, txt) != expectedResult)
		cout << "Wrong answer on test " << order << " output is " << countFreq(pat, txt) << endl;
	++order;
}

int main() {
	runTest("sunday", "today is sunday", 1); // test 0
	runTest("monday", "monday tuesday monday wednesday monday", 3); // 1
	runTest("minh", "minhminhminhminh", 4); // 2
	runTest("vu", "vuhoangminhvu", 2); // 3
	runTest("* in the world", "ars is the best club in the world", 1); // 4
	runTest("* * something", "apple banana and something else", 1); // 5
	runTest("apples * bananas", "i like apples and bananas and c", 1); // 6
	runTest("test *", "i test p", 1); // 7
	runTest("is * * string", "this is a test string", 1); // 8
	runTest("the * in the world", "i am the tallest in the world", 1); // 9
	runTest("math *", "somtheing here math 1 and math 2", 2); // 10
	runTest("math *", "math 1 and math 2 endl", 2); // 11
	runTest("is * *", "this is a test string", 2); // 12
	runTest("today", "today is sunday", 1); // 13
	runTest("is", "today is sunday", 1); // 14
	runTest("are", "Today is sunday", 0); // 15
	runTest("minh", "vuhoang", 0); // 16
	runTest("vu hoang", "vu hoang minh", 1); // 17
	runTest("hoang minh", "vu hoang minh", 1); // 18
	runTest("arsenal", "vu haong minh", 0); // 19
	runTest("so * * now", "i am so sleepy right now", 1); // 20
	runTest("i * * * right", "i am so sleepy right now", 1); // 21
	runTest("i * right", "i am so sleepy right now", 0); // 22
	runTest("i * * * * now", "i am so sleepy right now", 1); //23
	system("pause");
	return 0;
}