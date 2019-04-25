#pragma once
#include "log.h"
#include <iostream>
#include <time.h>
#include "DB.h"

class Login
{
	/**
	 *	Login类 介绍
	 *	用于创建玩家登陆服务器，需要输入日志指针类型(Log*)
	 *	使用 start() 启动登陆服务器
	 */
private:
	Log *c;
	DB *db;
	SOCKET sock;
	SOCKADDR_IN addrClient;
	int len;
	int port;
	int jieshou(char*);
	int rege(string username, string password, string email, string nickname);
	int rege(string username, string password);
	int logi(string username, string password);

public:
	Login(Config* config, DB *db_in) {
		c = new Log("Login");
		port = config->Read<int>("LOGIN_port");	//加载登陆服务器端口
		db = db_in;
	};
	Login(Log *logout, DB *db_in) {
		c = logout;
		port = 10099;
		db = db_in;
	};
	~Login() {
		
	};

	int start();
	int init();
	

	int end() {
		
		c->out("服务器关闭");
		closesocket(sock);
		WSACleanup();
		return 0;
	};



};
