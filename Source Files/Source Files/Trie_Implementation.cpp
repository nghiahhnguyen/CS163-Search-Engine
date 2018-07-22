#include "Function.h"


Trie_t::Trie_t() {
	root = new Word_t;
}


Word_t* Trie_t::insert(string word) {
	//Nghia
	//return true if the word is inserted
	//return false if the word already existed and is not inserted


	Word_t* cur = root;
	int n = word.size(), link_index;

	for (int i = 0; i < n; ++i) {

		//find the index of the next link
		link_index = linkIndex(word[i]);

		//if that link didnt' exist, create the link
		if (!cur->link[link_index])
			cur->link[link_index] = new Word_t;

		//move to the next node
		cur = cur->link[link_index];
	}
	//mark the end of the word
	cur->is_end = true;
	return cur;
}


Word_t* Trie_t::search(string word) {
	//Nghia
	//return the pointer to the word if it exists in the Trie
	//return NULL if the word doesn't exist in the Trie
	

	Word_t* cur = root;
	int n = word.size(), link_index;
	for (int i = 0; i < n; ++i) {

		//find the index of the next link
		link_index = linkIndex(word[i]);


		//if the word doesn't exist
		if (!cur->link[link_index])
			return NULL;

		//move to the next node
		cur = cur->link[link_index];
	}
	if (cur->is_end)
		return cur;
	else
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



void Trie_t::minus(string word_not_in_operator, string word_in_operator) {

}