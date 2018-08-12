#include"FunctionTrang.h"

struct keys
{
	char key[50],keyO[50];
	int rank;
};
int KEYs = 0;

keys ListKey[50];

void ResizeConsole(int w, int h) {
	HWND console = GetConsoleWindow();
	RECT r;
	GetWindowRect(console, &r);
	MoveWindow(console, r.left, r.top, w, h, TRUE);
}

void GOTOXY(int x, int y)
{
	static HANDLE h = NULL;
	if (!h)
		h = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD c = { x, y };
	SetConsoleCursorPosition(h, c);
}

void tsize(int x) {
	CONSOLE_FONT_INFOEX cfi;
	cfi.cbSize = sizeof(cfi);
	cfi.nFont = 0;
	cfi.dwFontSize.X = 0;                   // Width of each character in the font
	cfi.dwFontSize.Y = x;                  // Height
	cfi.FontFamily = FF_DONTCARE;
	cfi.FontWeight = FW_NORMAL;
	SetCurrentConsoleFontEx(GetStdHandle(STD_OUTPUT_HANDLE), FALSE, &cfi);
}

void genKey(char s[]) {
	while (strlen(s) < 49) {
		int tmp = strlen(s);
		s[tmp++] = ' ';
		s[tmp] = '\0';
	}
}

void LoadText() {
	ifstream f;
	ccc;
	tc(116);
	f.open("Graphic\\text.txt");
	char s[110];
	for (int i = 0; i < 8; i++) {
		f.get(s, 110, '\n'); f.ignore(10, '\n');
		cout << s << endl;
	}
	tc(112);
}

void readHistory() {
	char s[50];
	ifstream fi;
	fi.open("Logs\\SearchedKey.txt");
	int dem = -1;
	while (!fi.eof()) {
		fi >> ListKey[++dem].rank; fi.ignore(10, '\n');
		fi.get(s, 50, '\n'); fi.ignore(10, '\n');
		strcpy(ListKey[dem].keyO, s);
		strcpy(ListKey[dem].key, s);
		genKey(ListKey[dem].key);
	}
	KEYs = dem;
	//for (int i = 0; i <= dem; i++) cout << ListKey[i].keyO << " " << ListKey[i].key << endl;
}

void LoadBorder() {
	tc(113);
	GOTOXY(2, 9); for (int i = 0; i < 130; i++) cout << "_"; cout << "   ___________";
	GOTOXY(2, 12); for (int i = 0; i < 130; i++) cout << "_";
	GOTOXY(1, 10); cout << "|";
	GOTOXY(1, 11); cout << "| Type here...";
	GOTOXY(1, 12); cout << "|";
	GOTOXY(132, 10); cout << "|"; cout << "  |         |";
	GOTOXY(132, 11); cout << "|"; cout << "  |  ENTER  |";
	GOTOXY(132, 12); cout << "|"; cout << "  |_________|";

	GOTOXY(115, 2);
	for (int i = 0; i < 31; i++) cout << "-";
	GOTOXY(115, 6);
	for (int i = 0; i < 31; i++) cout << "-";
	for (int i = 3; i < 6; i++) { GOTOXY(115, i); cout << "|                             |"; }
	GOTOXY(117, 4); cout << "Run time: ";

	GOTOXY(1, 14);
	for (int i = 0; i < 145; i++) cout << "-";
	GOTOXY(1, 39);
	for (int i = 0; i < 145; i++) cout << "-";
	for (int i = 14; i < 39; i++) {
		GOTOXY(1, i);
		cout << "|" << endl;
		GOTOXY(1 + 145, i);
		cout << "|" << endl;
	}
}

void AddHistory(char s[]) {
	
	char ss[50];
	strcpy(ss, s);
	genKey(ss);

	int tmp = -1;
	for (int i = 0; i <= KEYs; i++) {
		if (strcmp(ListKey[i].key, ss) == 0) { tmp = i; break; }
	}

	//cout << tmp; p;

	if (tmp == -1) {
		ListKey[++KEYs].rank = 5;
		strcpy(ListKey[KEYs].keyO, s);
		strcpy(ListKey[KEYs].key, ss);
	}
	else {
		ListKey[tmp].rank++;
	}

	for (int i = 0; i <= KEYs; i++) for (int j = 0; j <= KEYs - 1; j++) {
		if (strcmp(ListKey[j].key, ListKey[j + 1].key) > 0) {
			ListKey[KEYs+1] = ListKey[j];
			ListKey[j] = ListKey[j + 1];
			ListKey[j + 1] = ListKey[KEYs+1];
		}
	}

	for (int i = 0; i <= KEYs; i++) for (int j = 0; j <= KEYs - 1; j++) {
		if (ListKey[j].rank < ListKey[j + 1].rank) {
			ListKey[KEYs + 1] = ListKey[j];
			ListKey[j] = ListKey[j + 1];
			ListKey[j + 1] = ListKey[KEYs + 1];
		}
	}

	ofstream fo;
	fo.open("Logs\\SearchedKey.txt");
	for (int i = 0; i < KEYs; i++) fo << ListKey[i].rank << endl << ListKey[i].keyO << endl;
	fo << ListKey[KEYs].rank << endl << ListKey[KEYs].keyO; fo.close();
	//for (int i = 0; i <= KEYs; i++) cout << ListKey[i].rank << " " << ListKey[i].keyO << endl;
}

