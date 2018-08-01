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

vector<Node> merge(const vector<Node> &v1, const vector<Node> &v2) {
	vector<Node> result;
	result += substract(v1, v2);
	result += substract(v2, v1);
	result += intersect(v1, v2);
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
		for (j = v2.begin(); j != v2.end(); ++j)
			if ((*i).file_name == (*j).file_name)
				totalKeywordCount += (*j).keyword_count;
		result.push_back(Node(totalKeywordCount, (*i).file_name));
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
}

bool exist(vector<Node> v, int file_name) {
	vector<Node>::iterator it;
	for (it = v.begin(); it != v.end(); ++it) {
		if ((*it).file_name == file_name)
			return true;
	}
	return false;
}

//vector<Node> Trie_t::getKeywordData(string keyword) {
//	// TO DO: wildcard character (*)
//	if (keyword.find(" ") == string::npos) {
//		// if it is a token
//		if (keyword.size() >= 1 && (keyword[0] == '+' || keyword[0] == '-'))
//			return search(keyword.substr(1, string::npos))->file_list;
//		if (keyword.size() >= strlen("intitle:") && keyword.substr(0, 8) == "intitle:")
//			return search(keyword.substr(8, string::npos))->title_list;
//		if (keyword.size() >= 1 && keyword[0] == '~') {
//			vector<Node> ans = search(keyword.substr(1, string::npos))->file_list;
//			vector<string> syn = search(keyword.substr(1, string::npos))->synonyms;
//			syn.push_back(keyword);
//			int n = syn.size();
//			for (int i = 0; i < n; ++i)
//				ans = merge(ans, getKeywordData(syn[i]));
//			return ans;
//		}
//		if (keyword.find("..") != string::npos) {
//			vector<string> f = findExactValue(keyword, numbers);
//			vector<Node> result;
//			int n = f.size();
//			for (int i = 0; i < n; i++)
//				result = merge(result, getKeywordData(f[i]));
//			return result;
//		}
//		return search(keyword)->file_list;
//	}
//	// if it is an exact match phase
//	vector<string> kw = splitString(keyword);
//	vector<Node> result;
//	if (kw.size() == 0) return result;
//	result = getKeywordData(kw[0]);
//	for (int i = 1; i < kw.size(); ++i)
//		if (strcmp(kw[i].c_str(), "*") != 0)
//			result = intersect(result, getKeywordData(kw[i]));
//	// open file to recalculate word_count
//	ifstream fin;
//	for (int i = 0; i < result.size(); ++i) {
//		result[i].keyword_count = 0;
//		fin.open(getFileName(result[i].file_name).c_str());
//		if (!fin.is_open()) continue;
//		// TODO: count apperance in file
//		string text;
//		while (getline(fin, text)) {
//			result[i].keyword_count += countFreq(keyword, text);
//		}
//		fin.close();
//	}
//	// remove files with 0 word_count
//	for (int i = 0; i < result.size(); ++i)
//		if (result[i].keyword_count == 0)
//			result.erase(result.begin() + i);
//	return result;
//}

vector<string> findExactValue(string keyword, const vector<int> &exactVal) {
	// Minh
	// replace range in keyword by value in exactVal
	// assume there is at most 1 range in keyword
	// PreCondition: exactVal should be sorted 
	// if the startVal or endVal if empty it will assign INT_MIN or INT_MAX
	// eg. #worldcup..2018 == #worldcupINT_MIN..2018
	int dotPos = keyword.find("..");
	vector<string> stringsWithExactVal;
	if (dotPos == string::npos) return stringsWithExactVal;
	int startValPos = dotPos;
	while (startValPos >= 0 && keyword[startValPos - 1] >= '0' && keyword[startValPos - 1] <= '9')
		--startValPos;
	if (startValPos != 0 && keyword[startValPos - 1] == '-')
		--startValPos;
	int startVal;
	if (startValPos == dotPos)
		startVal = INT_MIN;
	else startVal = stoi(keyword.substr(startValPos, dotPos - startValPos));

	int endValPos = dotPos + 2;
	int n = keyword.size();
	if (endValPos < n - 1 && keyword[endValPos] == '-' && keyword[endValPos + 1] >= '0' && keyword[endValPos + 1] <= '9')
		++endValPos;
	while (endValPos < n && keyword[endValPos] >= '0' && keyword[endValPos] <= '9')
		++endValPos;
	int endVal;
	if (endValPos == dotPos + 2)
		endVal = INT_MAX;
	else endVal = stoi(keyword.substr(dotPos + 2, endValPos - dotPos - 2));

	if (startVal > endVal) return stringsWithExactVal;

	vector<int>::const_iterator findStartIndex = upper_bound(exactVal.begin(), exactVal.end(), startVal);
	vector<int>::const_iterator findEndIndex = lower_bound(exactVal.begin(), exactVal.end(), endVal);
	if (findStartIndex != exactVal.begin() && *(findStartIndex - 1) == startVal)
		--findStartIndex;
	if (*findEndIndex == endVal) ++findEndIndex;
	for (vector<int>::const_iterator i = findStartIndex; i != findEndIndex; ++i)
		stringsWithExactVal.push_back(keyword.substr(0, startValPos) + to_string(*i) + keyword.substr(endValPos, string::npos));
	return stringsWithExactVal;
}

