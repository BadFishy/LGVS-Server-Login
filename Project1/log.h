#pragma once
#pragma comment(lib, "WS2_32")
#include <string>
#include <winsock2.h>
#include <iostream>
#include <time.h>
#include <fstream>
#include "Config.h"

using namespace std;

class Log
{
	/**
	 *	Log类 介绍
	 *	用于在控制台输出日志，并输出到日志文件（LGVS.log）中
	 *	创建类型为Log的对象时，需要设定日志文件名参数，如 Log abc（"filename"）
	 *	使用 out("文本内容") 输出正常日志
	 *	使用 err("文本内容") 输出错误日志
	 */
private:

	string filepath;
	string erropath;
	string qz;

	string getTime()
	{
		time_t timep;
		time(&timep);
		char tmp[64];
		strftime(tmp, sizeof(tmp), "%Y-%m-%d %H:%M:%S", localtime(&timep));
		return tmp;
	}

public:
	Log(string filename ="LGVS",string ip = "") {
		string logpath = ".\\log\\" + filename;
		filepath = logpath;
		filepath += ".log";
		erropath = logpath;
		erropath += "_error.log";
		if (ip.length() != 0) {
			qz = "[";
			qz += (ip+"]");
		}
		else qz = ip;
	};

	int out(string str) {
		string nowtime = getTime();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_GREEN);//设置绿色
		ofstream file(filepath, ios::app);
		file << "[" << nowtime << "]" << qz << str << endl;
		cout << "[" << nowtime << "]" << qz << str << " [" << filepath << "]" << endl;
		return 0;
	};

	int err(string str) {
		string nowtime = getTime();
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);//设置红色
		ofstream file(filepath, ios::app);
		ofstream erro(erropath, ios::app);
		file << "[" << nowtime << "]" << qz << "Error:" << str << endl;
		erro << "[" << nowtime << "]" << qz << "Error:" << str << endl;
		cout << "[" << nowtime << "]" << qz << "Error:" << str << " [" << filepath << "]" << endl;
		return 0;
	};
};
