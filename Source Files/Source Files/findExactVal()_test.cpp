#include <iostream>
#include <cstring>
#include <string>
#include <vector>
#include <climits>
#include <algorithm>
using namespace std;

vector<string> findExactValue(string keyword, const vector<int> &exactVal) {
	// Minh
	// replace range in keyword by value in exactVal
	// assume there is at most 1 range in keyword
	// PreCondition: exactVal should be sorted 
	// if the startVal or endVal if empty it will assign INT_MIN or INT_MAX
	// eg. #worldcup..2018 == #worldcupINT_MIN..2018
	int dotPos = -1;
	int n = keyword.size();
	for (int i = 0; i < n - 1; ++i)
		if (keyword[i] == '.' && keyword[i + 1] == '.') {
			dotPos = i;
			break;
		}
	vector<string> stringsWithExactVal;
	if (dotPos == -1) return stringsWithExactVal;
	
	int startValPos = dotPos;
	while(startValPos >= 0 && keyword[startValPos - 1] >= '0' && keyword[startValPos - 1] <= '9')
		--startValPos;
	if (startValPos != 0 && keyword[startValPos - 1] == '-')
		--startValPos;
	int startVal;
	if (startValPos == dotPos)
		startVal = INT_MIN;
	else startVal = stoi(keyword.substr(startValPos, dotPos - startValPos));
	
	int endValPos = dotPos + 2;
	if (endValPos < n - 1 && keyword[endValPos] == '-' && keyword[endValPos + 1] >= '0' && keyword[endValPos + 1] <= '9')
		++endValPos;
	while (endValPos < n && keyword[endValPos] >= '0' && keyword[endValPos] <= '9')
		++endValPos;
	int endVal;
	if (endValPos == dotPos + 2)
		endVal = INT_MAX;
	else endVal = stoi(keyword.substr(dotPos + 2, endValPos - dotPos - 2));

	if (startVal > endVal) return stringsWithExactVal;
	
	vector<int>::const_iterator findStartIndex = upper_bound(exactVal.begin(), exactVal.end(), startVal);
	vector<int>::const_iterator findEndIndex = lower_bound(exactVal.begin(), exactVal.end(), endVal);
	if (findStartIndex != exactVal.begin() && *(findStartIndex - 1) == startVal)
		--findStartIndex;
	if (*findEndIndex == endVal) ++findEndIndex;
	for (vector<int>::const_iterator i = findStartIndex; i != findEndIndex; ++i)
		stringsWithExactVal.push_back(keyword.substr(0, startValPos) + to_string(*i) + keyword.substr(endValPos, string::npos));
	return stringsWithExactVal;
}

int main() {
    vector<int> val = { -5, -2 , -1 ,0, 11, 70 };
	vector<int> rangePair;
	for (int i = 0; i < val.size(); ++i) {
		rangePair.push_back(val[i] - 1);
		rangePair.push_back(val[i]);
		rangePair.push_back(val[i] + 1);
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
			cout << "Convert: " << range << endl;
			vector<string> f = findExactValue(range, val);
			for (int t = 0; t < f.size(); ++t)
				cout << f[t] << endl;
			cout << endl;
		}

    return 0;
}