vector<string> findExactValue(string keyword, const vector<int> &exactVal) {
	// Minh
	// replace range in keyword by value in exactVal
	// assume there is at most 1 range in keyword
	// PreCondition: exactVal should be sorted 
	// if the startVal or endVal if empty it will assign INT_MIN or INT_MAX
	// eg. #worldcup..2018 == #worldcupINT_MIN..2018
	int dotPos = keyword.find("..");
	vector<string> stringsWithExactVal;
	if (dotPos == string::npos) return stringsWithExactVal;
	int startValPos = dotPos;
	while (startValPos >= 0 && keyword[startValPos - 1] >= '0' && keyword[startValPos - 1] <= '9')
		--startValPos;
	if (startValPos != 0 && keyword[startValPos - 1] == '-')
		--startValPos;
	int startVal;
	if (startValPos == dotPos)
		startVal = INT_MIN;
	else startVal = stoi(keyword.substr(startValPos, dotPos - startValPos));

	int endValPos = dotPos + 2;
	int n = keyword.size();
	if (endValPos < n - 1 && keyword[endValPos] == '-' && keyword[endValPos + 1] >= '0' && keyword[endValPos + 1] <= '9')
		++endValPos;
	while (endValPos < n && keyword[endValPos] >= '0' && keyword[endValPos] <= '9')
		++endValPos;
	int endVal;
	if (endValPos == dotPos + 2)
		endVal = INT_MAX;
	else endVal = stoi(keyword.substr(dotPos + 2, endValPos - dotPos - 2));

	if (startVal > endVal) return stringsWithExactVal;

	vector<int>::const_iterator findStartIndex = upper_bound(exactVal.begin(), exactVal.end(), startVal);
	vector<int>::const_iterator findEndIndex = lower_bound(exactVal.begin(), exactVal.end(), endVal);
	if (findStartIndex != exactVal.begin() && *(findStartIndex - 1) == startVal)
		--findStartIndex;
	if (*findEndIndex == endVal) ++findEndIndex;
	for (vector<int>::const_iterator i = findStartIndex; i != findEndIndex; ++i)
		stringsWithExactVal.push_back(keyword.substr(0, startValPos) + to_string(*i) + keyword.substr(endValPos, string::npos));
	return stringsWithExactVal;
}

string getFileName(int fileName) {
	string name = to_string(fileName);
	name += ".txt";
	return name;
}

void preprocessing(string& word) {
	while (!isalnum(word[0]) && word[0]!='$' && word[0]!='#')
		word.erase(word.begin());
	while (!isalnum(*(word.rbegin())))
		word.pop_back();
}
vector<Node> Trie_t::getQueryData(string quiery) {
	vector<string> tokens = splitString(quiery);
	int n = tokens.size();
	vector<Node> result = getKeywordData(tokens[0]);
	bool pendingOrOperator = false;
	for (int i = 1; i < n; ++i) {
		if (isStopWord(tokens[i])) continue;
		if (pendingOrOperator) {
			pendingOrOperator = false;
			result = intersect(result, merge(getKeywordData(tokens[i - 2]), getKeywordData(tokens[i])));
			continue;
		}
		if (i < n - 1 && strcmp(tokens[i + 1].c_str(), "OR") == 0) {
			pendingOrOperator = true;
			continue;
		}
		if (tokens[i].size() >= 1 && tokens[i][0] == '-') {
			result = substract(result, getKeywordData(tokens[i]));
			continue;
		}
		result = intersect(result, getKeywordData(tokens[i]));
	}
	sort(result.begin(), result.end(), NodeMaxFirst);
	return result;
}

int countFreq(const string &pat, const string &txt) {
	stringstream ss(pat);
	string pat1;
	string pat2;
	getline(ss, pat1, '*');
	getline(ss, pat2);
	int M = pat1.length();
	int N = txt.length();
	int res = 0;

	/* A loop to slide pat[] one by one */
	for (int i = 0; i <= N - M; i++) {
		/* For current index i, check for
		pattern match */
		int j;
		for (j = 0; j < M; j++)
			if (txt[i + j] != pat1[j])
				break;

		// if pat[0...M-1] = txt[i, i+1, ...i+M-1]
		if (j == M) {
			int t = i + j + 1;
			while (t < N && txt[t] != ' ') t++;
			if (pat2.length() == 0) res++;
			else if (t < N && txt.substr(t, pat2.length()) == pat2) res++;
		}
	}
	return res;
}

vector<ll> numbersInString(const string& word) {
	int sz = word.size(), num_digit = 0;
	ll cur_number;
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
			// a) We ignore ‘*’ character and move
			//    to next  character in the pattern,
			//     i.e., ‘*’ indicates an empty sequence.
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

bool checkStopWordUsingBS(string word, int l, int r) {
    
    // Using binary search to find whether user searching contains stop word or not.
    // int l = 0, r = 666;
    if (r >= l)
    {
        int mid = l + (r - l)/2;
        
        // Found stop word - Return TRUE
        if (sWords[mid].compare(word) == 0)
            return true;
        
        if (sWords[mid].compare(word) > 0)
            return checkStopWordUsingBS(word, l, mid-1);
        
        return checkStopWordUsingBS(word, mid+1, r);
    }
    
    return false;
}

bool initCheckStopWordUsingBS(string word) {
    locale loc;
    for (string::size_type i = 0; i<word.length(); ++i)
        word[i] = tolower(word[i],loc);
    
    return checkStopWord(word, 0, 666);
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

    Trie_t *stopWords;
    for (int i = 0; i < 667; i++) stopWords.insert(sWords[i]);
    if (stopWords.search(word) == NULL) return false;
    else return true;
    
    delete stopWords;
    
}
