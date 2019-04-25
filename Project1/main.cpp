#include "Log.h"
#include "Login.h"
#include "DB.h"
#include <thread>
#include <windows.h>
#include <io.h>
#include <fstream>
#include <iostream>
#include <vector>
#include <iterator>

using namespace std;

void start_login_server(Login *);

bool initdb(Log *c, DB* db) {
	if (db->connectDB() == false) {//连接数据库
		return false;
	}

	if (db->runSQL("CREATE TABLE IF NOT EXISTS `USER`("
		"`uid` INT UNSIGNED AUTO_INCREMENT,"
		"`username` VARCHAR(16) NOT NULL,"
		"`password` CHAR(32) NOT NULL,"
		"`nickname` VARCHAR(16),"
		"`email` VARCHAR(50),"
		"`regtime` DATETIME DEFAULT CURRENT_TIMESTAMP,"
		"`lasttime` TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP,"
		"`money` INT(16) NOT NULL DEFAULT 0,"
		"`online` BOOLEAN NOT NULL DEFAULT 0,"
		"`home` INT(16),"
		"`ban` BOOLEAN NOT NULL DEFAULT 0,"
		"PRIMARY KEY(`uid`)"
		")ENGINE = InnoDB DEFAULT CHARSET = utf8;  ") == false) {
		return false;
	}

	//将config.ini中的 INIT = 1
	//config->Add<int>("INIT", 1);
	//ofstream conf_out("config.ini", ios::app);
	//conf_out.open("config.ini", ios::app);
	//if (conf_out.is_open()) {
		// 利用 Config 类的 << 重载运算符
	//	conf_out << config;
	//}
	//conf_out.close();
	{
		vector<string> tmp_files;

		ifstream infile("config.ini");
		if (!infile) {
			c->err("没有找到配置文件");
			return false;
		}

		string lineContent;
		while (getline(infile, lineContent, '\n')) {
			tmp_files.push_back(lineContent + "\n");
		}
		infile.close();

		ofstream outfile("tmp.txt", ios::out);
		vector<string>::iterator siter = tmp_files.begin();

		copy(tmp_files.begin(), tmp_files.end() - 1, ostream_iterator<string>(outfile));
		//cout << "ok!" << endl;
		outfile << "INIT = 1";
		outfile.close();
		system("del config.ini");
		system("rename tmp.txt config.ini");
	}
	
	return true;
}

//2. 开启服务端
int start(Log *c)
{
	
	while (true) {
		
		string cmd;
		
		c->out("-------------!开始运行!--------------");
		c->out("凉糕对战平台（LGVS）服务端启动器");

		c->out("加载配置文件...");
		
		Config* config = new Config("config.ini");	//初始化Config类，加载配置文件

		c->out("连接到数据库...");
		DB* db = new DB(config);	//实例化数据库
		if (db->connectDB() == false) {
			return -1;
		}	//连接到数据库

		if (config->Read<string>("INIT") != "1") {//如果config.ini中的 INIT = 0 则
			c->out("检测到未初始化，开始初始化");
			if (initdb(c, db) == false) {
				c->err("初始化时发生错误！");
				return -1;
			}
			delete config;
			Config* config = new Config("config.ini");
			if (config->Read<std::string>("INIT") == "1") {
				c->out("初始化完成!");
				continue;
			}
			c->err("初始化时发生错误！");
			return -1;
		}


		

		c->out("启用登陆服务器...");
		Login *login_server = new Login(config, db);	//实例化登录服务器
		login_server->init();	//初始化登陆服务器
		thread login_thread(start_login_server, login_server);	//创建登录服务器线程

		while (true) {
			cin >> cmd;
		}
	}

	return 0;
}

//1. 初始化
int init(Log *c) {//初始化
	if (_access(".\\log", 0) == -1) {	//如果目录不存在
		system("mkdir .\\log");
		c->out("检测到没有日志目录，目前已经自动创建...");
		return 1;
	}
	if (_access(".\\config.ini", 0) == -1) {//如果配置文件不存在
		c->err("配置文件不存在！请修改config_origin.ini内的配置后将其更名为config.ini");
		//ofstream confout(".\\config.ini");
		return -1;
	}

	return 0;
}

int main() {
	Log *c = new Log;
	while (true) {

		//1. 初始化
		if (init(c) == -1) {
			system("echo - 按任意键重新开始...");
			system("pause > nul");
		}

		//2. 开启服务端
		int kaishi = start(c);
		if (kaishi == -1) {
			system("echo - 按任意键重新开始...");
			system("pause > nul");
		}
		if (kaishi == 0) {
			system("echo - 按任意键关闭本窗口...");
			system("pause > nul");
			return 0;
		}
	}
	return 0;
}

void start_login_server(Login *login_server) {
	while (true) { login_server->start(); }
}
