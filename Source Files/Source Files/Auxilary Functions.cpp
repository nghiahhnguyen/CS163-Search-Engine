#include "Function.h"
#include "GlobalVariables.h"
const int BONUSPOINT = 500;

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
		return 36;
	else if (x == '#')
		return 37;
	// meaning x is "  '  "
	else
		return 38;
}

void operator+= (vector<Node> &v1, const vector<Node> &v2) {
	vector<Node>::const_iterator i;
	for (i = v2.begin(); i != v2.end(); ++i)
		v1.push_back(*i);
}

vector<Node> merge(const vector<Node> &v1, const vector<Node> &v2, int bonusPoint) {
	vector<Node> result;
	result += intersect(v1, v2);
	// increase the score if Node is both in v1 and v2
	for (size_t i = 0; i < result.size(); ++i)
		result[i].keyword_count += bonusPoint;
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
	size_t i, j;
	vector<Node> result;
	for (i = 0; i < v1.size(); ++i) {
		int totalKeywordCount = v1[i].keyword_count;
        bool isInBothVector = false;
		for (j = 0; j < v2.size(); ++j)
			if (v1[i].file_name == v2[j].file_name) {
                isInBothVector = true;
				totalKeywordCount += v2[j].keyword_count;
				break;
            }
		if (isInBothVector)
			result.push_back(Node(totalKeywordCount, v1[i].file_name, (v1[i].keyword_count > v2[j].keyword_count) ? v1[i].best_paragraph : v2[j].best_paragraph));
	}
	return result;
}

vector<string> splitString(string inputStr) {
	stringstream ss(inputStr);
	string tmp;
	vector<string> result;
	for (int i = 0; getline(ss, tmp, '\"'); ++i) {
		if (i % 2 != 0) {
			result.push_back(tmp);
		}
		else {
			stringstream sss(tmp);
			string tmp2;
			while (sss >> tmp2)
				result.push_back(tmp2);
		}
	}
	return result;
}

bool exist(vector<Node> v, int file_name) {
	vector<Node>::iterator it;
	for (it = v.begin(); it != v.end(); ++it) {
		if ((*it).file_name == file_name)
			return true;
	}
	return false;
}

vector<Node> Trie_t::getKeywordData(string keyword, set<string> &highlightWords) {
	vector<Node> emptyVector; // empty vector to return in the case there is no data
	if (keyword.find(" ") == string::npos) {
		// if it is a token
		if (keyword.size() >= 1 && (keyword[0] == '+' || keyword[0] == '-')) {
			string sTmp = keyword.substr(1, string::npos);
			Word_t* m = search(sTmp);
			if (!m) return emptyVector;
			if (keyword[0] != '-') highlightWords.insert(sTmp);
			return m->file_list;
		}
		if (keyword.size() >= strlen("intitle:") && keyword.substr(0, 8) == "intitle:") {
			string sTmp = keyword.substr(8, string::npos);
			Word_t* m = search(sTmp);
			if (!m) return emptyVector;
			highlightWords.insert(sTmp);
			return m->title_list;
		}
		if (keyword.size() >= 1 && keyword[0] == '~') {
			vector<string> syn;
			vector<Node> ans;
			Word_t* m = search(keyword.substr(1, string::npos));
			if (m) syn = m->synonyms;
			syn.push_back(keyword.substr(1, string::npos));
			int n = syn.size();
			for (int i = 0; i < n; ++i)
				ans = merge(ans, getKeywordData(syn[i], highlightWords));
			return ans;
		}
		if (keyword.find("..") != string::npos) {
			vector<string> f = findExactValue(keyword, numbers);
			vector<Node> result;
			int n = f.size();
			for (int i = 0; i < n; i++)
				result = merge(result, getKeywordData(f[i], highlightWords));
			return result;
		}
		Word_t* m = search(keyword);
		if (!m) return emptyVector;
		highlightWords.insert(keyword);
		return m->file_list;
	}
	// if it is an exact match phase
	vector<string> splitIntoTokens = splitString(keyword);
	vector<string> kw;
	for (vector<string>::iterator i = splitIntoTokens.begin(); i != splitIntoTokens.end(); ++i)
		if ((*i) != "*") kw.push_back(*i);
	vector<Node> result;
	if (kw.size() == 0) return result;
	set<string> tmpArrayForFunctCall;
	result = getKeywordData(kw[0], tmpArrayForFunctCall);
	for (int i = 1; i < kw.size(); ++i)
			result = intersect(result, getKeywordData(kw[i], tmpArrayForFunctCall));
	// open file to recalculate word_count
	ifstream fin;
	for (int i = 0; i < result.size(); ++i) {
		result[i].keyword_count = 0;
		fin.open(indexToFilename(result[i].file_name).c_str());
		if (!fin.is_open()) continue;
		// TODO: count apperance in file
		string text;
		while (getline(fin, text)) {
			result[i].keyword_count += countFreq(keyword, text, highlightWords);
		}
		fin.close();
	}
	// remove files with 0 word_count
	vector<Node> resultAfterRemove0WordCount;
	for (vector<Node>::iterator i = result.begin(); i != result.end(); ++i)
		if (i->keyword_count != 0)
			resultAfterRemove0WordCount.push_back(*i);
	return resultAfterRemove0WordCount;
}

