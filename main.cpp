#include <iostream>
#include <windows.h>
#include <ctime>
#include <string>
#include <conio.h>
#include <vector>
#include <thread>
using namespace std;

struct tm *info; //时间结构体 
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
vector<struct clock>clocks; //闹钟容器 
vector<struct timer>timers;//计时器容器 
time_t t1,t2;//用于检测时间是否变化 

inline void clearScreen() //清屏 
{
    system("cls");
}

inline void Hello()
{
	cout<< "BaiTangClock Ver1.3.0" << endl;
	cout<< "Made by Mo_Ink/白糖洒一地" <<endl;
	cout <<"Github: Mo-Ink" << endl;
	cout <<"Bilibili: 白糖洒一地" << endl;
	cout<< "----未经允许禁止转载----" << endl;
	Sleep(2500);  
}

inline void getTime() //刷新时间函数 
{
	time_t curtime; //现在时间 
	time(&curtime); // 获取时间 
	t1=curtime;
	info = localtime( &curtime ); //结构体赋值
}

inline void setClock() // 闹钟设置函数 
{
	char c=' ';
	clearScreen();
	string s = asctime(info); //时间字符串 
	cout << "当前时间 = " << s;
	cout << "按1设置闹钟，按2删除闹钟，按3清除所有闹钟，按其他键返回" << endl;	//画面输出 
	do
	{
		if(_kbhit())
			c=getch(); //字符读取
	}while(c==' ');
	if(c=='1')
	{
		struct clock tmp; //先输入参数后推入vector 
		int temp=0;//临时计数 
		clearScreen();
		string s = asctime(info); //时间字符串 
		cout << "当前时间 = " << s; 
		cout << "请顺序输入h m s，程序会在h小时m分钟s秒时响铃(24h制)" << endl;   //画面输出 
		do
		{
			if(temp!=0)
				cout << "数据错误，请重新输入" << endl;
			cin >> tmp.h >> tmp.m >> tmp.s;
			temp++;
		}
		while(tmp.h>23||tmp.m>59||tmp.s>59); //若数据错误则重新输入 
		clocks.push_back(tmp);//推入vector 
		cout << "闹钟将在" << tmp.h <<"时 " << tmp.m << "分 " << tmp.s <<"秒时响铃！" <<endl;
		Sleep(1800); //等待2.8s
	}
	if(c=='2'&&!clocks.empty())
	{
		int num,tot=0;
		clearScreen();
		string s = asctime(info); //时间字符串 
		cout << "当前时间 = " << s << endl; //画面输出 
		if(!clocks.empty())
		{
			cout << "闹钟列表：" << endl; //画面输出 
			for(int i=0;i<clocks.size();i++)
				cout << i+1 << ' ' << clocks[i].h << "h " << clocks[i].m << "m " << clocks[i].s << "s " <<endl;
		}
		cout << endl << "请输入要删除的闹钟的编号：" << endl;
		do
		{
			if(tot!=0)
				cout << "数据错误，请重新输入" << endl;
			cin >> num;
			tot++;
		}while(num<=0||num>clocks.size());
		clocks.erase(clocks.begin()+num-1);
		cout << "已删除！" << endl;
		Sleep(1000);
	}
	if(c=='3'&&!clocks.empty())
	{
		clearScreen();
		string s = asctime(info); //时间字符串 
		cout << "当前时间 = " << s << endl; //画面输出 
		clocks.clear();
		cout << "已清除！" << endl;
		Sleep(1000);
	}
}

inline void ringClock(int &x) //响铃函数 
{
	PlaySound("data\ring.wav",NULL,SND_FILENAME|SND_ASYNC|SND_LOOP); //播放声音 
	char c=' ';
	cout << "您设置的时间到了，输入1结束，输入2五分钟后再提醒您。" << endl;
	do
	{
		if(_kbhit())
			c=getch(); //字符读取
	}while(c==' ');
	if(c=='2')
		x=2;
	else
		x=1;
	PlaySound(NULL,NULL,SND_FILENAME|SND_ASYNC|SND_LOOP); //关闭声音 
}

inline void checkClock() //检测闹钟
{
	for(int i=0;i<clocks.size();i++)//遍历整个容器 
		if(clocks[i].h==info->tm_hour&&clocks[i].m==info->tm_min&&clocks[i].s==info->tm_sec)//如果到时间了 
		{
			int tmpin;
			ringClock(tmpin);//响铃 
			if(tmpin==2)//如果选择五分钟后响铃 
			{
				struct clock tmp;
				tmp.h=clocks[i].h;
				tmp.m=clocks[i].m+5;
				tmp.s=clocks[i].s;
				clocks.push_back(tmp);
			}
			clocks.erase(clocks.begin()+i);//删除闹钟 
		}
}

