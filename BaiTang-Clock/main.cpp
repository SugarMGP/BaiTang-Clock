#include <iostream>
#include <windows.h>
#include <ctime>
#include <string>
#include <conio.h>
#include <vector>
#include <thread>
#include <time.h>

#include "clearScreen.h"
#include "ring.h"
#include "Hello.h"
#include "smallegg.h"
#include "quit.h"

#pragma clang diagnostic ignored "-Winvalid-source-encoding"

using namespace std;

struct tm info; //时间结构体 
struct clock
{
	int h;
	int m;
	int s;
};
struct timer
{
	int h;
	int m;
	int s;
	int oh; //计时结束的时间 
	int om;
	int os;
};
struct sw //stopwatch
{
	int h;
	int m;
	int s;
	int time;
};
vector<struct clock>clocks; //闹钟vector
vector<struct timer>timers;//计时器vector
vector<struct sw>sws;//秒表分裂vector
struct sw stopwatch;
time_t t1, t2;//用于检测时间是否变化 

inline void getTime() //刷新时间函数 
{
	time_t curtime; //现在时间 
	time(&curtime); // 获取时间 
	t1 = curtime;
	localtime_s(&info, &t1); //结构体赋值
}

inline void setClock() // 闹钟设置函数 
{
	char c = ' ';
	clearScreen();
	char str[26];
	asctime_s(str, sizeof str, &info); //时间字符串 
	cout << " 当前时间 = " << str << endl;
	cout << " 按1设置闹钟，按2删除闹钟，按3清除所有闹钟，按其他键返回" << endl;	//画面输出 
	do
	{
		if (_kbhit())
			c = _getch(); //字符读取
	} while (c == ' ');
	if (c == '1')
	{
		struct clock tmp; //先输入参数后推入vector 
		int temp = 0;//临时计数 
		clearScreen();
		char str[26];
		asctime_s(str, sizeof str, &info); //时间字符串 
		cout << " 当前时间 = " << str << endl;
		cout << " 请顺序输入h m s，程序会在h小时m分钟s秒时响铃(24h制)" << endl;   //画面输出 
		do
		{
			if (temp != 0)
				cout << " 数据错误，请重新输入" << endl;
			cin.clear();
			cin.sync();
			cin >> tmp.h >> tmp.m >> tmp.s;
			temp++;
		} while (tmp.h > 23 || tmp.m > 59 || tmp.s > 59); //若数据错误则重新输入 
		clocks.push_back(tmp);//推入vector 
	}
	if (c == '2' && !clocks.empty())
	{
		int num, tot = 0;
		clearScreen();
		char str[26];
		asctime_s(str, sizeof str, &info); //时间字符串 
		cout << " 当前时间 = " << str << endl;
		if (!clocks.empty())
		{
			cout << " 闹钟列表：" << endl; //画面输出 
			for (int i = 0; i < clocks.size(); i++)
				cout << i + 1 << ' ' << clocks[i].h << "h " << clocks[i].m << "m " << clocks[i].s << "s " << endl;
		}
		cout << endl << " 请输入要删除的闹钟的编号：" << endl;
		do
		{
			if (tot != 0)
				cout << " 数据错误，请重新输入" << endl;
			cin.clear();
			cin.sync();
			cin >> num;
			tot++;
		} while (num <= 0 || num > clocks.size());
		clocks.erase(clocks.begin() + num - 1);
	}
	if (c == '3' && !clocks.empty())
	{
		clearScreen();
		char str[26];
		asctime_s(str, sizeof str, &info); //时间字符串 
		cout << " 当前时间 = " << str << endl;
		clocks.clear();
	}
}

inline void checkClock() //检测闹钟
{
	for (int i = 0; i < clocks.size(); i++)//遍历整个容器 
		if (clocks[i].h == info.tm_hour && clocks[i].m == info.tm_min && clocks[i].s == info.tm_sec)//如果到时间了 
		{
			int tmpin;
			ringClock(tmpin);//响铃 
			if (tmpin == 2)//如果选择五分钟后响铃 
			{
				struct clock tmp;
				tmp.h = clocks[i].h;
				tmp.m = clocks[i].m + 5;
				tmp.s = clocks[i].s;
				clocks.push_back(tmp);
			}
			clocks.erase(clocks.begin() + i);//删除闹钟 
		}
}

