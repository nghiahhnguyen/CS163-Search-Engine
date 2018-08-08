#include "Function.h"
#include "FunctionTrang.h"
#include <sys/timeb.h>

int getMilliCount() {
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

int getMilliSpan(int nTimeStart) {
	int nSpan = getMilliCount() - nTimeStart;
	if (nSpan < 0)
		nSpan += 0x100000 * 1000;
	return nSpan;
}

int main() {

	Trie_t stopWord, mainTree;

	mainTree.inputFromFile("CS163-Project-Data");
	mainTree.inputSynonymFromFile();

	char search[50];
	ResizeConsole(1220, 700);
	system("color 70");
	readHistory();
	while (true) {
		tc(112);
		LoadText();
		LoadBorder();
		ready(search);
		// Trang  - Get time every query
		int start = getMilliCount();
		string input(search);
		set<string> highlightWords;
		vector<Node> top5Ranking = mainTree.getQueryData(search, highlightWords);
		int milliSecondsElapsed = getMilliSpan(start);
		GOTOXY(130, 4); cout << milliSecondsElapsed << " ms";

		//Gia - export and display
		string file_path;
		int numPath = 0;
		if (top5Ranking.size() != 0 && top5Ranking.size() <= 5) numPath = (int)top5Ranking.size();
		else if (top5Ranking.size() > 5) numPath = 5;
		if (numPath != 0)
			for (int i = 0; i < numPath; i++)
			{
				file_path = "CS163-Project-Data\\Group07News" + itoXX(top5Ranking[i].file_name) + ".txt";

				ifstream fi;
				fi.open(file_path);

				char title[500];
				fi.get(title, 500, '\n');
				fi.get();


				tc(79);
				GOTOXY(5, 15 + 5 * i); cout << "[" << (char)(i + 65) << "] " << title;
				tc(120); GOTOXY(7, 15 + 5 * i + 1); cout << "File name: " << itoXX(top5Ranking[i].file_name);
				tc(112);

				int trackingPara = 0;
				string paragraph, test[32];
				stringstream strStr, strStr2, strOri;

				strOri = stringstream(search);
				int o = 0;
				while (strOri.good()) {
					strOri >> test[o];
					preprocessing(test[o]);
					o++;
				};

				while (fi.good()) {
					++trackingPara;
					fi >> ws;
					getline(fi, paragraph);
					strStr = stringstream(paragraph);
					strStr2 = stringstream(paragraph);
					if (trackingPara > top5Ranking[i].best_paragraph - 2) break;
				};

				trackingPara = 0;
				int trackingPara2 = 0;
				while (strStr.good())
				{
					string word;
					bool isOK = false;
					strStr >> word;
					trackingPara++;
					preprocessing(word);
					for (int q = 0; q < o; q++)	if (word == test[q]) { isOK = true; break; };
					if (isOK) break;
				};
				for (int j = 0; j < 2; j++)
				{
					GOTOXY(8, 17 + 5 * i + j); if (j == 0) cout << "...";
					while (true)
					{
						string word;
						strStr2 >> word;
						if (trackingPara - 5 < trackingPara2) {
							if (word != " ") {
								string cloneWord;
								cloneWord = word;
								preprocessing(cloneWord);
								if (inKeyy(search, cloneWord) || inKeyyy(cloneWord, search)) {
									tc(139); cout << word;
									tc(112); cout << " ";
								}
								else cout << word << " ";
							}
						}
						trackingPara2++;
						if (trackingPara2 + 5 - trackingPara > 16) { trackingPara2 = 0; break; }
					};
				};
				cout << "...";
				fi.close();
			}
		else
		{
			GOTOXY(7, 15); cout << "Cannot find any article matched with search word";
		}
		GOTOXY(2, 40);
		char c=_getch()-65;
		if (c < 5 && c>=0) {
		ccc;
		int i = c;
		file_path = "CS163-Project-Data\\Group07News" + itoXX(top5Ranking[i].file_name) + ".txt";
		ifstream fi;
		fi.open(file_path);

		char title[500];
		fi.get(title, 500, '\n');
		fi.get();

		tc(79);
		GOTOXY(2, 2); cout << title << endl;
		tc(112);

		int trackingPara = 0;
		string paragraph, test[32];
		stringstream strStr, strStr2, strOri;

		strOri = stringstream(search);
		int o = 0;
		while (strOri.good()) {
		strOri >> test[o];
		preprocessing(test[o]);
		o++;
		};

		while (fi.good()) {
		++trackingPara;
		fi >> ws;
		getline(fi, paragraph);
		strStr = stringstream(paragraph);
		strStr2 = stringstream(paragraph);
		cout << paragraph<<endl;
		};
		fi.close();
		}
		GOTOXY(0, 0);
		_getch();
		ccc;
	}

	return 0;
}