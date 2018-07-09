#pragma once
#ifndef _FUNCTION_H_
#define _FUNCTION_H_
#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <fstream>
using namespace std;

#define ALP 26 //the number of words in the alphabet

//forward declaration
class Word_t;
class Trie_t;

//represent a file
struct Node {
	int keyword_count, file_name;
};

//represent a word in the Trie Tree
class Word_t {
public:
	friend Trie_t;
	void insert(Node file);
private:
	vector<Node>file_list;
	Word_t *link[ALP] = { NULL };
	bool is_end = false;
};

class Trie_t {
public:
	Trie_t();
	Word_t* insert(string word);
	Word_t* search(string word);
private:
	Word_t * root = NULL;
};

//TO DO: list of stopwords

#endif // !_FUNCTION_H_
