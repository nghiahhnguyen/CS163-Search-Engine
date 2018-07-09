#include "Function.h"

Trie_t::Trie_t() {

}

Word_t* Trie_t::insert(string word) {
	//Nghia
	//return true if the word is inserted
	//return false if the word already existed and is not inserted
	Word_t* cur = root;
	int n = word.size(), i;
	for (i = 0; i < n; ++i) {
		if (!root) {
			root = new Word_t;
			root->link[word[i] - 'a'] = new Word_t;
			if (i == n - 1) {
				root->is_end = true;
				return root;
			}
			cur = root->link[word[i] - 'a'];
		}
		else {
			if (cur->link[word[i] - 'a'] == NULL) cur->link[word[i] - 'a'] = new Word_t;
			if (i == n - 1) {
				cur->is_end = true;
				return cur;
			}
			cur = cur->link[word[i] - 'a'];
		}
	}
	//the function never reach this line
	cerr<<"Sth is wrong with Trie_t::insert";
	return NULL;
}

Word_t* Trie_t::search(string word) {
	Word_t* cur = root;
	int n = word.size(), i;
	for (i = 0; i < n; ++i) {
		if (i == n - 1) {
			if (cur->is_end) return cur;
			else break;
		}
		if (cur->link[word[i] - 'a'] == NULL) cur->link[word[i] - 'a'] = new Word_t;
		cur = cur->link[word[i] - 'a'];
	}
	return NULL;
}