bool inKey(char s[], char ss[], int &i) {
	if (strlen(s) > strlen(ss)) return false;
	int tmp = 0;
	for (i = 0; i < strlen(ss); i++) {
		if (ss[i] == s[tmp]) tmp++; else { tmp = 0; if (ss[i] == s[tmp]) tmp++;	}
		if (tmp == strlen(s)) return true;
	}
	return false;
}
bool inKeyy(char s[], string ss) {
	int i;
	if (strlen(s) > ss.length()) return false;
	int tmp = 0;
	for (i = 0; i < ss.length(); i++) {
		if (tolower(ss[i]) == tolower(s[tmp])) tmp++; else { tmp = 0; if (ss[i] == s[tmp]) tmp++; }
		if (tmp == strlen(s)) return true;
	}
	return false;
}
bool inKeyyy(string ss, char s[]) {
	int i;
	if (strlen(s) < ss.length()) return false;
	int tmp = 0;
	for (i = 0; i < strlen(s); i++) {
		if (tolower(ss[tmp]) == tolower(s[i])) tmp++; else { tmp = 0; if (ss[tmp] == s[i]) tmp++; }
		if (tmp == ss.length() && (s[i+1]==' ' || s[i+1]=='\0')) return true;
	}
	return false;
}


void preSearch() {

	for (int i = 13; i < 39; i++) {
		GOTOXY(2, i);
		cout << "                                                                                                                                                ";
	}
	GOTOXY(1, 14);
	for (int i = 0; i < 145; i++) cout << "-";
}

void KeySelect(char s[]) {
	preSearch();
	int selected[500];
	for (int i = 0; i < 50; i++) selected[i] = -1;
	for (int i = 0; i <= KEYs; i++) {
		int index = -1;
		if (inKey(s, ListKey[i].keyO, index))
		{
			if (index < strlen(ListKey[i].keyO)) selected[i] = index-strlen(s)+1;
		}
	}

	int count = 0;
	for (int i = 0; i <= KEYs; i++) {
		if (selected[i] == 0 ) {
			count++; tc(112);
			GOTOXY(2, 12 + count); cout << "|                                                                       |";
			GOTOXY(3, 12 + count); cout << ListKey[i].keyO;
			GOTOXY(3 + selected[i], 12 + count); tc(116); cout << s;
		}
	}
	for (int i = 0; i <= KEYs; i++) {
		if (count == 15) break;
		if (selected[i] > 0) {
			count++; 
			tc(112);
			GOTOXY(2, 12 + count); cout << "|                                                                       |";
			GOTOXY(3, 12 + count); cout << ListKey[i].keyO;
			GOTOXY(3 + selected[i], 12 + count); tc(116); cout << s;
		}
	}
	if (count > 0) {
		GOTOXY(2, count + 13); tc(112); cout << "-------------------------------------------------------------------------";
	}
}

void ready(char (&s)[50]) {
	GOTOXY(2, 40); cout << "                                                                           ";
	GOTOXY(3, 11); cout << "                                                                           ";
	GOTOXY(3,11); 
	int i;
	tc(112);
	inputUName(3, 11, s);
	AddHistory(s);
	preSearch();
}

void inputUName(int x, int y, char(&s)[50]) {
	s[0] = '\0'; int dem = -1;
	char c = 0;
	c = _getch();
	while (c != '~' && c != 13) {
		if (c == 33) exit(0);
		if (c == 8) {
			if (dem > -1) {
				s[dem] = '\0';
				dem--;
				KeySelect(s);
				GOTOXY(x + dem + 1, y); 
				if (dem < 0) {
					cout << "Type here...";
					preSearch();
				}
				cout << " ";
				GOTOXY(x + dem + 1, y);
			}
		}
		else {
			cout << "            ";
			dem++;
			s[dem] = c; s[dem + 1] = '\0';
			KeySelect(s);
			GOTOXY(x + dem, y); cout << c;
		}
		c = _getch();
	}
	if (dem == -1) inputUName(x, y, s);
}