inline void setTimer() // 计时器设置函数 
{
	char c = ' ';
	clearScreen();
	getTime();
	char str[26];
	asctime_s(str, sizeof str, &info); //时间字符串 
	cout << " 当前时间 = " << str << endl;
	cout << " 按1设置计时器，按2删除计时器，按3清除所有计时器，按其他键返回" << endl;	//画面输出 
	cout << " 计时器不支持跨天计时，如有需要请使用闹钟功能！" << endl;
	do
	{
		if (_kbhit())
			c = _getch(); //字符读取
	} while (c == ' ');
	if (c == '1')
	{
		struct timer tmp;//先输入参数后推入vector
		int temp = 0;
		clearScreen();
		char str[26];
		asctime_s(str, sizeof str, &info); //时间字符串 
		cout << " 当前时间 = " << str << endl;
		cout << " 请顺序输入h m s，程序会在h小时m分钟s秒后响铃" << endl;
		do
		{
			if (temp != 0)
				cout << " 数据错误，请重新输入" << endl;
			getTime();
			cin.clear();
			cin.sync();
			cin >> tmp.h >> tmp.m >> tmp.s;
			temp++;
		}											//如果计时时间跨天则不通过			
		while (tmp.h > 23 || tmp.m > 59 || tmp.s > 59 || ((info.tm_hour + tmp.h) * 3600 + (info.tm_min + tmp.m) * 60 + (info.tm_sec + tmp.s) >= 86100));
		getTime(); //获取时间 
		tmp.oh = tmp.h + info.tm_hour;
		tmp.om = tmp.m + info.tm_min;
		tmp.os = tmp.s + info.tm_sec; //抵消计算时间 
		if (tmp.oh >= 24)
			tmp.oh = tmp.oh % 24;
		if (tmp.om >= 60)
			tmp.om = tmp.om % 60;
		if (tmp.os >= 60)
			tmp.os = tmp.os % 60;
		timers.push_back(tmp);//推入vector 
	}
	if (c == '2' && !timers.empty())
	{
		int num, tot = 0;
		clearScreen();
		char str[26];
		asctime_s(str, sizeof str, &info); //时间字符串 
		cout << " 当前时间 = " << str << endl;
		if (!timers.empty())
		{
			cout << " 计时器列表：" << endl; //画面输出 
			for (int i = 0; i < timers.size(); i++)
				cout << i + 1 << ' ' << timers[i].h << "h " << timers[i].m << "m " << timers[i].s << "s " << endl;
		}
		cout << endl << " 请输入要删除的闹钟的编号：" << endl;
		do
		{
			if (tot != 0)
				cout << " 数据错误，请重新输入" << endl;
			cin.clear();
			cin.sync();
			cin >> num;
			tot++;
		} while (num <= 0 || num > timers.size());
		timers.erase(timers.begin() + num - 1);
	}
	if (c == '3' && !timers.empty())
	{
		clearScreen();
		char str[26];
		asctime_s(str, sizeof str, &info); //时间字符串 
		cout << " 当前时间 = " << str << endl;
		timers.clear();
	}
}

inline void checkTimer() //检测计时器
{
	for (int i = 0; i < timers.size(); i++)//遍历整个容器 
	{
		if (timers[i].oh * 3600 + timers[i].om * 60 + timers[i].os <= info.tm_hour * 3600 + info.tm_min * 60 + info.tm_sec)//如果到时间了 
		{
			ringTimer();//响铃 
			timers.erase(timers.begin() + i);//删除定时器 
		}
		else
		{
			int tmp, tmpo, tmpinfo;
			tmpinfo = info.tm_hour * 3600 + info.tm_min * 60 + info.tm_sec;
			tmpo = timers[i].oh * 3600 + timers[i].om * 60 + timers[i].os;
			tmp = tmpo - tmpinfo;
			timers[i].h = tmp / 3600;
			timers[i].m = tmp / 60 % 60;
			timers[i].s = tmp % 60;
		}
	}
}

