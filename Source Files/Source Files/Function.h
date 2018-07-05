#pragma once
#ifndef _FUNCTION_H_
#define _FUNCTION_H_
#include <string>
#include <vector>
#include <iostream>
using namespace std;

//represent a file
struct Node {
	int keyword_count, file_name;
};

//represent a word in the Trie Tree
class Word_t {
public:
private:
	vector<Node>file_list;
	Word_t *link[26];
};

struct Trie_t {
public:
private:
	Word_t * root;
};

//TO DO: list of stopwords

#endif // !_FUNCTION_H_
