//============================================================================
// Name        : getcher.cpp
// Author      : wocson
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <termios.h>
#include <unistd.h>

using namespace std;

int getch(void)
{
	struct termios oldt, newt;
	int ch;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

	return ch;
}

int main()
{
	cout << "Input: ";

	int key = getch();

	cout << (char)key << ' ';

	cout << dec << uppercase << key << ' ';
	cout << hex << uppercase << "0x" << key << ' ';
	cout << oct << uppercase << "0" << key << ' ';

	cout << endl;

	return 0;
}
