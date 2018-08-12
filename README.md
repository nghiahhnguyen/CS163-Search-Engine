# CS163-Search-Engine

This is the report for the final project in the course DATA STRUCTURE CS163. Our group consist of 4 people: Nguyen Ho Huu Nghia, Nguyen Ngoc Phuong Trang, Vu Hoang Minh, Nguyen Hoang Gia. Our task for the project is creating a search engine which supports the following operators besides the normal searching:

1. AND
2. OR
3. Manchester –united
4. intitle:hammer nails
5. Peanut Butter +and Jam
6. filetype:txt
7. Search for a price. Put $ in front of a number. For example: camera $400.
8. Search hashtags. Put # in front of a word. For example: #throwbackthursday
9. Search for an exact match. Put a word or phrase inside quotes. For example, "tallest building".
10. Search for wildcards or unknown words. Put a * in your word or phrase where you want to leave
a placeholder. For example, "largest * in the world"
11. Search within a range of numbers. Put .. between two numbers. For example, camera $50..$100.
12. Entering “~set” will bring back results that include words like “configure”, “collection” and “change”
which are all synonyms of “set”

In this report we are going to elaborate the process of creating this search engine. When we was first tasked with such a project, we were overwhelmed by the idea of the project alone. Therefore, it took us quite a lot of time to finally come up with the idea on how to implement this project. After that, we just went on to do the work until everything is done.

1. Design issues:
	The first issue with the project is that we have to find a way to store our data so that when we receive a query from the user, we can retrieve information about the best ranking documents. In order to deal with this, we agree with the idea that each word that appear in the data set will be stored as a node in a tree-like data structure. We construct the abstract data type for this node as a class named Word_t. Then in each node (which represent a word), we will store the following information in an std::vector named file_list.
	The index of the documents that the word appears in.
	The number of occurrence of that word in that document.
	After we found a way to store our information for future retrieval, we met with the problem of using the right tree-like data structure to store all our nodes. In the lectures of the courses, we were introduced to a wide range of search trees like BST, B-Tree, Red-Black Tree, AVL Tree. However, after some research about the running time, we decided to choose an unfamiliar data structure which is Trie, represented by the class named Trie_t. The reasoning behind our decision is going to be explained in the coming section.
		The abstraction behind this Trie is that for every word we found in the data set, we will insert it into the Trie (any duplicated word will not be added into the Trie).
		Next we are going to demonstrate our process of implementing the retrieval process and all the operators. Note that the result of the searching process will be an std::vector that consists of at most 5 elements representing the top 5 articles we found with the given query from the user.
		Firstly, in order to more complicated operations with our search engine, we had to decide how we will process a word to find the top 5 documents containing the word. Our process for a single word is quite basic to say the least. When we receive the word, we will use a member function of Trie_t named search(). Given a string, the function search() is going to return the node of type Word_t representing that word in the Trie. From that node, we will extract the index of the top 5 documents with the most occurrence of the word in the std::vector file_list. This is of course done only if the file_list is sorted in the decreasing order of the word occurrence count. This property is kept by sorting the file_list after the phase of indexing.
		Secondly we are going to discuss how we implement the AND, OR and ‘-’ operator. When the AND, OR or ‘-’ operator is present, we would extract the two words that is affected by the operator, let’s call them A and B. First we will use Trie_t::search() to get the two nodes A and B. 
	The AND operator is supposed to be the intersection of the two word so if one of the two words doesn’t exist, the AND operator is ignored. If both of the words exist, we would combine the std::vector file_list of both A and B into 1 std::vector. Here’s how we merge the two vectors for the AND operators: Only the documents that contains both of the words can be merged and stored in the returned vector (ie. both the vector of A and B contain the information about this document). The merging process for a qualified elements is simply the addition of the number of occurrence of each word in that document.
	The OR operator represents the union of the two words. Therefore, if one or neither of the word existed, it does not affect the OR operation. Hence, we are only going to consider the case where both of them existed. We would still combine the two vector file_list of A and B like in the AND operator but with a little twist. The document does not have to contain both of the word to be included in the returned vector. The merging process is then the addition of the number of occurrence of each word in every document containing them.
	The ‘-’ operator indicates that with a query such as “Manchester -united”, the result must be documents which contain the word “Manchester” and contain no “united”.  As a result, the returned vector simply contains documents that appear in file_list of A but not in file_list of B.
		The next operator we are going to take a look at is “intitle:”. This is actually a quite troublesome operator because with the present of this operator, we had to make a lot of modifications to our preprocessing and indexing phase. It is actually quite an interesting work, which we are going to present to you later in this report. In order to save the information about the title, we add an std::vector to each word that contains the occurrence of this word in the titlte of documents. It is basically a simple search in which only words in the title is considered.
		The next operator in the line is ‘+’ operator. However, for this operator to work properly, we first must take a look at another requirement of this project, that is stop words. 
	Stop words can be understood as a list of words that is excluded from the ranking process because they have no value in terms of searching. A few example of these words can be found here: https://www.ranks.nl/stopwords. In our structure, the stop words are stored in another Trie for fast searching in order to verify if a word is a stop word. For every word in the query entered by the user, we first check if the word is stop word or not. If it is indeed a stop word, we exclude that word from the ranking process.
	Only after it is clear what stop words are could we explain to you about the ‘+’ operator. If any stop words contain the ‘+’ opertator before it (like “+and”), its weight is still considered in the ranking process.
		The next two operation to take a look at is searching for price with ‘$’ and hashtag with ‘#’. We are not going to go into detail of these because they are simply treated as two simple characters in our engine.
		The wild card (‘*’) operator is among one of the hardest operators to implement. What we do is that with every word affected by the * operator, we find all the files that contain these words, extract the phrases in which all the words appear in the same order as in the query and use wildcard matching function to verify the phrases. ‘*’ can be a place holder for pretty much anything string including an empty one.
		Next, we are going to talk about the range of number operator “A..B”. Basically, this operator include every word in the range of from A to B. Our implementation include the use of std::set as a priority queue to keep all the numbers that appear in the data set sorted in an increasing order.
		Finally is the “~A” operation. This operators add all the symnonyms of A to the ranking process. We add this feature to the engine simply by adding a vector of every synonym of a word into its node. By doing so, every time the ~A is found, every synonym of A is treated the same as A.

