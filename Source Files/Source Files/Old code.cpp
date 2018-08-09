
//vector<int> Trie_t::minus(string word_not_in_operator, string word_in_operator) {
//vector<int> Trie_t::minus(string word_not_in_operator, string word_in_operator) {
//	//Nghia
//	//Perform the search on the word "word_not_in_operator" while excluding any result that has the "word_in_opertor" word
//	//return an empty vector if word_not_in_operator is not present. Otherwise return an unempty vector
//	//havent' been test
//
//	vector<int>results;
//	Word_t* search_word=search(word_not_in_operator), *ex_word=search(word_in_operator);
//
//	if (search_word) {
//		vector<Node> search_v = search_word->file_list, ex_v = ex_word->file_list;
//		int news_count = 0, m = search_v.size(), n = ex_v.size(), i = 0, j = 0;
//
//		while (news_count <= 5 && i < m) 
//			if (n == 0 || !exist(ex_v, search_v[i].file_name)) {
//				results.push_back(search_v[i].file_name);
//				++i;
//			}
//	}
//
//	return results;
//}

//void Trie_t::inputFromFile(string folder_path) {
//	//Nghia
//	//Read all the find and put all the occurrence of words data into the Trie
//
//
//	string file_path;
//
//
//	//traverse the file
//	for (int i = 1; i <= 100; ++i) {
//
//		//get the name of file
//		file_path = "Group07News" + itoXX(i);
//
//		word_map word_occurrence, word_occurrence_title;
//		word_map::iterator wmi;
//		ifstream fin(file_path);
//		string word;
//
//		if (fin.good()) {
//			//clear the input buffer
//			fin >> ws;
//			string title;
//			stringstream ss;
//			getline(fin, title);
//			ss = stringstream(title);
//
//
//			//start reading the title
//			while (ss.good()) {
//				ss >> word;
//				wmi = word_occurrence_title.find(word);
//
//				//if the word doesn't exist then add it to the map
//				if (wmi == word_occurrence_title.end()) {
//					word_occurrence_title[word] = 1;
//					word_occurrence[word] = 1;
//				}
//				//else just increment the counter of the word
//				else
//					++wmi->second;
//			}
//
//			//start reading the rest of the file
//			while (fin.good()) {
//				fin >> word;
//				wmi = word_occurrence.find(word);
//
//
//				//if the word doesn't exist then add it to the map
//				if (wmi == word_occurrence.end())
//					word_occurrence[word] = 1;
//				//else just increment the counter of the word
//				else
//					++wmi->second;
//			}
//		}
//
//		//pointer to get the position of the word in the Trie
//		Word_t* word_in_trie;
//
//
//		//traverse through all the word found in this file stored in the map
//		for (wmi = word_occurrence.begin(); wmi != word_occurrence.end(); ++wmi) {
//			word_in_trie = this->insert(wmi->first);
//			Node tmp(wmi->second, i);
//			word_in_trie->file_list.push_back(tmp);
//			sort(word_in_trie->file_list.begin(), word_in_trie->file_list.end(), NodeMaxFirst);
//		}
//		fin.close();
//	}
//
//
//}
