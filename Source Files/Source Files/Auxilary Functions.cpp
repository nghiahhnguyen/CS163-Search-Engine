#include "Function.h"
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
	string FILENAMES[] = { "0.txt","1.txt","10.txt","100.txt","11.txt","12.txt","13.txt","14.txt","15.txt","16.txt","17.txt","18.txt","19.txt","2.txt","20.txt","21.txt","22.txt","23.txt","24.txt","25.txt","26.txt","27.txt","28.txt","29.txt","3.txt","30.txt","31.txt","32.txt","33.txt","34.txt","35.txt","36.txt","37.txt","38.txt","39.txt","4.txt","40.txt","41.txt","42.txt","43.txt","44.txt","45.txt","46.txt","47.txt","48.txt","49.txt","5.txt","50.txt","51.txt","52.txt","53.txt","54.txt","55.txt","56.txt","57.txt","58.txt","59.txt","6.txt","60.txt","61.txt","62.txt","63.txt","64.txt","65.txt","66.txt","67.txt","68.txt","69.txt","7.txt","70.txt","71.txt","72.txt","73.txt","74.txt","75.txt","76.txt","77.txt","78.txt","79.txt","8.txt","80.txt","81.txt","82.txt","83.txt","84.txt","85.txt","86.txt","87.txt","88.txt","89.txt","9.txt","90.txt","91.txt","92.txt","93.txt","94.txt","95.txt","96.txt","97.txt","98.txt","99.txt","Group01News01.txt","Group01News02.txt","Group01News03.txt","Group01News04.txt","Group01News05.txt","Group01News06.txt","Group01News07.txt","Group01News08.txt","Group01News09.txt","Group01News10.txt","Group01News100.txt","Group01News11.txt","Group01News12.txt","Group01News13.txt","Group01News14.txt","Group01News15.txt","Group01News16.txt","Group01News17.txt","Group01News18.txt","Group01News19.txt","Group01News20.txt","Group01News21.txt","Group01News22.txt","Group01News23.txt","Group01News24.txt","Group01News25.txt","Group01News26.txt","Group01News27.txt","Group01News28.txt","Group01News29.txt","Group01News30.txt","Group01News31.txt","Group01News32.txt","Group01News33.txt","Group01News34.txt","Group01News35.txt","Group01News36.txt","Group01News37.txt","Group01News38.txt","Group01News39.txt","Group01News40.txt","Group01News41.txt","Group01News42.txt","Group01News43.txt","Group01News44.txt","Group01News45.txt","Group01News46.txt","Group01News47.txt","Group01News48.txt","Group01News49.txt","Group01News50.txt","Group01News51.txt","Group01News52.txt","Group01News53.txt","Group01News54.txt","Group01News55.txt","Group01News56.txt","Group01News57.txt","Group01News58.txt","Group01News59.txt","Group01News60.txt","Group01News61.txt","Group01News62.txt","Group01News63.txt","Group01News64.txt","Group01News65.txt","Group01News66.txt","Group01News67.txt","Group01News68.txt","Group01News69.txt","Group01News70.txt","Group01News71.txt","Group01News72.txt","Group01News73.txt","Group01News74.txt","Group01News75.txt","Group01News76.txt","Group01News77.txt","Group01News78.txt","Group01News79.txt","Group01News80.txt","Group01News81.txt","Group01News82.txt","Group01News83.txt","Group01News84.txt","Group01News85.txt","Group01News86.txt","Group01News87.txt","Group01News88.txt","Group01News89.txt","Group01News90.txt","Group01News91.txt","Group01News92.txt","Group01News93.txt","Group01News94.txt","Group01News95.txt","Group01News96.txt","Group01News97.txt","Group01News98.txt","Group01News99.txt","Group02News01.txt","Group02News02.txt","Group02News03.txt","Group02News04.txt","Group02News05.txt","Group02News06.txt","Group02News07.txt","Group02News08.txt","Group02News09.txt","Group02News10.txt","Group02News100.txt","Group02News11.txt","Group02News12.txt","Group02News13.txt","Group02News14.txt","Group02News15.txt","Group02News16.txt","Group02News17.txt","Group02News18.txt","Group02News19.txt","Group02News20.txt","Group02News21.txt","Group02News22.txt","Group02News23.txt","Group02News24.txt","Group02News25.txt","Group02News26.txt","Group02News27.txt","Group02News28.txt","Group02News29.txt","Group02News30.txt","Group02News31.txt","Group02News32.txt","Group02News33.txt","Group02News34.txt","Group02News35.txt","Group02News36.txt","Group02News37.txt","Group02News38.txt","Group02News39.txt","Group02News40.txt","Group02News41.txt","Group02News42.txt","Group02News43.txt","Group02News44.txt","Group02News45.txt","Group02News46.txt","Group02News47.txt","Group02News48.txt","Group02News49.txt","Group02News50.txt","Group02News51.txt","Group02News52.txt","Group02News53.txt","Group02News54.txt","Group02News55.txt","Group02News56.txt","Group02News57.txt","Group02News58.txt","Group02News59.txt","Group02News60.txt","Group02News61.txt","Group02News62.txt","Group02News63.txt","Group02News64.txt","Group02News65.txt","Group02News66.txt","Group02News67.txt","Group02News68.txt","Group02News69.txt","Group02News70.txt","Group02News71.txt","Group02News72.txt","Group02News73.txt","Group02News74.txt","Group02News75.txt","Group02News76.txt","Group02News77.txt","Group02News78.txt","Group02News79.txt","Group02News80.txt","Group02News81.txt","Group02News82.txt","Group02News83.txt","Group02News84.txt","Group02News85.txt","Group02News86.txt","Group02News87.txt","Group02News88.txt","Group02News89.txt","Group02News90.txt","Group02News91.txt","Group02News92.txt","Group02News93.txt","Group02News94.txt","Group02News95.txt","Group02News96.txt","Group02News97.txt","Group02News98.txt","Group02News99.txt","Group03News00.txt","Group03News01.txt","Group03News02.txt","Group03News03.txt","Group03News04.txt","Group03News05.txt","Group03News06.txt","Group03News07.txt","Group03News08.txt","Group03News09.txt","Group03News10.txt","Group03News11.txt","Group03News12.txt","Group03News13.txt","Group03News14.txt","Group03News15.txt","Group03News16.txt","Group03News17.txt","Group03News18.txt","Group03News19.txt","Group03News20.txt","Group03News21.txt","Group03News22.txt","Group03News23.txt","Group03News24.txt","Group03News25.txt","Group03News26.txt","Group03News27.txt","Group03News28.txt","Group03News29.txt","Group03News30.txt","Group03News31.txt","Group03News32.txt","Group03News33.txt","Group03News34.txt","Group03News35.txt","Group03News36.txt","Group03News37.txt","Group03News38.txt","Group03News39.txt","Group03News40.txt","Group03News41.txt","Group03News42.txt","Group03News43.txt","Group03News44.txt","Group03News45.txt","Group03News46.txt","Group03News47.txt","Group03News48.txt","Group03News49.txt","Group03News50.txt","Group03News51.txt","Group03News52.txt","Group03News53.txt","Group03News54.txt","Group03News55.txt","Group03News56.txt","Group03News57.txt","Group03News58.txt","Group03News59.txt","Group03News60.txt","Group03News61.txt","Group03News62.txt","Group03News63.txt","Group03News64.txt","Group03News65.txt","Group03News66.txt","Group03News67.txt","Group03News68.txt","Group03News69.txt","Group03News70.txt","Group03News71.txt","Group03News72.txt","Group03News73.txt","Group03News74.txt","Group03News75.txt","Group03News76.txt","Group03News77.txt","Group03News78.txt","Group03News79.txt","Group03News80.txt","Group03News81.txt","Group03News82.txt","Group03News83.txt","Group03News84.txt","Group03News85.txt","Group03News86.txt","Group03News87.txt","Group03News88.txt","Group03News89.txt","Group03News90.txt","Group03News91.txt","Group03News92.txt","Group03News93.txt","Group03News94.txt","Group03News95.txt","Group03News96.txt","Group03News97.txt","Group03News98.txt","Group03News99.txt","Group04News01.txt","Group04News02.txt","Group04News03.txt","Group04News04.txt","Group04News05.txt","Group04News06.txt","Group04News07.txt","Group04News08.txt","Group04News09.txt","Group04News10.txt","Group04News100.txt","Group04News11.txt","Group04News12.txt","Group04News13.txt","Group04News14.txt","Group04News15.txt","Group04News16.txt","Group04News17.txt","Group04News18.txt","Group04News19.txt","Group04News20.txt","Group04News21.txt","Group04News22.txt","Group04News23.txt","Group04News24.txt","Group04News25.txt","Group04News26.txt","Group04News27.txt","Group04News28.txt","Group04News29.txt","Group04News30.txt","Group04News31.txt","Group04News32.txt","Group04News33.txt","Group04News34.txt","Group04News35.txt","Group04News36.txt","Group04News37.txt","Group04News38.txt","Group04News39.txt","Group04News40.txt","Group04News41.txt","Group04News42.txt","Group04News43.txt","Group04News44.txt","Group04News45.txt","Group04News46.txt","Group04News47.txt","Group04News48.txt","Group04News49.txt","Group04News50.txt","Group04News51.txt","Group04News52.txt","Group04News53.txt","Group04News54.txt","Group04News55.txt","Group04News56.txt","Group04News57.txt","Group04News58.txt","Group04News59.txt","Group04News60.txt","Group04News61.txt","Group04News62.txt","Group04News63.txt","Group04News64.txt","Group04News65.txt","Group04News66.txt","Group04News67.txt","Group04News68.txt","Group04News69.txt","Group04News70.txt","Group04News71.txt","Group04News72.txt","Group04News73.txt","Group04News74.txt","Group04News75.txt","Group04News76.txt","Group04News77.txt","Group04News78.txt","Group04News79.txt","Group04News80.txt","Group04News81.txt","Group04News82.txt","Group04News83.txt","Group04News84.txt","Group04News85.txt","Group04News86.txt","Group04News87.txt","Group04News88.txt","Group04News89.txt","Group04News90.txt","Group04News91.txt","Group04News92.txt","Group04News93.txt","Group04News94.txt","Group04News95.txt","Group04News96.txt","Group04News97.txt","Group04News98.txt","Group04News99.txt","Group05News00.txt","Group05News01.txt","Group05News02.txt","Group05News03.txt","Group05News04.txt","Group05News05.txt","Group05News06.txt","Group05News07.txt","Group05News08.txt","Group05News09.txt","Group05News10.txt","Group05News11.txt","Group05News12.txt","Group05News13.txt","Group05News14.txt","Group05News15.txt","Group05News16.txt","Group05News17.txt","Group05News18.txt","Group05News19.txt","Group05News20.txt","Group05News21.txt","Group05News22.txt","Group05News23.txt","Group05News24.txt","Group05News25.txt","Group05News26.txt","Group05News27.txt","Group05News28.txt","Group05News29.txt","Group05News30.txt","Group05News31.txt","Group05News32.txt","Group05News33.txt","Group05News34.txt","Group05News35.txt","Group05News36.txt","Group05News37.txt","Group05News38.txt","Group05News39.txt","Group05News40.txt","Group05News41.txt","Group05News42.txt","Group05News43.txt","Group05News44.txt","Group05News45.txt","Group05News46.txt","Group05News47.txt","Group05News48.txt","Group05News49.txt","Group05News50.txt","Group05News51.txt","Group05News52.txt","Group05News53.txt","Group05News54.txt","Group05News55.txt","Group05News56.txt","Group05News57.txt","Group05News58.txt","Group05News59.txt","Group05News60.txt","Group05News61.txt","Group05News62.txt","Group05News63.txt","Group05News64.txt","Group05News65.txt","Group05News66.txt","Group05News67.txt","Group05News68.txt","Group05News69.txt","Group05News70.txt","Group05News71.txt","Group05News72.txt","Group05News73.txt","Group05News74.txt","Group05News75.txt","Group05News76.txt","Group05News77.txt","Group05News78.txt","Group05News79.txt","Group05News80.txt","Group05News81.txt","Group05News82.txt","Group05News83.txt","Group05News84.txt","Group05News85.txt","Group05News86.txt","Group05News87.txt","Group05News88.txt","Group05News89.txt","Group05News90.txt","Group05News91.txt","Group05News92.txt","Group05News93.txt","Group05News94.txt","Group05News95.txt","Group05News96.txt","Group05News97.txt","Group05News98.txt","Group05News99.txt","Group06News01.txt","Group06News02.txt","Group06News03.txt","Group06News04.txt","Group06News05.txt","Group06News06.txt","Group06News07.txt","Group06News08.txt","Group06News09.txt","Group06News10.txt","Group06News100.txt","Group06News11.txt","Group06News12.txt","Group06News13.txt","Group06News14.txt","Group06News15.txt","Group06News16.txt","Group06News17.txt","Group06News18.txt","Group06News19.txt","Group06News20.txt","Group06News21.txt","Group06News22.txt","Group06News23.txt","Group06News24.txt","Group06News25.txt","Group06News26.txt","Group06News27.txt","Group06News28.txt","Group06News29.txt","Group06News30.txt","Group06News31.txt","Group06News32.txt","Group06News33.txt","Group06News34.txt","Group06News35.txt","Group06News36.txt","Group06News37.txt","Group06News38.txt","Group06News39.txt","Group06News40.txt","Group06News41.txt","Group06News42.txt","Group06News43.txt","Group06News44.txt","Group06News45.txt","Group06News46.txt","Group06News47.txt","Group06News48.txt","Group06News49.txt","Group06News50.txt","Group06News51.txt","Group06News52.txt","Group06News53.txt","Group06News54.txt","Group06News55.txt","Group06News56.txt","Group06News57.txt","Group06News58.txt","Group06News59.txt","Group06News60.txt","Group06News61.txt","Group06News62.txt","Group06News63.txt","Group06News64.txt","Group06News65.txt","Group06News66.txt","Group06News67.txt","Group06News68.txt","Group06News69.txt","Group06News70.txt","Group06News71.txt","Group06News72.txt","Group06News73.txt","Group06News74.txt","Group06News75.txt","Group06News76.txt","Group06News77.txt","Group06News78.txt","Group06News79.txt","Group06News80.txt","Group06News81.txt","Group06News82.txt","Group06News83.txt","Group06News84.txt","Group06News85.txt","Group06News86.txt","Group06News87.txt","Group06News88.txt","Group06News89.txt","Group06News90.txt","Group06News91.txt","Group06News92.txt","Group06News93.txt","Group06News94.txt","Group06News95.txt","Group06News96.txt","Group06News97.txt","Group06News98.txt","Group06News99.txt","Group07News01.txt","Group07News02.txt","Group07News03.txt","Group07News04.txt","Group07News05.txt","Group07News06.txt","Group07News07.txt","Group07News08.txt","Group07News09.txt","Group07News10.txt","Group07News100.txt","Group07News101.txt","Group07News102.txt","Group07News103.txt","Group07News104.txt","Group07News105.txt","Group07News106.txt","Group07News107.txt","Group07News108.txt","Group07News109.txt","Group07News11.txt","Group07News110.txt","Group07News111.txt","Group07News112.txt","Group07News12.txt","Group07News13.txt","Group07News14.txt","Group07News15.txt","Group07News16.txt","Group07News17.txt","Group07News18.txt","Group07News19.txt","Group07News20.txt","Group07News21.txt","Group07News22.txt","Group07News23.txt","Group07News24.txt","Group07News25.txt","Group07News26.txt","Group07News27.txt","Group07News28.txt","Group07News29.txt","Group07News30.txt","Group07News31.txt","Group07News32.txt","Group07News33.txt","Group07News34.txt","Group07News35.txt","Group07News36.txt","Group07News37.txt","Group07News38.txt","Group07News39.txt","Group07News40.txt","Group07News41.txt","Group07News42.txt","Group07News43.txt","Group07News44.txt","Group07News45.txt","Group07News46.txt","Group07News47.txt","Group07News48.txt","Group07News49.txt","Group07News50.txt","Group07News51.txt","Group07News52.txt","Group07News53.txt","Group07News54.txt","Group07News55.txt","Group07News56.txt","Group07News57.txt","Group07News58.txt","Group07News59.txt","Group07News60.txt","Group07News61.txt","Group07News62.txt","Group07News63.txt","Group07News64.txt","Group07News65.txt","Group07News66.txt","Group07News67.txt","Group07News68.txt","Group07News69.txt","Group07News70.txt","Group07News71.txt","Group07News72.txt","Group07News73.txt","Group07News74.txt","Group07News75.txt","Group07News76.txt","Group07News77.txt","Group07News78.txt","Group07News79.txt","Group07News80.txt","Group07News81.txt","Group07News82.txt","Group07News83.txt","Group07News84.txt","Group07News85.txt","Group07News86.txt","Group07News87.txt","Group07News88.txt","Group07News89.txt","Group07News90.txt","Group07News91.txt","Group07News92.txt","Group07News93.txt","Group07News94.txt","Group07News95.txt","Group07News96.txt","Group07News97.txt","Group07News98.txt","Group07News99.txt","Group08News01.txt","Group08News02.txt","Group08News03.txt","Group08News04.txt","Group08News05.txt","Group08News06.txt","Group08News07.txt","Group08News08.txt","Group08News09.txt","Group08News10.txt","Group08News100.txt","Group08News11.txt","Group08News12.txt","Group08News13.txt","Group08News14.txt","Group08News15.txt","Group08News16.txt","Group08News17.txt","Group08News18.txt","Group08News19.txt","Group08News20.txt","Group08News21.txt","Group08News22.txt","Group08News23.txt","Group08News24.txt","Group08News25.txt","Group08News26.txt","Group08News27.txt","Group08News28.txt","Group08News29.txt","Group08News30.txt","Group08News31.txt","Group08News32.txt","Group08News33.txt","Group08News34.txt","Group08News35.txt","Group08News36.txt","Group08News37.txt","Group08News38.txt","Group08News39.txt","Group08News40.txt","Group08News41.txt","Group08News42.txt","Group08News43.txt","Group08News44.txt","Group08News45.txt","Group08News46.txt","Group08News47.txt","Group08News48.txt","Group08News49.txt","Group08News50.txt","Group08News51.txt","Group08News52.txt","Group08News53.txt","Group08News54.txt","Group08News55.txt","Group08News56.txt","Group08News57.txt","Group08News58.txt","Group08News59txt.txt","Group08News60.txt","Group08News61.txt","Group08News62.txt","Group08News63.txt","Group08News64.txt","Group08News65.txt","Group08News66.txt","Group08News67.txt","Group08News68.txt","Group08News69.txt","Group08News70.txt","Group08News71.txt","Group08News72.txt","Group08News73.txt","Group08News74.txt","Group08News75.txt","Group08News76.txt","Group08News77.txt","Group08News78.txt","Group08News79.txt","Group08News80.txt","Group08News81.txt","Group08News82.txt","Group08News83.txt","Group08News84.txt","Group08News85.txt","Group08News86.txt","Group08News87.txt","Group08News88.txt","Group08News89.txt","Group08News90.txt","Group08News91.txt","Group08News92.txt","Group08News93.txt","Group08News94.txt","Group08News95.txt","Group08News96.txt","Group08News97.txt","Group08News98.txt","Group08News99.txt","Group09News00.txt","Group09News01.txt","Group09News02.txt","Group09News03.txt","Group09News04.txt","Group09News05.txt","Group09News06.txt","Group09News07.txt","Group09News08.txt","Group09News09.txt","Group09News10.txt","Group09News11.txt","Group09News12.txt","Group09News13.txt","Group09News14.txt","Group09News15.txt","Group09News16.txt","Group09News17.txt","Group09News18.txt","Group09News19.txt","Group09News20.txt","Group09News21.txt","Group09News22.txt","Group09News23.txt","Group09News24.txt","Group09News25.txt","Group09News26.txt","Group09News27.txt","Group09News28.txt","Group09News29.txt","Group09News30.txt","Group09News31.txt","Group09News32.txt","Group09News33.txt","Group09News34.txt","Group09News35.txt","Group09News36.txt","Group09News37.txt","Group09News38.txt","Group09News39.txt","Group09News40.txt","Group09News41.txt","Group09News42.txt","Group09News43.txt","Group09News44.txt","Group09News45.txt","Group09News46.txt","Group09News47.txt","Group09News48.txt","Group09News49.txt","Group09News50.txt","Group09News51.txt","Group09News52.txt","Group09News53.txt","Group09News54.txt","Group09News55.txt","Group09News56.txt","Group09News57.txt","Group09News58.txt","Group09News59.txt","Group09News60.txt","Group09News61.txt","Group09News62.txt","Group09News63.txt","Group09News64.txt","Group09News65.txt","Group09News66.txt","Group09News67.txt","Group09News68.txt","Group09News69.txt","Group09News70.txt","Group09News71.txt","Group09News72.txt","Group09News73.txt","Group09News74.txt","Group09News75.txt","Group09News76.txt","Group09News77.txt","Group09News78.txt","Group09News79.txt","Group09News80.txt","Group09News81.txt","Group09News82.txt","Group09News83.txt","Group09News84.txt","Group09News85.txt","Group09News86.txt","Group09News87.txt","Group09News88.txt","Group09News89.txt","Group09News90.txt","Group09News91.txt","Group09News92.txt","Group09News93.txt","Group09News94.txt","Group09News95.txt","Group09News96.txt","Group09News97.txt","Group09News98.txt","Group09News99.txt","group10news01.txt","group10news02.txt","group10news03.txt","group10news04.txt","group10news05.txt","group10news06.txt","group10news07.txt","group10news08.txt","group10news09.txt","group10news10.txt","group10news11.txt","group10news12.txt","group10news13.txt","group10news14.txt","group10news15.txt","group10news16.txt","group10news17.txt","group10news18.txt","group10news19.txt","group10news20.txt","group10news21.txt","group10news22.txt","group10news23.txt","group10news24.txt","group10news25.txt","group10news26.txt","group10news27.txt","group10news28.txt","group10news29.txt","group10news30.txt","group10news31.txt","group10news32.txt","group10news33.txt","group10news34.txt","group10news35.txt","group10news36.txt","group10news37.txt","group10news38.txt","group10news39.txt","group10news40.txt","group10news41.txt","group10news42.txt","group10news43.txt","group10news44.txt","group10news45.txt","group10news46.txt","group10news47.txt","group10news48.txt","group10news49.txt","group10news50.txt","group10news51.txt","group10news52.txt","group10news53.txt","group10news54.txt","group10news55.txt","group10news56.txt","group10news57.txt","group10news58.txt","group10news59.txt","group10news60.txt","group10news61.txt","group10news62.txt","group10news63.txt","group10news64.txt","Group10News69.txt","Group10News70.txt","Group10News71.txt","Group10News72.txt","Group10News73.txt","Group10News74.txt","Group10News75.txt","Group10News76.txt","Group10News77.txt","Group10News78.txt","Group10News79.txt","Group10News80.txt","Group10News81.txt","Group10News82.txt","Group10News83.txt","Group10News84.txt","Group10News85.txt","Group10News86.txt","Group10News87.txt","Group10News88.txt","Group10News89.txt","Group10News90.txt","Group10News91.txt","Group10News92.txt","Group10News93.txt","Group10News94.txt","Group10News95.txt","Group10News96.txt","Group10News97.txt","Group10News98.txt","Group10News99.txt","Group11News00.txt","Group11News01.txt","Group11News02.txt","Group11News03.txt","Group11News04.txt","Group11News05.txt","Group11News06.txt","Group11News07.txt","Group11News08.txt","Group11News09.txt","Group11News10.txt","Group11News11.txt","Group11News12.txt","Group11News13.txt","Group11News14.txt","Group11News15.txt","Group11News16.txt","Group11News17.txt","Group11News18.txt","Group11News19.txt","Group11News20.txt","Group11News21.txt","Group11News22.txt","Group11News23.txt","Group11News24.txt","Group11News25.txt","Group11News26.txt","Group11News27.txt","Group11News28.txt","Group11News29.txt","Group11News30.txt","Group11News31.txt","Group11News32.txt","Group11News33.txt","Group11News34.txt","Group11News35.txt","Group11News36.txt","Group11News37.txt","Group11News38.txt","Group11News39.txt","Group11News40.txt","Group11News41.txt","Group11News42.txt","Group11News43.txt","Group11News44.txt","Group11News45.txt","Group11News46.txt","Group11News47.txt","Group11News48.txt","Group11News49.txt","Group11News50.txt","Group11News51.txt","Group11News52.txt","Group11News53.txt","Group11News54.txt","Group11News55.txt","Group11News56.txt","Group11News57.txt","Group11News58.txt","Group11News59.txt","Group11News60.txt","Group11News61.txt","Group11News62.txt","Group11News63.txt","Group11News64.txt","Group11News65.txt","Group11News66.txt","Group11News67.txt","Group11News68.txt","Group11News69.txt","Group11News70.txt","Group11News71.txt","Group11News72.txt","Group11News73.txt","Group11News74.txt","Group11News75.txt","Group11News76.txt","Group11News77.txt","Group11News78.txt","Group11News79.txt","Group11News80.txt","Group11News81.txt","Group11News82.txt","Group11News83.txt","Group11News84.txt","Group11News85.txt","Group11News86.txt","Group11News87.txt","Group11News88.txt","Group11News89.txt","Group11News90.txt","Group11News91.txt","Group11News92.txt","Group11News93.txt","Group11News94.txt","Group11News95.txt","Group11News96.txt","Group11News97.txt","Group11News98.txt","Group11News99.txt","Group12News00.txt","Group12News01.txt","Group12News02.txt","Group12News03.txt","Group12News04.txt","Group12News05.txt","Group12News06.txt","Group12News07.txt","Group12News08.txt","Group12News09.txt","Group12News10.txt","Group12News11.txt","Group12News12.txt","Group12News13.txt","Group12News14.txt","Group12News15.txt","Group12News16.txt","Group12News17.txt","Group12News18.txt","Group12News19.txt","Group12News20.txt","Group12News21.txt","Group12News22.txt","Group12News23.txt","Group12News24.txt","Group12News25.txt","Group12News26.txt","Group12News27.txt","Group12News28.txt","Group12News29.txt","Group12News30.txt","Group12News31.txt","Group12News32.txt","Group12News33.txt","Group12News34.txt","Group12News35.txt","Group12News36.txt","Group12News37.txt","Group12News38.txt","Group12News39.txt","Group12News40.txt","Group12News41.txt","Group12News42.txt","Group12News43.txt","Group12News44.txt","Group12News45.txt","Group12News46.txt","Group12News47.txt","Group12News48.txt","Group12News49.txt","Group12News50.txt","Group12News51.txt","Group12News52.txt","Group12News53.txt","Group12News54.txt","Group12News55.txt","Group12News56.txt","Group12News57.txt","Group12News58.txt","Group12News59.txt","Group12News60.txt","Group12News61.txt","Group12News62.txt","Group12News63.txt","Group12News64.txt","Group12News65.txt","Group12News66.txt","Group12News67.txt","Group12News68.txt","Group12News69.txt","Group12News70.txt","Group12News71.txt","Group12News72.txt","Group12News73.txt","Group12News74.txt","Group12News75.txt","Group12News76.txt","Group12News77.txt","Group12News78.txt","Group12News79.txt","Group12News80.txt","Group12News81.txt","Group12News82.txt","Group12News83.txt","Group12News84.txt","Group12News85.txt","Group12News86.txt","Group12News87.txt","Group12News88.txt","Group12News89.txt","Group12News90.txt","Group12News91.txt","Group12News92.txt","Group12News93.txt","Group12News94.txt","Group12News95.txt","Group12News96.txt","Group12News97.txt","Group12News98.txt","Group12News99.txt","Group13News01.txt","Group13News02.txt","Group13News03.txt","Group13News04.txt","Group13News05.txt","Group13News06.txt","Group13News07.txt","Group13News08.txt","Group13News09.txt","Group13News10.txt","Group13News100.txt","Group13News11.txt","Group13News12.txt","Group13News13.txt","Group13News14.txt","Group13News15.txt","Group13News16.txt","Group13News17.txt","Group13News18.txt","Group13News19.txt","Group13News20.txt","Group13News21.txt","Group13News22.txt","Group13News23.txt","Group13News24.txt","Group13News25.txt","Group13News26.txt","Group13News27.txt","Group13News28.txt","Group13News29.txt","Group13News30.txt","Group13News31.txt","Group13News32.txt","Group13News33.txt","Group13News34.txt","Group13News35.txt","Group13News36.txt","Group13News37.txt","Group13News38.txt","Group13News39.txt","Group13News40.txt","Group13News41.txt","Group13News42.txt","Group13News43.txt","Group13News44.txt","Group13News45.txt","Group13News46.txt","Group13News47.txt","Group13News48.txt","Group13News49.txt","Group13News50.txt","Group13News51.txt","Group13News52.txt","Group13News53.txt","Group13News54.txt","Group13News55.txt","Group13News56.txt","Group13News57.txt","Group13News58.txt","Group13News59.txt","Group13News60.txt","Group13News61.txt","Group13News62.txt","Group13News63.txt","Group13News64.txt","Group13News65.txt","Group13News66.txt","Group13News67.txt","Group13News68.txt","Group13News69.txt","Group13News70.txt","Group13News71.txt","Group13News72.txt","Group13News73.txt","Group13News74.txt","Group13News75.txt","Group13News76.txt","Group13News77.txt","Group13News78.txt","Group13News79.txt","Group13News80.txt","Group13News81.txt","Group13News82.txt","Group13News83.txt","Group13News84.txt","Group13News85.txt","Group13News86.txt","Group13News87.txt","Group13News88.txt","Group13News89.txt","Group13News90.txt","Group13News91.txt","Group13News92.txt","Group13News93.txt","Group13News94.txt","Group13News95.txt","Group13News96.txt","Group13News97.txt","Group13News98.txt","Group13News99.txt","Group14News01.txt","Group14News02.txt","Group14News03.txt","Group14News04.txt","Group14News05.txt","Group14News06.txt","Group14News07.txt","Group14News08.txt","Group14News09.txt","Group14News10.txt","Group14News100.txt","Group14News11.txt","Group14News12.txt","Group14News14.txt","Group14News15.txt","Group14News16.txt","Group14News17.txt","Group14News18.txt","Group14News19.txt","Group14News20.txt","Group14News21.txt","Group14News22.txt","Group14News23.txt","Group14News24.txt","Group14News25.txt","Group14News26.txt","Group14News27.txt","Group14News28.txt","Group14News29.txt","Group14News30.txt","Group14News31.txt","Group14News32.txt","Group14News33.txt","Group14News34.txt","Group14News35.txt","Group14News36.txt","Group14News37.txt","Group14News38.txt","Group14News39.txt","Group14News40.txt","Group14News41.txt","Group14News42.txt","Group14News43.txt","Group14News44.txt","Group14News45.txt","Group14News46.txt","Group14News47.txt","Group14News48.txt","Group14News49.txt","Group14News50.txt","Group14News51.txt","Group14News52.txt","Group14News53.txt","Group14News54.txt","Group14News55.txt","Group14News56.txt","Group14News57.txt","Group14News58.txt","Group14News59.txt","Group14News60.txt","Group14News61.txt","Group14News62.txt","Group14News63.txt","Group14News64.txt","Group14News65.txt","Group14News66.txt","Group14News67.txt","Group14News68.txt","Group14News69.txt","Group14News70.txt","Group14News71.txt","Group14News72.txt","Group14News73.txt","Group14News74.txt","Group14News75.txt","Group14News76.txt","Group14News77.txt","Group14News78.txt","Group14News79.txt","Group14News80.txt","Group14News81.txt","Group14News82.txt","Group14News83.txt","Group14News84.txt","Group14News85.txt","Group14News86.txt","Group14News87.txt","Group14News88.txt","Group14News89.txt","Group14News90.txt","Group14News91.txt","Group14News92.txt","Group14News93.txt","Group14News94.txt","Group14News95.txt","Group14News96.txt","Group14News97.txt","Group14News98.txt","Group14News99.txt","Group15News00.txt","Group15News01.txt","Group15News02.txt","Group15News03.txt","Group15News04.txt","Group15News05.txt","Group15News06.txt","Group15News07.txt","Group15News08.txt","Group15News09.txt","Group15News10.txt","Group15News11.txt","Group15News12.txt","Group15News13.txt","Group15News14.txt","Group15News15.txt","Group15News16.txt","Group15News17.txt","Group15News18.txt","Group15News19.txt","Group15News20.txt","Group15News21.txt","Group15News22.txt","Group15News23.txt","Group15News24.txt","Group15News25.txt","Group15News26.txt","Group15News27.txt","Group15News28.txt","Group15News29.txt","Group15News30.txt","Group15News31.txt","Group15News32.txt","Group15News33.txt","Group15News34.txt","Group15News35.txt","Group15News36.txt","Group15News37.txt","Group15News38.txt","Group15News39.txt","Group15News40.txt","Group15News41.txt","Group15News42.txt","Group15News43.txt","Group15News44.txt","Group15News45.txt","Group15News46.txt","Group15News47.txt","Group15News48.txt","Group15News49.txt","Group15News50.txt","Group15News51.txt","Group15News52.txt","Group15News53.txt","Group15News54.txt","Group15News55.txt","Group15News56.txt","Group15News57.txt","Group15News58.txt","Group15News59.txt","Group15News60.txt","Group15News61.txt","Group15News62.txt","Group15News63.txt","Group15News64.txt","Group15News65.txt","Group15News66.txt","Group15News67.txt","Group15News68.txt","Group15News69.txt","Group15News70.txt","Group15News71.txt","Group15News72.txt","Group15News73.txt","Group15News74.txt","Group15News75.txt","Group15News76.txt","Group15News77.txt","Group15News78.txt","Group15News79.txt","Group15News80.txt","Group15News81.txt","Group15News82.txt","Group15News83.txt","Group15News84.txt","Group15News85.txt","Group15News86.txt","Group15News87.txt","Group15News88.txt","Group15News89.txt","Group15News90.txt","Group15News91.txt","Group15News92.txt","Group15News93.txt","Group15News94.txt","Group15News95.txt","Group15News96.txt","Group15News97.txt","Group15News98.txt","Group15News99.txt","Group16News01.txt","Group16News02.txt","Group16News03.txt","Group16News04.txt","Group16News05.txt","Group16News06.txt","Group16News07.txt","Group16News08.txt","Group16News09.txt","Group16News10.txt","Group16News11.txt","Group16News12.txt","Group16News13.txt","Group16News14.txt","Group16News15.txt","Group16News16.txt","Group16News17.txt","Group16News18.txt","Group16News19.txt","Group16News20.txt","Group16News21.txt","Group16News22.txt","Group16News23.txt","Group16News24.txt","Group16News25.txt","Group16News26.txt","Group16News27.txt","Group16News28.txt","Group16News29.txt","Group16News30.txt","Group16News31.txt","Group16News32.txt","Group16News33.txt","Group16News34.txt","Group16News35.txt","Group16News36.txt","Group16News37.txt","Group16News38.txt","Group16News39.txt","Group16News40.txt","Group16News41.txt","Group16News42.txt","Group16News43.txt","Group16News44.txt","Group16News45.txt","Group16News46.txt","Group16News47.txt","Group16News48.txt","Group16News49.txt","Group16News50.txt","Group16News51.txt","Group16News52.txt","Group16News53.txt","Group16News54.txt","Group16News55.txt","Group16News56.txt","Group16News57.txt","Group16News58.txt","Group16News59.txt","Group16News60.txt","Group16News61.txt","Group17News00.txt","Group17News01.txt","Group17News02.txt","Group17News03.txt","Group17News04.txt","Group17News05.txt","Group17News06.txt","Group17News07.txt","Group17News08.txt","Group17News09.txt","Group17News10.txt","Group17News11.txt","Group17News12.txt","Group17News13.txt","Group17News14.txt","Group17News15.txt","Group17News16.txt","Group17News17.txt","Group17News18.txt","Group17News19.txt","Group17News20.txt","Group17News21.txt","Group17News22.txt","Group17News23.txt","Group17News24.txt","Group17News25.txt","Group17News26.txt","Group17News27.txt","Group17News28.txt","Group17News29.txt","Group17News30.txt","Group17News31.txt","Group17News32.txt","Group17News33.txt","Group17News34.txt","Group17News35.txt","Group17News36.txt","Group17News37.txt","Group17News38.txt","Group17News39.txt","Group17News40.txt","Group17News41.txt","Group17News42.txt","Group17News43.txt","Group17News44.txt","Group17News45.txt","Group17News46.txt","Group17News47.txt","Group17News48.txt","Group17News49.txt","Group17News50.txt","Group17News51.txt","Group17News52.txt","Group17News53.txt","Group17News54.txt","Group17News55.txt","Group17News56.txt","Group17News57.txt","Group17News58.txt","Group17News59.txt","Group17News60.txt","Group17News61.txt","Group17News62.txt","Group17News63.txt","Group17News64.txt","Group17News65.txt","Group17News66.txt","Group17News67.txt","Group17News68.txt","Group17News69.txt","Group17News70.txt","Group17News71.txt","Group17News72.txt","Group17News73.txt","Group17News74.txt","Group17News75.txt","Group17News76.txt","Group17News77.txt","Group17News78.txt","Group17News79.txt","Group17News80.txt","Group17News81.txt","Group17News82.txt","Group17News83.txt","Group17News84.txt","Group17News85.txt","Group17News86.txt","Group17News87.txt","Group17News88.txt","Group17News89.txt","Group17News90.txt","Group17News91.txt","Group17News92.txt","Group17News93.txt","Group17News94.txt","Group17News95.txt","Group17News96.txt","Group17News97.txt","Group17News98.txt","Group17News99.txt","Group18News01.txt","Group18News02.txt","Group18News03.txt","Group18News04.txt","Group18News05.txt","Group18News06.txt","Group18News07.txt","Group18News08.txt","Group18News09.txt","Group18News10.txt","Group18News100.txt","Group18News11.txt","Group18News12.txt","Group18News13.txt","Group18News14.txt","Group18News15.txt","Group18News16.txt","Group18News17.txt","Group18News18.txt","Group18News19.txt","Group18News20.txt","Group18News21.txt","Group18News22.txt","Group18News23.txt","Group18News24.txt","Group18News25.txt","Group18News26.txt","Group18News27.txt","Group18News28.txt","Group18News29.txt","Group18News30.txt","Group18News31.txt","Group18News32.txt","Group18News33.txt","Group18News34.txt","Group18News35.txt","Group18News36.txt","Group18News37.txt","Group18News38.txt","Group18News39.txt","Group18News40.txt","Group18News41.txt","Group18News42.txt","Group18News43.txt","Group18News44.txt","Group18News45.txt","Group18News46.txt","Group18News47.txt","Group18News48.txt","Group18News49.txt","Group18News50.txt","Group18News51.txt","Group18News52.txt","Group18News53.txt","Group18News54.txt","Group18News55.txt","Group18News56.txt","Group18News57.txt","Group18News58.txt","Group18News59.txt","Group18News60.txt","Group18News61.txt","Group18News62.txt","Group18News63.txt","Group18News64.txt","Group18News65.txt","Group18News66.txt","Group18News67.txt","Group18News68.txt","Group18News69.txt","Group18News70.txt","Group18News71.txt","Group18News72.txt","Group18News73.txt","Group18News74.txt","Group18News75.txt","Group18News76.txt","Group18News77.txt","Group18News78.txt","Group18News79.txt","Group18News80.txt","Group18News81.txt","Group18News82.txt","Group18News83.txt","Group18News84.txt","Group18News85.txt","Group18News86.txt","Group18News87.txt","Group18News88.txt","Group18News89.txt","Group18News90.txt","Group18News91.txt","Group18News92.txt","Group18News93.txt","Group18News94.txt","Group18News95.txt","Group18News96.txt","Group18News97.txt","Group18News98.txt","Group18News99.txt","Group20News00.txt","Group20News01.txt","Group20News02.txt","Group20News03.txt","Group20News04.txt","Group20News05.txt","Group20News06.txt","Group20News07.txt","Group20News08.txt","Group20News09.txt","Group20News10.txt","Group20News11.txt","Group20News12.txt","Group20News13.txt","Group20News14.txt","Group20News15.txt","Group20News16.txt","Group20News17.txt","Group20News18.txt","Group20News19.txt","Group20News20.txt","Group20News21.txt","Group20News22.txt","Group20News23.txt","Group20News24.txt","Group20News25.txt","Group20News26.txt","Group20News27.txt","Group20News28.txt","Group20News29.txt","Group20News30.txt","Group20News31.txt","Group20News32.txt","Group20News33.txt","Group20News34.txt","Group20News35.txt","Group20News36.txt","Group20News37.txt","Group20News38.txt","Group20News39.txt","Group20News40.txt","Group20News42.txt","Group20News43.txt","Group20News44.txt","Group20News45.txt","Group20News46.txt","Group20News47.txt","Group20News48.txt","Group20News49.txt","Group20News50.txt","Group20News51.txt","Group20News52.txt","Group20News53.txt","Group20News54.txt","Group20News55.txt","Group20News56.txt","Group20News57.txt","Group20News58.txt","Group20News59.txt","Group20News60.txt","Group20News61.txt","Group20News62.txt","Group20News63.txt","Group20News64.txt","Group20News65.txt","Group20News66.txt","Group20News67.txt","Group20News68.txt","Group20News69.txt","Group20News70.txt","Group20News71.txt","Group20News72.txt","Group20News73.txt","Group20News74.txt","Group20News75.txt","Group20News76.txt","Group20News77.txt","Group20News78.txt","Group20News79.txt","Group20News80.txt","Group20News81.txt","Group20News82.txt","Group20News83.txt","Group20News84.txt","Group20News85.txt","Group20News86.txt","Group20News87.txt","Group20News88.txt","Group20News89.txt","Group20News90.txt","Group20News91.txt","Group20News92.txt","Group20News93.txt","Group20News94.txt","Group20News95.txt","Group20News96.txt","Group20News97.txt","Group20News98.txt","Group20News99.txt","Group21News01.txt","Group21News02.txt","Group21News03.txt","Group21News04.txt","Group21News05.txt","Group21News06.txt","Group21News07.txt","Group21News08.txt","Group21News09.txt","Group21News10.txt","Group21News100.txt","Group21News11.txt","Group21News12.txt","Group21News13.txt","Group21News14.txt","Group21News15.txt","Group21News16.txt","Group21News17.txt","Group21News18.txt","Group21News19.txt","Group21News20.txt","Group21News21.txt","Group21News22.txt","Group21News23.txt","Group21News24.txt","Group21News25.txt","Group21News26.txt","Group21News27.txt","Group21News28.txt","Group21News29.txt","Group21News30.txt","Group21News31.txt","Group21News32.txt","Group21News33.txt","Group21News34.txt","Group21News35.txt","Group21News36.txt","Group21News37.txt","Group21News38.txt","Group21News39.txt","Group21News40.txt","Group21News41.txt","Group21News42.txt","Group21News43.txt","Group21News44.txt","Group21News45.txt","Group21News46.txt","Group21News47.txt","Group21News48.txt","Group21News49.txt","Group21News50.txt","Group21News51.txt","Group21News52.txt","Group21News53.txt","Group21News54.txt","Group21News55.txt","Group21News56.txt","Group21News57.txt","Group21News58.txt","Group21News59.txt","Group21News60.txt","Group21News61.txt","Group21News62.txt","Group21News63.txt","Group21News64.txt","Group21News65.txt","Group21News66.txt","Group21News67.txt","Group21News68.txt","Group21News69.txt","Group21News70.txt","Group21News71.txt","Group21News72.txt","Group21News73.txt","Group21News74.txt","Group21News75.txt","Group21News76.txt","Group21News77.txt","Group21News78.txt","Group21News79.txt","Group21News80.txt","Group21News81.txt","Group21News82.txt","Group21News83.txt","Group21News84.txt","Group21News85.txt","Group21News86.txt","Group21News87.txt","Group21News88.txt","Group21News89.txt","Group21News90.txt","Group21News91.txt","Group21News92.txt","Group21News93.txt","Group21News94.txt","Group21News95.txt","Group21News96.txt","Group21News97.txt","Group21News98.txt","Group21News99.txt","Group22News01.txt","Group22News02.txt","Group22News03.txt","Group22News04.txt","Group22News05.txt","Group22News06.txt","Group22News07.txt","Group22News08.txt","Group22News09.txt","Group22News10.txt","Group22News100.txt","Group22News11.txt","Group22News12.txt","Group22News13.txt","Group22News14.txt","Group22News15.txt","Group22News16.txt","Group22News17.txt","Group22News18.txt","Group22News19.txt","Group22News20.txt","Group22News21.txt","Group22News22.txt","Group22News23.txt","Group22News24.txt","Group22News25.txt","Group22News26.txt","Group22News27.txt","Group22News28.txt","Group22News29.txt","Group22News30.txt","Group22News31.txt","Group22News32.txt","Group22News33.txt","Group22News34.txt","Group22News35.txt","Group22News36.txt","Group22News37.txt","Group22News38.txt","Group22News39.txt","Group22News40.txt","Group22News41.txt","Group22News42.txt","Group22News43.txt","Group22News44.txt","Group22News45.txt","Group22News46.txt","Group22News47.txt","Group22News48.txt","Group22News49.txt","Group22News50.txt","Group22News51.txt","Group22News52.txt","Group22News53.txt","Group22News54.txt","Group22News55.txt","Group22News56.txt","Group22News57.txt","Group22News58.txt","Group22News59.txt","Group22News60.txt","Group22News61.txt","Group22News62.txt","Group22News63.txt","Group22News64.txt","Group22News65.txt","Group22News66.txt","Group22News67.txt","Group22News68.txt","Group22News69.txt","Group22News70.txt","Group22News71.txt","Group22News72.txt","Group22News73.txt","Group22News74.txt","Group22News75.txt","Group22News76.txt","Group22News77.txt","Group22News78.txt","Group22News79.txt","Group22News80.txt","Group22News81.txt","Group22News82.txt","Group22News83.txt","Group22News84.txt","Group22News85.txt","Group22News86.txt","Group22News87.txt","Group22News88.txt","Group22News89.txt","Group22News90.txt","Group22News91.txt","Group22News92.txt","Group22News93.txt","Group22News94.txt","Group22News95.txt","Group22News96.txt","Group22News97.txt","Group22News98.txt","Group22News99.txt","Group25News00.txt","Group25News01.txt","Group25News02.txt","Group25News03.txt","Group25News04.txt","Group25News05.txt","Group25News06.txt","Group25News07.txt","Group25News08.txt","Group25News09.txt","Group25News10.txt","Group25News11.txt","Group25News12.txt","Group25News13.txt","Group25News14.txt","Group25News15.txt","Group25News16.txt","Group25News17.txt","Group25News18.txt","Group25News19.txt","Group25News20.txt","Group25News21.txt","Group25News22.txt","Group25News23.txt","Group25News24.txt","Group25News25.txt","Group25News26.txt","Group25News27.txt","Group25News28.txt","Group25News29.txt","Group25News30.txt","Group25News31.txt","Group25News32.txt","Group25News33.txt","Group25News34.txt","Group25News35.txt","Group25News36.txt","Group25News37.txt","Group25News38.txt","Group25News39.txt","Group25News40.txt","Group25News41.txt","Group25News42.txt","Group25News43.txt","Group25News44.txt","Group25News45.txt","Group25News46.txt","Group25News47.txt","Group25News48.txt","Group25News49.txt","Group25News50.txt","Group25News51.txt","Group25News52.txt","Group25News53.txt","Group25News54.txt","Group25News55.txt","Group25News56.txt","Group25News57.txt","Group25News58.txt","Group25News59.txt","Group25News60.txt","Group25News61.txt","Group25News62.txt","Group25News63.txt","Group25News64.txt","Group25News65.txt","Group25News66.txt","Group25News67.txt","Group25News68.txt","Group25News69.txt","Group25News70.txt","Group25News71.txt","Group25News72.txt","Group25News73.txt","Group25News74.txt","Group25News75.txt","Group25News76.txt","Group25News77.txt","Group25News78.txt","Group25News79.txt","Group25News80.txt","Group25News81.txt","Group25News82.txt","Group25News83.txt","Group25News84.txt","Group25News85.txt","Group25News86.txt","Group25News87.txt","Group25News88.txt","Group25News89.txt","Group25News90.txt","Group25News91.txt","Group25News92.txt","Group25News93.txt","Group25News94.txt","Group25News95.txt","Group25News96.txt","Group25News97.txt","Group25News98.txt","Group25News99.txt","test.txt" };
	if (index > MAX_FILE_COUNT) return "";
	return FILENAMES[index - 1];
}