2. Data structures:
	As said above, the data structure we use for storing the information about all the words that appeared on the data set is Trie. The reason we choose Trie over many others search trees namely B-Tree, Red Black Tree, AVL Tree etc. is that its time complexity for an inserting or searching operation of a word in O(n) with n being the length of the word. It is actually quite impressive compared to the time comlexities of inserting and searching for a word in Red Black Tree and AVL Tree, both of which are O(log(n)) with n being the length of the word.
	In our implementation, each node represents a character. A node also contain a boolean variable to mark if the character in that node is at the end of the word. From the two above properties, a word would be in the tree if there is path in the Trie so that the sequence of characters in the path is the same as the sequence of characters in the word.
	All the information of a word is stored in the node containing the last character of the word: 
	2 vectors named file_list and title_list (described above).
	An array of pointers pointing to the next node in the Trie.
3. Optimization issues:
	When building the Trie, we scan through all the files in our database to extract keywords and add data to the Trie. This procedure happens every time the program is executed and it is time-consuming. To optimize this process, after finishing the first running time, we should save the Trie into a file. Later, we can load the data from that file.
	Another problem is that if the user wants to add a new file into the database, we will have to rebuild the whole Trie. A way to solve this issue is that we can extract keywords from the new file and add new data into the existing Trie.
4. Scalability:
	Our implementation is efficient in the case of very large text collections.
	At the very beginning, our initialization is kinda slow because we have to put word-by-word into the Trie tree. But when the data is large enough, it will be completely fast. If the word is already in the tree, we will just need to push the file name and other information instead of checking whether it is exist or not and create a new node.
	Talking about memory consumption, it will take about a hundred MB for about 2000 text files. As long as the program is running, the memory consumption will take less and less. Because like mentioned above, words already exist so the program doesn't take any temporary memory.
