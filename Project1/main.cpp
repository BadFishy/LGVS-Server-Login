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
	if (db->connectDB() == false) {//�������ݿ�
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

	//��config.ini�е� INIT = 1
	//config->Add<int>("INIT", 1);
	//ofstream conf_out("config.ini", ios::app);
	//conf_out.open("config.ini", ios::app);
	//if (conf_out.is_open()) {
		// ���� Config ��� << ���������
	//	conf_out << config;
	//}
	//conf_out.close();
	{
		vector<string> tmp_files;

		ifstream infile("config.ini");
		if (!infile) {
			c->err("û���ҵ������ļ�");
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

//2. ���������
int start(Log *c)
{
	
	while (true) {
		
		string cmd;
		
		c->out("-------------!��ʼ����!--------------");
		c->out("�����սƽ̨��LGVS�������������");

		c->out("���������ļ�...");
		
		Config* config = new Config("config.ini");	//��ʼ��Config�࣬���������ļ�

		c->out("���ӵ����ݿ�...");
		DB* db = new DB(config);	//ʵ�������ݿ�
		if (db->connectDB() == false) {
			return -1;
		}	//���ӵ����ݿ�

		if (config->Read<string>("INIT") != "1") {//���config.ini�е� INIT = 0 ��
			c->out("��⵽δ��ʼ������ʼ��ʼ��");
			if (initdb(c, db) == false) {
				c->err("��ʼ��ʱ��������");
				return -1;
			}
			delete config;
			Config* config = new Config("config.ini");
			if (config->Read<std::string>("INIT") == "1") {
				c->out("��ʼ�����!");
				continue;
			}
			c->err("��ʼ��ʱ��������");
			return -1;
		}


		

		c->out("���õ�½������...");
		Login *login_server = new Login(config, db);	//ʵ������¼������
		login_server->init();	//��ʼ����½������
		thread login_thread(start_login_server, login_server);	//������¼�������߳�

		while (true) {
			cin >> cmd;
		}
	}

	return 0;
}

//1. ��ʼ��
int init(Log *c) {//��ʼ��
	if (_access(".\\log", 0) == -1) {	//���Ŀ¼������
		system("mkdir .\\log");
		c->out("��⵽û����־Ŀ¼��Ŀǰ�Ѿ��Զ�����...");
		return 1;
	}
	if (_access(".\\config.ini", 0) == -1) {//��������ļ�������
		c->err("�����ļ������ڣ����޸�config_origin.ini�ڵ����ú������Ϊconfig.ini");
		//ofstream confout(".\\config.ini");
		return -1;
	}

	return 0;
}

int main() {
	Log *c = new Log;
	while (true) {

		//1. ��ʼ��
		if (init(c) == -1) {
			system("echo - ����������¿�ʼ...");
			system("pause > nul");
		}

		//2. ���������
		int kaishi = start(c);
		if (kaishi == -1) {
			system("echo - ����������¿�ʼ...");
			system("pause > nul");
		}
		if (kaishi == 0) {
			system("echo - ��������رձ�����...");
			system("pause > nul");
			return 0;
		}
	}
	return 0;
}

void start_login_server(Login *login_server) {
	while (true) { login_server->start(); }
}
