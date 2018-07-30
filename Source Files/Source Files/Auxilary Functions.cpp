#include "Function.h"

string itoXX(int number) {
	//Nghia
	if (number < 10) return "0" + to_string(number);
	return to_string(number);
}


bool NodeMaxFirst(Node a, Node b) {
	return a.keyword_count > b.keyword_count;
}


int linkIndex(char x) {
	if (isalpha(x))
		return tolower(x) - 'a';
	else if (isdigit(x))
		return x - '0' + 26;
	else if (x == '$')
		return 36;
	//meaning x=='#'
	else
		return 37;
}

void operator+= (vector<Node> &v1, const vector<Node> &v2) {
	vector<Node>::const_iterator i;
	for (i = v2.begin(); i != v2.end(); ++i)
		v1.push_back(*i);
}

vector<Node> merge(const vector<Node> &v1, const vector<Node> &v2) {
	vector<Node> result;
	result += substract(v1, v2);
	result += substract(v2, v1);
	result += intersect(v1, v2);
	return result;
}

vector<Node> substract (const vector<Node> &v1, const vector<Node> &v2) {
	vector<Node>::const_iterator i, j;
	vector<Node> result;
	for (i = v1.begin(); i != v1.end(); ++i) {
		bool isInBothVector = false;
		for (j = v2.begin(); j != v2.end(); ++j)
			if ((*i).file_name == (*j).file_name) {
				isInBothVector = true;
				break;
			}
		if (!isInBothVector)
			result.push_back(*i);
	}
	return result;	
}

vector<Node> intersect(const vector<Node> &v1, const vector<Node> &v2) {
	vector<Node>::const_iterator i, j;
	vector<Node> result;
	for (i = v1.begin(); i != v1.end(); ++i) {
		int totalKeywordCount = (*i).keyword_count;
		for (j = v2.begin(); j != v2.end(); ++j)
			if ((*i).file_name == (*j).file_name)
				totalKeywordCount += (*j).keyword_count;
		result.push_back(Node(totalKeywordCount, (*i).file_name));
	}
	return result;
}

void splitString(string inputStr, vector<string> &tokens, vector<string> &exactMatch) {
	stringstream ss(inputStr);
	string tmp;
	vector<string> t;
	for (int i = 0; getline(ss, tmp, '\"'); ++i) {
		if (i % 2 != 0) {
			exactMatch.push_back(tmp);
		}
		else {
			stringstream sss(tmp);
			string temp;
			while (sss >> temp)
				tokens.push_back(temp);
		}
	}
}

bool exist(vector<Node> v, int file_name) {
	vector<Node>::iterator it;
	for (it = v.begin(); it != v.end(); ++it) {
		if ((*it).file_name == file_name)
			return true;
	}
	return false;
}

vector<Node> Trie_t::getData(string keyword) {
	// TO DO: wildcard character (*)
	if (keyword.size() >= 1 && (keyword[0] == '+' || keyword[0] == '-'))
		return search(keyword.substr(1, string::npos))->file_list;
	if (keyword.size() >= strlen("intitle:") && keyword.substr(0, 8) == "intitle:")
		return search(keyword.substr(8, string::npos))->title_list;
	/*if (keyword.size() >= 1 && keyword[0] == '~') {
		vector<Node> ans = search(keyword.substr(1, string::npos))->file_list;
		vector<string> syn = findSynonym(keyword.substr(1, string::npos));
		int n = syn.size();
		for (int i = 0; i < n; ++i)
			ans = merge(ans, getData(syn[i]));
		return ans;
	}*/
	return search(keyword)->file_list;
}


void preprocessing(string& word) {
	while (!isalnum(word[0]) && word[0]!='$' && word[0]!='#')
		word.erase(word.begin());
	while (!isalnum(*(word.rbegin())))
		word.pop_back();
}