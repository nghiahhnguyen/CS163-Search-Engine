#pragma once
#ifndef _FUNCTION_H_
#define _FUNCTION_H_
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <fstream>
#include <map>
#include <sstream>
using namespace std;

typedef map<string, int> word_map;

#define ALP 38 //the number of words in the alphabet + 10 digits + '$' + '#'

//forward declaration
class Word_t;
class Trie_t;

//represent a file
struct Node {
	int keyword_count, file_name;
	Node(int x, int y)
		:keyword_count(x), file_name(y) {};
};

//represent a word in the Trie Tree
class Word_t {
public:
	friend Trie_t;
private:
	vector<Node>file_list, title_list;
	Word_t *link[ALP] = { NULL };
	bool is_end = false;
};

class Trie_t {
public:
	Trie_t();
	Word_t* insert(string word);
	Word_t* search(string word);
	void inputFromFile(string folder_path);
<<<<<<< HEAD
	void minus(string word_not_in_operator, string word_in_operator);
	
	// return data of a keyword
	vector<Node> getData(string keyword);
	// return if s is a stopword or not
	bool isStopWord(string s);
=======
	vector<int> minus(string word_not_in_operator, string word_in_operator);
>>>>>>> c2fd3ced93d6e42e6f99a8dc47980604c4bfe2e2
private:
	Word_t * root = NULL;
};

//auxilary functions
string itoXX(int number);
bool NodeMaxFirst(Node a, Node b);
int linkIndex(char x);

<<<<<<< HEAD
// vector functions
void operator+= (vector<Node> &v1, const vector<Node> &v2); // add two vectors
vector<Node> merge(const vector<Node> &v1, const vector<Node> &v2); // return nodes in v1 or v2
vector<Node> substract (const vector<Node> &v1, const vector<Node> &v2); // return nodes in v1 but not in v2
vector<Node> intersect(const vector<Node> &v1, const vector<Node> &v2); // return nodes in both v1 and v2

// string functions
void splitString(string inputStr, vector<string> &tokens, vector<string> &exactMatch); // split intputStr into tokens
vector<string> synonyms(string word); // return all synonyms of word

=======
bool exist(vector<Node> v, int file_name) {
	vector<Node>::iterator it;
	for (it = v.begin(); it != v.end(); ++it) {
		if ((*it).file_name == file_name)
			return true;
	}
	return false;
}
>>>>>>> c2fd3ced93d6e42e6f99a8dc47980604c4bfe2e2

#endif // !_FUNCTION_H_
