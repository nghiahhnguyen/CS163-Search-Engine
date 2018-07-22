#include "Function.h"

string itoXX(int number) {
	//Nghia
	if (number < 10) return "0" + to_string(number);
	return to_string(number);
}


bool NodeMaxFirst(Node a, Node b) {
	return a.keyword_count > b.keyword_count;
}


int linkIndex(char x) {
	if (isalpha(x))
		return tolower(x) - 'a';
	else if (isdigit(x))
		return x - '0' + 26;
	else if (x == '$')
		return 26;
	//meaning x=='#'
	else
		return 27;
}