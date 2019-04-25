#pragma once
#include "log.h"
#include <iostream>
#include <time.h>
#include "DB.h"

class Login
{
	/**
	 *	Login�� ����
	 *	���ڴ�����ҵ�½����������Ҫ������־ָ������(Log*)
	 *	ʹ�� start() ������½������
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
		port = config->Read<int>("LOGIN_port");	//���ص�½�������˿�
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
		
		c->out("�������ر�");
		closesocket(sock);
		WSACleanup();
		return 0;
	};



};
