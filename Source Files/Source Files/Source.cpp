#include "Function.h"

int main() {
	Trie_t Trie;
	Trie.insert("Nghia");
	Trie.insert("Nghiasdfs");
	Trie.insert("Ngh");
	Trie.insert("Nghsdfs");
	Trie.search("Nghsdfs");
	return 0;
}