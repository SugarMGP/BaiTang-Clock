#include <iostream>
#include <windows.h>
#include <Mmsystem.h>
#include <conio.h>
#include "resource.h"
#include "clearScreen.h"
#pragma comment(lib,"winmm.lib")
#pragma clang diagnostic ignored "-Winvalid-source-encoding"
using namespace std;

int ring = 1;

#define ringDef MAKEINTRESOURCE(IDR_WAVE1)
#define ringBman MAKEINTRESOURCE(IDR_WAVE2)
#define ringJi MAKEINTRESOURCE(IDR_WAVE3)
#define ringCrp MAKEINTRESOURCE(IDR_WAVE4)

void ringClock(int& x) //响铃函数 
{
	if (ring == 1)
		PlaySound(ringDef, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP); //播放声音 
	if (ring == 2)
		PlaySound(ringBman, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP); //播放声音 
	if (ring == 3)
		PlaySound(ringJi, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP); //播放声音  
	if (ring == 4)
		PlaySound(ringCrp, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP); //播放声音 
	char c = ' ';
	cout << " 您设置的时间到了，输入1结束，输入2将在五分钟后再次提醒您。" << endl;
	do
	{
		if (_kbhit())
			c = _getch(); //字符读取
	} while (c == ' ');
	if (c == '2')
		x = 2;
	else
		x = 1;
	PlaySound(NULL, NULL, SND_ASYNC | SND_LOOP); //关闭声音 
}

void ringTimer() //响铃函数 
{
	if (ring == 1)
		PlaySound(ringDef, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP); //播放声音 
	if (ring == 2)
		PlaySound(ringBman, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP); //播放声音 
	if (ring == 3)
		PlaySound(ringJi, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP); //播放声音  
	if (ring == 4)
		PlaySound(ringCrp, GetModuleHandle(NULL), SND_RESOURCE | SND_ASYNC | SND_LOOP); //播放声音 
	char c = ' ';
	cout << " 您设置的时间到了，输入任意键结束" << endl;
	do
	{
		if (_kbhit())
			c = _getch(); //字符读取
	} while (c == ' ');
	PlaySound(NULL, NULL, SND_ASYNC | SND_LOOP); //关闭声音 
}

void showRing()
{
	cout << " 当前铃声：";
	if (ring == 1)
		cout << "1.默认" << endl;
	if (ring == 2)
		cout << "2.黑人抬棺" << endl;
	if (ring == 3)
		cout << "3.鸡你太美" << endl;
	if (ring == 4)
		cout << "4.Creeper?" << endl;
}

void setRing()
{
	int n, tot = 0;
	clearScreen();
	cout << endl;
	cout << " 可选铃声：" << endl;	//画面输出 
	cout << " 1.默认" << endl;
	cout << " 2.黑人抬棺" << endl;
	cout << " 3.鸡你太美" << endl;
	cout << " 4.Creeper?" << endl << endl;
	cout << " 请输入想要的铃声编号:" << endl;
	do
	{
		if (tot != 0)
			cout << " 数据错误，请重新输入" << endl;
		cin.clear();
		cin.sync();
		cin >> n;
		tot++;
	} while (n != 1 && n != 2 && n != 3 && n != 4);
	ring = n;
}