inline void setStopwatch()
{
	clearScreen();
	cout << endl << " 加载中QvQ" << endl;
	bool swon = false;
	stopwatch.h = 0;
	stopwatch.m = 0;
	stopwatch.s = 0;
	stopwatch.time = 0;
	while (true)
	{
		char c = ' ';
		t2 = t1;//t1,t2校准 
		getTime(); //获取时间 
		if (t1 != t2)
		{
			clearScreen();
			char str[26];
			asctime_s(str, sizeof str, &info); //时间字符串 
			cout << " 当前时间 = " << str << endl;
			cout << " 按1开始/暂停，按2分裂，按3清零，按其他键退出" << endl;
			if (swon)
			{
				stopwatch.time++;
				stopwatch.h = stopwatch.time / 3600;
				stopwatch.m = stopwatch.time / 60 % 60;
				stopwatch.s = stopwatch.time % 60;
			}									// 用奇怪的方法实现格式化输入输出 
			cout << " " << stopwatch.h / 10 << stopwatch.h % 10 << ":" << stopwatch.m / 10 << stopwatch.m % 10 << ":" << stopwatch.s / 10 << stopwatch.s % 10 << endl << endl;
			if (!sws.empty())
			{
				cout << " 分裂列表：" << endl;
				for (int i = 0; i < sws.size(); i++) // 用奇怪的方法实现格式化输入输出 
					cout << " " << (i + 1) / 10 << (i + 1) % 10 << " " << sws[i].h / 10 << sws[i].h % 10 << ":" << sws[i].m / 10 << sws[i].m % 10 << ":" << sws[i].s / 10 << sws[i].s % 10 << endl;
			}
			checkClock();
			checkTimer();
		}
		if (_kbhit())
			c = _getch(); //字符读取 
		if (c == '1')
			swon = !swon;
		if (c == '2' && swon)
		{
			struct sw tmp; //先输入参数后推入vector 
			tmp.h = stopwatch.h;
			tmp.m = stopwatch.m;
			tmp.s = stopwatch.s;
			sws.push_back(tmp);//推入vector 
		}
		if (c == '3')
		{
			stopwatch.h = 0;
			stopwatch.m = 0;
			stopwatch.s = 0;
			stopwatch.time = 0;
			sws.clear();
		}
		if (c != '1' && c != '2' && c != '3' && c != ' ')
			break;
		if (sws.size() > 23)
			sws.clear();
	}
}

int main()
{
	ios_base::sync_with_stdio(false);
	cin.tie(0);
	SetConsoleTitle(TEXT("BaiTangClock Ver1.5 | Made by Mo_Ink"));
	Hello();
	while (true)
	{
		char c = ' ';
		t2 = t1;//t1,t2校准 
		getTime(); //获取时间 
		if (t1 != t2) //若t1刷新后于t2不相同则刷新画面 
		{
			clearScreen(); //刷新 
			char str[26];
			asctime_s(str, sizeof str, &info); //时间字符串 
			cout << " 当前时间 = " << str << endl;
			showRing();
			cout << endl;
			cout << " 按N可进入闹钟管理！" << endl;
			cout << " 按M可进入计时器管理！" << endl;
			cout << " 按S可进入秒表管理！" << endl;
			cout << " 按R可进入铃声管理！" << endl;
			cout << " 按Q可退出程序！" << endl << endl;
			if (!clocks.empty())
			{
				cout << " 闹钟列表：" << endl;
				for (int i = 0; i < clocks.size(); i++)
					cout << " " << i + 1 << ' ' << clocks[i].h << "h " << clocks[i].m << "m " << clocks[i].s << "s " << endl;
			}
			cout << endl;
			if (!timers.empty())
			{
				cout << " 计时器列表：" << endl;
				for (int i = 0; i < timers.size(); i++)
					cout << " " << i + 1 << ' ' << timers[i].h << "h " << timers[i].m << "m " << timers[i].s << "s " << endl;
			}
		}
		if (_kbhit())
			c = _getch(); //字符读取 
		if (c == 'n' || c == 'N')
			setClock();
		if (c == 'm' || c == 'M')
			setTimer();
		if (c == 'r' || c == 'R')
			setRing();
		if (c == 's' || c == 'S')
			setStopwatch();
		if (c == 'Q' || c == 'q')
			quit();
		if (c == '~')
			egg();
		checkClock();
		checkTimer();
	}
}