inline void setTimer() // 计时器设置函数 
{
	char c=' ';
	clearScreen();
	string s = asctime(info); //时间字符串 
	cout << "当前时间 = " << s;
	cout << "按1设置计时器，按2删除计时器，按3清除所有计时器，按其他键返回" << endl;	//画面输出 
	do
	{
		if(_kbhit())
			c=getch(); //字符读取
	}while(c==' ');
	if(c=='1')
	{
		struct timer tmp;//先输入参数后推入vector
		int temp=0;
		clearScreen();
		string s = asctime(info); //时间字符串 
		cout << "当前时间 = " << s; 
		cout << "请顺序输入h m s，程序会在h小时m分钟s秒后响铃" << endl; 
		do
		{
			if(temp!=0)
				cout << "数据错误，请重新输入" << endl;
			cin >> tmp.h >> tmp.m >> tmp.s;
			temp++;
		}
		while(tmp.h>23||tmp.m>59||tmp.s>59||(tmp.h+info->tm_hour)>=24);
		tmp.oh=tmp.h+info->tm_hour; 
		tmp.om=tmp.m+info->tm_min;
		tmp.os=tmp.s+info->tm_sec;
		if(tmp.oh>=24) 
			tmp.oh=tmp.oh%24;
		if(tmp.om>=60)
			tmp.om=tmp.om%60;
		if(tmp.os>=60)
			tmp.os=tmp.os%60;
		timers.push_back(tmp);//推入vector 
		cout << "计时器将在" << tmp.h <<"时 " << tmp.m << "分 " << tmp.s <<"秒后响铃！" <<endl;
		Sleep(1800); //等待2.8s
	}
	if(c=='2'&&!timers.empty())
	{
		int num,tot=0;
		clearScreen();
		string s = asctime(info); //时间字符串 
		cout << "当前时间 = " << s << endl; //画面输出 
		if(!timers.empty())
		{
			cout << "计时器列表：" << endl; //画面输出 
			for(int i=0;i<timers.size();i++)
				cout << i+1 << ' ' << timers[i].h << "h " << timers[i].m << "m " << timers[i].s << "s " <<endl;
		}
		cout << endl << "请输入要删除的闹钟的编号：" << endl;
		do
		{
			if(tot!=0)
				cout << "数据错误，请重新输入" << endl;
			cin >> num;
			tot++;
		}while(num<=0||num>timers.size());
		timers.erase(timers.begin()+num-1);
		cout << "已删除！" << endl;
		Sleep(1000);
	}
	if(c=='3'&&!timers.empty())
	{
		clearScreen();
		string s = asctime(info); //时间字符串 
		cout << "当前时间 = " << s << endl; //画面输出 
		timers.clear();
		cout << "已清除！" << endl;
		Sleep(1000);
	}
}

inline void checkTimer() //检测计时器
{
	for(int i=0;i<timers.size();i++)//遍历整个容器 
	{
		if(timers[i].oh==info->tm_hour&&timers[i].om==info->tm_min&&timers[i].os<=info->tm_sec)//如果到时间了 
		{
			int tmpin;
			ringClock(tmpin);//响铃 
			if(tmpin==2)//如果选择五分钟后响铃 
			{
				struct timer tmp;
				tmp.h=0;
				tmp.m=5;
				tmp.s=0;
				tmp.oh=timers[i].h;
				tmp.om=timers[i].m+5;
				tmp.os=timers[i].s;
				timers.push_back(tmp);
			}
			timers.erase(timers.begin()+i);//删除定时器 
		}
		else
		{
			int tmp,tmpo,tmpinfo;
			tmpinfo=info->tm_hour*3600+info->tm_min*60+info->tm_sec;
			tmpo=timers[i].oh*3600+timers[i].om*60+timers[i].os;
			tmp=tmpo-tmpinfo;
			timers[i].h=tmp/3600;
			timers[i].m=tmp/60%60;
			timers[i].s=tmp%60;
		}
	}
}

int main()
{
	SetConsoleTitle("BaiTangClock Ver1.3.0 | Made by Mo_Ink");
	Hello();
	while(true)
	{
		char c = ' ';
		t2=t1;//t1,t2校准 
		getTime(); //获取时间 
		if(t1!=t2) //若t1刷新后于t2不相同则刷新画面 
		{
			clearScreen(); //刷新 
			string s = asctime(info); //时间字符串 
			cout << "当前时间 = " << s;
			cout << "按n可进入闹钟管理！" << endl ;
			cout << "按m可进入计时器管理！" << endl << endl;  
			if(!clocks.empty())
			{
				cout << "闹钟列表：" << endl;
				for(int i=0;i<clocks.size();i++)
					cout << i+1 << ' ' << clocks[i].h << "h " << clocks[i].m << "m " << clocks[i].s << "s " <<endl;
			}
			cout << endl;
			if(!timers.empty())
			{
				cout << "计时器列表：" << endl;
				for(int i=0;i<timers.size();i++)
					cout << i+1 << ' ' << timers[i].h << "h " << timers[i].m << "m " << timers[i].s << "s " <<endl;
			}
		}
		if(_kbhit())
			c=getch(); //字符读取 
		if(c=='n'||c=='N')
			setClock();
		if(c=='m'||c=='M')
			setTimer();
		checkClock();
		checkTimer();
	} 
} 
