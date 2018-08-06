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


void Trie_t::inputSynonymFromFile() {
	//Nghia
	//add at most 3 symnonyms(if exist) to vector<string>symnonyms
	ifstream fin("data\\Synonym.txt");
	string keyword, hyphen, syn, syn_list;
	stringstream ss;
	while (fin.good()) {
		fin >> keyword;
		Word_t* keyword_node = insert(keyword);;
		//eat the hyphen in the file
		fin >> hyphen;
		getline(fin, syn_list);
		ss = stringstream(syn_list);
		int it = 0;
		while (ss.good() && it++<3) {
			ss >> syn;
			//push the word into the vector
			keyword_node->synonyms.push_back(syn);
		}
		ss.clear();
	}
	fin.close();
}


void Trie_t::inputFromFile(const string& folder_path) {
	//Nghia
	//Read all the find and put all the occurrence of words data into the Trie
	
	//to read all the num
	string file_path;
	//traverse the file. Total files: 2268
	for (int i = 1; i <= 2268; ++i) {

		//get the name of file
		file_path = folder_path + "\\Group07News" + itoXX(i) +".txt";
		ifstream fin(file_path);

		if (fin.good()) cout << file_path << " is open\n";
		map<string, Article_t> article_word_count;
		map<string, Article_t>::iterator article_wmi;

		//the position of the paragraph (the title is the 1st paragraph)
		int para_position = 0;
		string paragraph, word;
		stringstream ss;
		
		//process each paragraph
		while (fin.good()) {
			//increment the count of paragraphs
			++para_position;
			//clear the input buffer
			fin >> ws;
			//read in the paragraph
			getline(fin, paragraph);
			ss = stringstream(paragraph);
			word_map para_word_count;
			word_map::iterator para_wmi;

			//start processing words
			while (ss.good()) {
				ss >> word;
				preprocessing(word);
				para_wmi = para_word_count.find(word);
				//if the word doesn't exist in the dictionary
				if (para_wmi == para_word_count.end()) {
					para_word_count[word] = 1;
				}
				else
					++para_wmi->second;
			}

			string keyword;
			//update the best paragraph and pass the information of the title(if this paragraph is the title)
			for (para_wmi = para_word_count.begin(); para_wmi != para_word_count.end(); ++para_wmi) {
				keyword = para_wmi->first;

				//find the keyword in the article dictionary
				article_wmi = article_word_count.find(keyword);

				//if the word doesn't exist
				if (article_wmi == article_word_count.end()) {
					//initialize the word count of the word
					article_word_count[keyword].word_count = para_wmi->second;
					//redirect the iterator to point to the word
					article_wmi = article_word_count.find(keyword);
					//initialize the info about the best paragraph
					article_wmi->second.best_para_posi = para_position;
					//initialize the word count of the best paragraph in the article
					article_wmi->second.best_para_word_count = para_wmi->second;
				}
				//otherwise
				else {
					//update the article word count of that word
					article_wmi->second.word_count += para_wmi->second;
					//if the word count of this word in the paragraph is better than the current best paragraph
					if (para_wmi->second > article_wmi->second.best_para_word_count) {
						//update the best paragraph position
						article_wmi->second.best_para_posi = para_position;
						//update the best paragraph word count
						article_wmi->second.best_para_word_count = para_wmi->second;
					}
				}

				//if the paragraph is the title
				if (para_position == 1) {
					article_wmi->second.is_intitle = true;
					article_wmi->second.word_count_title = para_wmi->second;
				}

			}
		}

		//pointer to get the position of the word in the Trie
		Word_t* word_in_trie;

		//traverse through all the word found in this article stored in the map
		for (article_wmi = article_word_count.begin(); article_wmi != article_word_count.end(); ++article_wmi) {
			word_in_trie = this->insert(article_wmi->first);

			//push the article info in the node in the Trie
			Node this_article(article_wmi->second.word_count, i, article_wmi->second.best_para_posi);
			word_in_trie->file_list.push_back(this_article);

			//if the word is in the article, pass that info to the Trie
			if (article_wmi->second.is_intitle) 
				word_in_trie->title_list.push_back(Node(article_wmi->second.word_count_title, i));

			//sort the file list
			sort(word_in_trie->file_list.begin(), word_in_trie->file_list.end(), NodeMaxFirst);
		}
		fin.close();
	}
}

void Trie_t::addAllNumbers(const string& word) {
	vector<ll>numbers_in_string = numbersInString(word);
	vector<ll>::iterator it;
	for (it=numbers_in_string.begin(); it!=numbers_in_string.end(); ++it)
		numbers.insert(*it);
}