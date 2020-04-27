#include <iostream>
#include <windows.h>
#include <ctime>
#include "clearScreen.h"
#pragma clang diagnostic ignored "-Winvalid-source-encoding"
using namespace std;

void egg()
{
	clearScreen();
	srand((int)time(0));
	SetConsoleTitle(TEXT("Boooooooom! BUG++"));
	for (int i = 1; i <= 2000; i++)
		cout << " 数据错误，请重新输入" << endl;
	for (int i = 1; i <= 1500; i++)
		cout << "ERROR!" << endl;

	int* p = NULL;
	int tmp = 0;
	for (int i = 1;; i++)
	{
		p++;
		cout << p;
		if (i % 5 == 0)
			cout << "鍝斿澂";
		if (i % 3 == 0)
			cout << "鐧界硸娲洿浣?";
		if (i % 8 == 0)
			cout << " $据$%^&误#)新*#%输入";
		if (i % rand() == 0)
			cout << endl;
		if (i % 34 == 0 && tmp < 4)
		{
			Sleep((rand() + 100) % 1500);
			tmp++;
		}
	}
	exit(0);
}