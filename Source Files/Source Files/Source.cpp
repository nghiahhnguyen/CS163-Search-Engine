#include "Function.h"
#include "FunctionTrang.h"

int main() {

	Trie_t stopWord, mainTree;

	auto start = std::chrono::system_clock::now();
	
	mainTree.inputFromFile("CS163-Project-Data");
	mainTree.inputSynonymFromFile();

	auto end = std::chrono::system_clock::now();
	ofstream fout("Running time.txt");
	auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	fout << "The running time of the loading operation: " << elapsed.count() << " milliseconds\n";

	char search[500];
	ResizeConsole(1220, 700);
	system("color 70");
	tc(112);
	LoadText();
	LoadBorder();
	readHistory();
	ready(search);

	start = std::chrono::system_clock::now();
	string input(search);
	vector<Node> top5Ranking = mainTree.getQueryData(input);
	end = std::chrono::system_clock::now();
	elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	fout << "The running time of the searching operation: " << elapsed.count() << " milliseconds\n";

	system("pause");
	fout.close();
	return 0;
}