vector<string> findExactValue(string keyword, const set<long long> &exactVal) {
	// Minh
	// replace range in keyword by value in exactVal
	// assume there is at most 1 range in keyword
	// PreCondition: exactVal should be sorted 
	// if the startVal or endVal if empty it will assign INT_MIN or INT_MAX
	// eg. #worldcup..2018 == #worldcupINT_MIN..2018
	size_t dotPos = keyword.find("..");
	vector<string> stringsWithExactVal;
	if (dotPos == string::npos) return stringsWithExactVal;
	size_t startValPos = dotPos;
	while (startValPos > 0 && keyword[startValPos - 1] >= '0' && keyword[startValPos - 1] <= '9')
		--startValPos;
	if (startValPos > 0 && keyword[startValPos - 1] == '-')
		--startValPos;
	long long startVal;
	if (startValPos == dotPos)
		startVal = INT_MIN;
	else startVal = stoll(keyword.substr(startValPos, dotPos - startValPos));

	size_t endValPos = dotPos + 2;
	size_t n = keyword.size();
	if (endValPos < n - 1 && keyword[endValPos] == '-' && keyword[endValPos + 1] >= '0' && keyword[endValPos + 1] <= '9')
		++endValPos;
	while (endValPos < n && keyword[endValPos] >= '0' && keyword[endValPos] <= '9')
		++endValPos;
	long long endVal;
	if (endValPos == dotPos + 2)
		endVal = INT_MAX;
	else endVal = stoll(keyword.substr(dotPos + 2, endValPos - dotPos - 2));

	if (startVal > endVal) return stringsWithExactVal;

	set<long long>::const_iterator findStartIndex = upper_bound(exactVal.begin(), exactVal.end(), startVal);
	set<long long>::const_iterator findEndIndex = lower_bound(exactVal.begin(), exactVal.end(), endVal);
	set<long long>::const_iterator tmp = findStartIndex;
	if (findStartIndex != exactVal.begin()) --tmp;
	if (findStartIndex != exactVal.begin() && *(tmp) == startVal)
		--findStartIndex;
	if (findEndIndex != exactVal.end() && *findEndIndex == endVal) ++findEndIndex;
	for (set<long long>::const_iterator i = findStartIndex; i != findEndIndex; ++i)
		stringsWithExactVal.push_back(keyword.substr(0, startValPos) + to_string(*i) + keyword.substr(endValPos, string::npos));
	return stringsWithExactVal;
}

string getFileName(int fileName) {
	string name("CS163-Project-Data\\Group07News");
	name += itoXX(fileName) + ".txt";
	return name;
}

