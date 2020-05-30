#include <iostream>
#include <Windows.h>
#include "clearScreen.h"
#pragma clang diagnostic ignored "-Winvalid-source-encoding"
using namespace std;

void quit()
{
	clearScreen();
	cout << endl;
	cout << " 感谢您使用本程序！本程序还有待改进，若有不足之处请见谅！" << endl;
	cout << " 程序将在不久后关闭！" << endl;
	Sleep(1500);
	exit(0);
}