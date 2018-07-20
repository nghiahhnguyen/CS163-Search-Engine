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
using namespace std;

typedef map<string, int> word_map;

#define ALP 36 //the number of words in the alphabet plus the 10 digits

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
	vector<Node>file_list;
	Word_t *link[ALP] = { NULL };
	bool character_existed[ALP]{ false };
	bool is_end = false;
};

class Trie_t {
public:
	Word_t* insert(string word);
	Word_t* search(string word);
	void inputFromFile(string folder_path);
private:
	Word_t * root = NULL;
};

//auxilary functions
string itoXX(int number);
bool NodeMaxFirst(Node a, Node b);

//TO DO: list of stopwords

#endif // !_FUNCTION_H_
