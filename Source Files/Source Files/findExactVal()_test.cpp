#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
#include <set>
using namespace std;

vector<string> findExactValue(string keyword, const set<long long> &exactVal) {
	// Minh
	// replace range in keyword by value in exactVal
	// assume there is at most 1 range in keyword
	// PreCondition: exactVal should be sorted 
	// if the startVal or endVal if empty it will assign INT_MIN or INT_MAX
	// eg. #worldcup..2018 == #worldcupINT_MIN..2018
	size_t dotPos = keyword.find("..");
	vector<string> stringsWithExactVal;
	if (dotPos == string::npos) return stringsWithExactVal;
	size_t startValPos = dotPos;
	while (startValPos >= 0 && keyword[startValPos - 1] >= '0' && keyword[startValPos - 1] <= '9')
		--startValPos;
	if (startValPos != 0 && keyword[startValPos - 1] == '-')
		--startValPos;
	long long startVal;
	if (startValPos == dotPos)
		startVal = INT_MIN;
	else startVal = stoi(keyword.substr(startValPos, dotPos - startValPos));

	size_t endValPos = dotPos + 2;
	size_t n = keyword.size();
	if (endValPos < n - 1 && keyword[endValPos] == '-' && keyword[endValPos + 1] >= '0' && keyword[endValPos + 1] <= '9')
		++endValPos;
	while (endValPos < n && keyword[endValPos] >= '0' && keyword[endValPos] <= '9')
		++endValPos;
	long long endVal;
	if (endValPos == dotPos + 2)
		endVal = INT_MAX;
	else endVal = stoi(keyword.substr(dotPos + 2, endValPos - dotPos - 2));

	if (startVal > endVal) return stringsWithExactVal;

	set<long long>::const_iterator findStartIndex = upper_bound(exactVal.begin(), exactVal.end(), startVal);
	set<long long>::const_iterator findEndIndex = lower_bound(exactVal.begin(), exactVal.end(), endVal);
	set<long long>::const_iterator tmp = findStartIndex;
	--tmp;
	if (findStartIndex != exactVal.begin() && *(tmp) == startVal)
		--findStartIndex;
	if (*findEndIndex == endVal) ++findEndIndex;
	for (set<long long>::const_iterator i = findStartIndex; i != findEndIndex; ++i)
		stringsWithExactVal.push_back(keyword.substr(0, startValPos) + to_string(*i) + keyword.substr(endValPos, string::npos));
	return stringsWithExactVal;
}

int main() {
    vector<long long> v = { -5, -2 , -1 ,0, 11, 70 };
	set<long long> val(v.begin(), v.end());
	vector<long long> rangePair;
	for (int i = 0; i < v.size(); ++i) {
		rangePair.push_back(v[i] - 1);
		rangePair.push_back(v[i]);
		rangePair.push_back(v[i] + 1);
	}
	for (int i = 0; i < rangePair.size(); ++i)
		for (int j = 0; j < rangePair.size(); ++j) {
			// form a range string;
			string doubleDot("..");
			string range("Begin");
			range += to_string(rangePair[i]);
			range += doubleDot;
			range += to_string(rangePair[j]);
			range += "End";
			cout << "Original: " << range << endl;
			cout <<"Result:\n";
			vector<string> f = findExactValue(range, val);
			for (int t = 0; t < f.size(); ++t)
				cout << f[t] << endl;
			cout << endl;
		}

    return 0;
}