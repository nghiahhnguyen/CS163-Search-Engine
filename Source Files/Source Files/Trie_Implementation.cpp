#include "Function.h"

Word_t* Trie_t::insert(string word) {
	//Nghia
	//return true if the word is inserted
	//return false if the word already existed and is not inserted
	Word_t* cur = root;
	int n = word.size(), i, link_position;
	for (i = 0; i < n; ++i) {

		//if this the first time a word is insert into the Trie
		if (cur == root) {

			if (!root) root = new Word_t;

			//if the character is a digit
			if (isdigit(word[i]))
				link_position = word[i] - '0' + 26;
			else
				link_position = tolower(word[i]) - 'a';

			//mark that this character exist the Trie
			root->character_existed[link_position] = true;

			//if we are evaluating the last character of the word
			if (i == n - 1) {
				root->is_end = true;
				return root;
			}
			cur = root;
		}
		else {
			//the position of the new link added depending on whether the character is digit or not
			if (cur->link[link_position] == NULL) cur->link[link_position] = new Word_t;

			cur = cur->link[link_position];
			
			//if it's a digit
			if (isdigit(word[i])) 
				link_position = word[i] - '0' + 26;
			else 
				link_position = tolower(word[i]) - 'a';
			
			cur->character_existed[link_position] = true;

			//if it's the last character of the word
			if (i == n - 1) {
				cur->is_end = true;
				return cur;
			}
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
	int n = word.size(), i, link_position;
	for (i = 0; i < n; ++i) {

		//if there is no word in the Trie
		if (!root) {
			return NULL;
		}
		else {

			//if this is not the first character of the word
			if (cur != root) {

				if (cur->link[link_position] == NULL) return NULL;

				cur = cur->link[link_position];
			}

			//if it's a digit
			if (isdigit(word[i]))
				link_position = word[i] - '0' + 26;
			else
				link_position = tolower(word[i]) - 'a';

			if (!cur->character_existed[link_position]) return NULL;

			if (i == n - 1) {
				if (cur->is_end)
					return cur;
				else
					return NULL;
			}
		}
	}

	return NULL;
	
}


void Trie_t::inputFromFile(string folder_path) {
	//Nghia
	//Read all the find and put all the occurrence of words data into the Trie


	string file_path;


	//traverse the file
	for (int i = 1; i <= 100; ++i) {

		//get the name of file
		file_path = "Group07News" + itoXX(i);

		word_map word_occurrence, word_occurrence_title;
		word_map::iterator wmi;
		ifstream fin(folder_path);
		string word;

		if (fin.good()) {
			//clear the input buffer
			fin >> ws;
			string title;
			stringstream ss;
			getline(fin, title);
			ss = stringstream(title);

			//start reading the title
			while (ss.good()) {
				ss >> word;
				wmi = word_occurrence_title.find(word);

				//if the word doesn't exist then add it to the map
				if (wmi == word_occurrence_title.end())
					word_occurrence_title[word] = 1;
				//else just increment the counter of the word
				else
					++wmi->second;
			}

			//start reading the rest of the file
			while (fin.good()) {
				fin >> word;
				wmi = word_occurrence.find(word);


				//if the word doesn't exist then add it to the map
				if (wmi == word_occurrence.end())
					word_occurrence[word] = 1;
				//else just increment the counter of the word
				else
					++wmi->second;
			}
		}

		//pointer to get the position of the word in the Trie
		Word_t* word_in_trie;


		//traverse through all the word found in this file stored in the map
		for (wmi = word_occurrence.begin(); wmi != word_occurrence.end(); ++wmi) {
			word_in_trie = this->insert(wmi->first);
			Node tmp(wmi->second, i);
			word_in_trie->file_list.push_back(tmp);
			sort(word_in_trie->file_list.begin(), word_in_trie->file_list.end(), NodeMaxFirst);
		}
	}


}


