#include "Function.h"

Word_t* Trie_t::insert(string word) {
	//Nghia
	//return true if the word is inserted
	//return false if the word already existed and is not inserted
	Word_t* cur = root;
	int n = word.size(), i;
	for (i = 0; i < n; ++i) {

		//if this the first time a word is insert into the Trie
		if (!root) {
			root = new Word_t;

			int link_position;

			//if the character is a digit
			if (isdigit(word[i]))
				link_position = word[i] - '0' + 26;
			else
				link_position = tolower(word[i]) - 'a';

			root->link[link_position] = new Word_t;
			
			//if we are evaluating the last character of the word
			if (i == n - 1) {
				root->is_end = true;
				return root;
			}
			cur = root->link[link_position];
		}
		else {
			//the position of the new link added depending on whether the character is digit ornot
			int link_position;
			
			//if it's a digit
			if (isdigit(word[i])) 
				link_position = word[i] - '0' + 26;
			else 
				link_position = tolower(word[i]) - 'a';

			if (cur->link[link_position] == NULL) cur->link[link_position] = new Word_t;
			if (i == n - 1) {
				cur->is_end = true;
				return cur;
			}
			cur = cur->link[link_position];
		}
	}
	//the function should never reach this line
	//this is just to avoid a compilation error
	cerr<<"Sth is wrong with Trie_t::insert";
	return NULL;
}

Word_t* Trie_t::search(string word) {
	//Nghia
	//return the pointer to the word if it exists in the Trie
	//return NULL if the word doesn't exist in the Trie
	
	Word_t* cur = root;
	int n = word.size(), i;
	for (i = 0; i < n; ++i) {

		//if there is no word in the Trie
		if (!root) {
			return NULL;
		}
		else {
			//the position of the new link added depending on whether the character is a digit or not
			int link_position;

			//if it's a digit
			if (isdigit(word[i]))
				link_position = word[i] - '0' + 26;
			else
				link_position = tolower(word[i]) - 'a';

			if (cur->link[link_position] == NULL) return NULL;

			if (i == n - 1) {
				if (cur->is_end)
					return cur;
				else
					return NULL;
			}

			cur = cur->link[link_position];
		}
	}

	return NULL;
	
}