void preprocessing(string& word, set<ll>& numbers_in_word) {
	string::reverse_iterator it;
	string result = "";
	ll cur_number = 0;
	int num_digit = 0;
	bool last_char_is_digit = false;
	for (it = word.rbegin(); it != word.rend(); ++it) {
		if (*it >= 32 && *it <= 126 && (isalnum(*it) || *it == '$' || *it == '#' || *it == '\'')) {
			result = string(1, tolower(*it)) + result;
			if (isdigit(*it)) {
				if (!last_char_is_digit) {
					cur_number = *it - '0';
					num_digit = 1;
					last_char_is_digit = true;
				}
				else
					cur_number += (*it - '0')*int(pow(10, num_digit++));
			}
			else {
				if (last_char_is_digit) {
					numbers_in_word.insert(cur_number);
					cur_number = 0;
					num_digit = 0;
				}
				last_char_is_digit = false;
			}
		}
	}
	if (cur_number != 0) numbers_in_word.insert(cur_number);
	word = result;
}

void preprocessing(string& word){
	string::iterator it;
	string result;
	for (it = word.begin(); it != word.end(); ++it) {
		if (*it >= 32 && *it <= 126 && (isalnum(*it) || *it == '$' || *it == '#' || *it == '\'')) {
			result.push_back(tolower(*it));
		}
	}
	word = result;
}

vector<Node> Trie_t::getQueryData(string quiery, set<string> &highlightWords) {
	vector<string> tokens = splitString(quiery);
	int n = tokens.size();
	vector<Node> result;
	bool pendingAndOperator = false;
	vector<vector<Node>> pendingMinusOperator;
	for (int i = 0; i < n; ++i) {
		if (checkStopWordUsingTrie_t(tokens[i])) continue;
		if (pendingAndOperator) {
			pendingAndOperator = false;
			result = merge(result, merge(getKeywordData(tokens[i - 2], highlightWords), getKeywordData(tokens[i], highlightWords)), BONUSPOINT);
			continue;
		}
		if (i < n - 1 && strcmp(tokens[i + 1].c_str(), "AND") == 0) {
			pendingAndOperator = true;
			continue;
		}
		if (tokens[i].size() >= 1 && tokens[i][0] == '-') {
			pendingMinusOperator.push_back(getKeywordData(tokens[i].substr(1,string::npos), highlightWords));
			continue;
		}
		if (tokens[i].size() >= 1 && tokens[i][0] == '+') {
			result = merge(result, getKeywordData(tokens[i].substr(1, string::npos), highlightWords), BONUSPOINT);
			continue;
		}
		result = merge(result, getKeywordData(tokens[i], highlightWords), BONUSPOINT);
	}
	for (vector<vector<Node>>::iterator i = pendingMinusOperator.begin(); i != pendingMinusOperator.end(); ++i)
		result = substract(result, *i);
	sort(result.begin(), result.end(), NodeMaxFirst);
	return result;
}

int countFreq(const string &pat, const string &txt, set<string> &highlightWords) {
	// count number of occurence of pat in txt
	// pat might contain wildcard char 
	stringstream ss;
	if (pat.size() >= 1 && pat[0] == '*') ss << ' ';
	ss << pat;
	vector<string> patterns;
	string tmp;
	while (getline(ss, tmp, '*'))
		patterns.push_back(tmp);
	int n = txt.length();
	int m = patterns[0].length();
	int res = 0;

	/* A loop to slide pat[] one by one */
	for (int i = 0; i <= n - m; i++) {
		/* For current index i, check for
		pattern match */
		int j;
		for (j = 0; j < m; j++)
			if (txt[i + j] != patterns[0][j])
				break;
		// if patterns[0][0...m-1] = txt[i, i+1, ...i+m-1]
		if (j == m) {
			int t = i + j;
			bool found = true;
			for (int k = 1; k < patterns.size(); ++k) {
				while (t < n && txt[t] != ' ') ++t; // skip a word
				if (t == n || txt.substr(t, patterns[k].length()) != patterns[k]) {
					found = false;
					break;
				}
				++t;
			}
			if (found) {
				++res;
				highlightWords.insert(txt.substr(i, t - 1 + patterns[patterns.size() - 1].length() - i));
			}
		}

	}
	return res;
}

