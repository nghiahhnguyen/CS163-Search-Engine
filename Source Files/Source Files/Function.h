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

typedef pair<int, int> ii;
typedef long long ll;
typedef map<string, int> word_map;

#define mp make_pair
#define ALP 38 //the number of words in the alphabet + 10 digits + '$' + '#'

//forward declaration
class Word_t;
class Trie_t;
class Word_t;
struct Article_t;

//represent a file
struct Node {
	int keyword_count, file_name, best_paragraph;
	Node(int x, int y, int z)
		:keyword_count(x), file_name(y), best_paragraph(z) {};
	Node(int x, int y)
		:keyword_count(x), file_name(y) {};
};

//represent a word in the Trie Tree
class Word_t {
public:
	friend Trie_t;
private:
	vector<Node>file_list, title_list;
	vector<string>synonyms;
	Word_t *link[ALP] = { NULL };
	bool is_end = false;
};

struct Article_t {
	int word_count, best_para_posi, best_para_word_count, word_count_title;
	bool is_intitle = false;
};

struct Para_t {
	int word_count, word_count_title;
	bool is_intitle = false;
};

class Trie_t {
public:
	Trie_t();
	Word_t* insert(string word);
	Word_t* search(string word);
	//read the data into the Trie
	void inputFromFile(const string& folder_path);
	//add at most 3 symnonyms(if exist) to vector<string>symnonyms
	void inputSynonymFromFile();
	// return data of a keyword
	vector<Node> getData(string keyword);
	// return if s is a stopword or not
	bool isStopWord(string s);
private:
	Word_t * root = NULL;
	vector<ll>numbers;
};

//auxilary functions
string itoXX(int number);
bool NodeMaxFirst(Node a, Node b);
int linkIndex(char x);

// vector functions
void operator+= (vector<Node> &v1, const vector<Node> &v2); // add two vectors
vector<Node> merge(const vector<Node> &v1, const vector<Node> &v2); // return nodes in v1 or v2
vector<Node> substract (const vector<Node> &v1, const vector<Node> &v2); // return nodes in v1 but not in v2
vector<Node> intersect(const vector<Node> &v1, const vector<Node> &v2); // return nodes in both v1 and v2

// string functions
void splitString(string inputStr, vector<string> &tokens, vector<string> &exactMatch); // split intputStr into tokens
bool exist(vector<Node> v, int file_name);
void preprocessing(string& word);

#endif // !_FUNCTION_H_
