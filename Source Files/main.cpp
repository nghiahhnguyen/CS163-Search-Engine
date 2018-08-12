//#define _CRT_SECURE_NO_WARNINGS
#include"FunctionTrang.h"

int main() {
	ResizeConsole(1220, 700);
	system("color 70");
	tc(112);
	LoadText();
	LoadBorder();
	readHistory();
	ready();
	system("pause");
}