vector<ll> numbersInString(const string& word) {
	int sz = word.size(), num_digit = 0;
	ll cur_number = 0;
	vector<ll>results;
	bool last_char_is_digit = false;
	for (int i = sz - 1; i >= 0; --i) {
		if (isdigit(word[i])) {
			if (!last_char_is_digit) {
				cur_number = word[i] - '0';
				num_digit = 1;
				last_char_is_digit = true;
			}
			else 
				cur_number += (word[i] - '0')*int(pow(10, num_digit++));
		}
		else {
			if (last_char_is_digit) {
				results.push_back(cur_number);
				cur_number = 0;
				num_digit = 0;
			}
			last_char_is_digit = false;
		}
	}
	if (cur_number != 0) results.push_back(cur_number);
	return results;
}

bool wildCardMatch(const string& input, const string& pattern) {
	// Nghia
	int n = input.size(), m = pattern.size();
	//two empty string match with each other
	if (m == 0) return n == 0;
	vector<vector<bool>>lookup;
	vector<bool>tmp;
	for (int i = 0; i <= m; ++i) tmp.push_back(false);
	for (int i = 0; i <= n; ++i) lookup.push_back(tmp);
	// empty pattern can match with empty string
	lookup[0][0] = true;
	// Only '*' can match with empty string
	for (int j = 0; j <= m; j++)
		if (pattern[j - 1] == '*')
			lookup[0][j] = lookup[0][j - 1];
	// fill the table in bottom-up fashion
	for (int i = 1; i <= n; i++)
	{
		for (int j = 1; j <= m; j++)
		{
			// Two cases if we see a '*'
			// a) We ignore �*� character and move
			//    to next  character in the pattern,
			//     i.e., �*� indicates an empty sequence.
			// b) '*' character matches with ith
			//     character in input
			if (pattern[j - 1] == '*')
				lookup[i][j] = lookup[i][j - 1] ||
				lookup[i - 1][j];

			//if the characters match
			else if (input[i - 1] == pattern[j - 1])
				lookup[i][j] = lookup[i - 1][j - 1];

			// If characters don't match
			else lookup[i][j] = false;
		}
	}

	return lookup[n][m];
}



