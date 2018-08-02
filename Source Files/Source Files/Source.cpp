#include "Function.h"
#include "FunctionTrang.h"

int main() {

	Trie_t stopWord, mainTree;

	mainTree.inputFromFile("CS163-Project-Data");
	mainTree.inputSynonymFromFile();

	char search[500];
	ResizeConsole(1220, 700);
	system("color 70");
	tc(112);
	LoadText();
	LoadBorder();
	readHistory();
	ready(search);

	string str(search);
	vector<Node> top5Ranking = mainTree.getQueryData("~come");
	vector<Node> come = mainTree.getKeywordData("come");
	vector<Node> arrive = mainTree.getKeywordData("arrive");
	vector<Node> nearr = mainTree.getKeywordData("near");
	vector<Node> advance = mainTree.getKeywordData("advance");

	system("pause");

	return 0;
}