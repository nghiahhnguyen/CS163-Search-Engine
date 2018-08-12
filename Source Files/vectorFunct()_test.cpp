#include <iostream>
#include <vector>
#include <string>
using namespace std;

struct Node {
	int keyword_count, file_name, best_paragraph;
	Node(int x, int y, int z)
		:keyword_count(x), file_name(y), best_paragraph(z) {};
	Node(int x, int y)
		:keyword_count(x), file_name(y) {};
};

void operator+= (vector<Node> &v1, const vector<Node> &v2);
vector<Node> merge(const vector<Node> &v1, const vector<Node> &v2, int m = 1);
vector<Node> substract (const vector<Node> &v1, const vector<Node> &v2);
vector<Node> intersect(const vector<Node> &v1, const vector<Node> &v2);
void printOut(const vector<Node> &v);


void operator+= (vector<Node> &v1, const vector<Node> &v2) {
	vector<Node>::const_iterator i;
	for (i = v2.begin(); i != v2.end(); ++i)
		v1.push_back(*i);
}

vector<Node> merge(const vector<Node> &v1, const vector<Node> &v2, int m) {
	vector<Node> result;
	result += intersect(v1, v2);
	// increase the score if Node is both in v1 and v2
	for (vector<Node>::iterator i = result.begin(); i != result.end(); ++i)
		i->keyword_count *= m;
	result += substract(v1, v2);
	result += substract(v2, v1);
	return result;
}

vector<Node> substract (const vector<Node> &v1, const vector<Node> &v2) {
	vector<Node>::const_iterator i, j;
	vector<Node> result;
	for (i = v1.begin(); i != v1.end(); ++i) {
		bool isInBothVector = false;
		for (j = v2.begin(); j != v2.end(); ++j)
			if ((*i).file_name == (*j).file_name) {
				isInBothVector = true;
				break;
			}
		if (!isInBothVector)
			result.push_back(*i);
	}
	return result;	
}

vector<Node> intersect(const vector<Node> &v1, const vector<Node> &v2) {
	vector<Node>::const_iterator i, j;
	vector<Node> result;
	for (i = v1.begin(); i != v1.end(); ++i) {
		int totalKeywordCount = (*i).keyword_count;
        bool isInBothVector = false;
		for (j = v2.begin(); j != v2.end(); ++j)
			if ((*i).file_name == (*j).file_name) {
                isInBothVector = true;
				totalKeywordCount += (*j).keyword_count;
            }
		if (isInBothVector) result.push_back(Node(totalKeywordCount, (*i).file_name));
	}
	return result;
}

void printOut(const vector<Node> &v) {
    for (vector<Node>::const_iterator i = v.begin(); i != v.end(); ++i) {
        cout << i->keyword_count << " " << i->file_name << endl;
    }
}

int main() {
    vector<Node> v1;
    vector<Node> v2;
    v1.push_back(Node(1, 2));
    v1.push_back(Node(2, 3));
    v2.push_back(Node(4, 3));
    v2.push_back(Node(5, 5));
    printOut(intersect(v1, v2));
    cout << endl;
    printOut(merge(v1, v2, 2));
    cout << endl;
    printOut(substract( v1, v2));
    cout << endl;
    printOut(substract(v2, v1));
    return 0;
}