bool checkStopWordUsingTrie_t (string word) {
    // Return FALSE if NOT FOUND THE STOP WORD
    string sWords[] = {"a","able","about","above","abst","accordance","according","accordingly",
        "across","act","actually","added","adj","affected","affecting","affects","after",
        "afterwards","again","against","ah","all","almost","alone","along","already","also",
        "although","always","am","among","amongst","an","and","announce","another","any","anybody",
        "anyhow","anymore","anyone","anything","anyway","anyways","anywhere","apparently",
        "approximately","are","aren","arent","arise","around","as","aside","ask","asking","at","auth",
        "available","away","awfully","b","back","be","became","because","become","becomes","becoming",
        "been","before","beforehand","begin","beginning","beginnings","begins","behind","being",
        "believe","below","beside","besides","between","beyond","biol","both","brief","briefly","but",
        "by","c","ca","came","can","cannot","can't","cause","causes","certain","certainly","co","com",
        "come","comes","contain","containing","contains","could","couldnt","d","date","did","didn't",
        "different","do","does","doesn't","doing","done","don't","down","downwards","due","during","e",
        "each","ed","edu","effect","eg","eight","eighty","either","else","elsewhere","end",
        "ending","enough","especially","et","etal","etc","even","ever","every","everybody","everyone",
        "everything","everywhere","ex","except","f","far","few","ff","fifth","first","five","fix",
        "followed","following","follows","for","former","formerly","forth","found","four","from",
        "further","furthermore","g","gave","get","gets","getting","give","given","gives","giving",
        "go","goes","gone","got","gotten","h","had","happens","hardly","has","hasn't","have","haven't",
        "having","he","hed","hence","her","here","hereafter","hereby","herein","heres","hereupon",
        "hers","herself","hes","hi","hid","him","himself","his","hither","home","how","howbeit",
        "however","hundred","i","id","ie","if","i'll","im","immediate","immediately","importance",
        "important","in","inc","indeed","index","information","instead","into","invention","inward",
        "is","isn't","it","itd","it'll","its","itself","i've","j","just","k","keep","keeps","kept",
        "kg","km","know","known","knows","l","largely","last","lately","later","latter","latterly",
        "least","less","lest","let","lets","like","liked","likely","line","little","'ll","look",
        "looking","looks","ltd","m","made","mainly","make","makes","many","may","maybe","me","mean",
        "means","meantime","meanwhile","merely","mg","might","million","miss","ml","more","moreover",
        "most","mostly","mr","mrs","much","mug","must","my","myself","n","na","name","namely","nay",
        "nd","near","nearly","necessarily","necessary","need","needs","neither","never","nevertheless",
        "new","next","nine","ninety","no","nobody","non","none","nonetheless","noone","nor","normally",
        "nos","not","noted","nothing","now","nowhere","o","obtain","obtained","obviously","of","off",
        "often","oh","ok","okay","old","omitted","on","once","one","ones","only","onto","or","ord",
        "other","others","otherwise","ought","our","ours","ourselves","out","outside","over","overall",
        "owing","own","p","page","pages","part","particular","particularly","past","per","perhaps",
        "placed","please","plus","poorly","possible","possibly","potentially","pp","predominantly",
        "present","previously","primarily","probably","promptly","proud","provides","put","q","que",
        "quickly","quite","qv","r","ran","rather","rd","re","readily","really","recent","recently",
        "ref","refs","regarding","regardless","regards","related","relatively","research",
        "respectively","resulted","resulting","results","right","run","s","said","same","saw","say",
        "saying","says","sec","section","see","seeing","seem","seemed","seeming","seems","seen","self",
        "selves","sent","seven","several","shall","she","shed","she'll","shes","should","shouldn't",
        "show","showed","shown","showns","shows","significant","significantly","similar","similarly",
        "since","six","slightly","so","some","somebody","somehow","someone","somethan","something",
        "sometime","sometimes","somewhat","somewhere","soon","sorry","specifically","specified",
        "specify","specifying","still","stop","strongly","sub","substantially","successfully","such",
        "sufficiently","suggest","sup","sure","t","take","taken","taking","tell","tends","th","than",
        "thank","thanks","thanx","that","that'll","thats","that've","the","their","theirs","them",
        "themselves","then","thence","there","thereafter","thereby","thered","therefore","therein",
        "there'll","thereof","therere","theres","thereto","thereupon","there've","these","they",
        "theyd","they'll","theyre","they've","think","this","those","thou","though","thoughh",
        "thousand","throug","through","throughout","thru","thus","til","tip","to","together","too",
        "took","toward","towards","tried","tries","truly","try","trying","ts","twice","two","u","un",
        "under","unfortunately","unless","unlike","unlikely","until","unto","up","upon","ups","us",
        "use","used","useful","usefully","usefulness","uses","using","usually","v","value","various",
        "'ve","very","via","viz","vol","vols","vs","w","want","wants","was","wasnt","way","we","wed",
        "welcome","we'll","went","were","werent","we've","what","whatever","what'll","whats","when",
        "whence","whenever","where","whereafter","whereas","whereby","wherein","wheres","whereupon",
        "wherever","whether","which","while","whim","whither","who","whod","whoever","whole","who'll",
        "whom","whomever","whos","whose","why","widely","willing","wish","with","within","without",
        "wont","words","world","would","wouldnt","www","x","y","yes","yet","you","youd","you'll",
        "your","youre","yours","yourself","yourselves","you've","z","zero"};

	Trie_t stopWords;

    for (int i = 0; i < 667; i++) stopWords.insert(sWords[i]);
    if (stopWords.search(word) == NULL) return false;
    else return true;
    
    
}

string indexToFilename(int index) {
	if (index > MAX_FILE_COUNT) return "";
	return FILENAMES[index - 1];
}