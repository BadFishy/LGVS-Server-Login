#pragma comment(lib, "WS2_32")
#include "login.h"


using namespace std;

int Login::init()
{
	//1.����SOCKET�⣬�汾Ϊ2.0
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 0);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		c->err("Socket2.0��ʼ��ʧ�ܣ�Exit!");
		return -1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
	{
		WSACleanup();
		return 0;
	}

	//2.�����׽���,���ڼ���
	//SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		c->err("Socket ����ʧ�ܣ�Exit!");
		return -1;
	}

	//3.��
	SOCKADDR_IN myaddr; //sockaddr_in�൱��sockaddr�ṹ
	memset(&myaddr, 0, sizeof(myaddr));//��ʼ��  ���� 
	myaddr.sin_family = AF_INET;//ֻ��Ϊ���ֵ
	myaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//��u_long��ת��Ϊ�����ֽ�����
	myaddr.sin_port = htons(port);// ��u_short��ת��Ϊ�����ֽ�����

	bind(sock, (SOCKADDR*)&myaddr, sizeof(myaddr));//���׽���

	//4.���ü���
	listen(sock, 5);
	//cout << "��ʼ����10099�˿�..." << endl;
	c->out("��ʼ����10099�˿�...");

	//5.�ȴ�����������
	//SOCKADDR_IN addrClient;
	len = sizeof(SOCKADDR);
}


int Login::start()
{
	

	//while (true) {

		c->out("�ȴ��û�������...");
		SOCKET sockConnect = accept(sock, (SOCKADDR*)&addrClient, &len);

		Log *login_user = new Log("login", (string)inet_ntoa(addrClient.sin_addr));
		//c->out ("��[" + (string)inet_ntoa(addrClient.sin_addr) + "]��������") ;
		login_user->out("��������");
		while (true) {


			char sendBuf[100];
			//string   time = getTime();
			//const char* nowtime = time.data();
			sprintf(sendBuf, "LGVS_Login");
			//����

			if (send(sockConnect, sendBuf, strlen(sendBuf) + 1, 0) == SOCKET_ERROR) {
				login_user->err( "����ʧ�ܣ������ǿͻ����ѹر�" );
				break;
			}

			//����
			char recvBuf[100];
			if (recv(sockConnect, recvBuf, 100, 0) == -1)//TCP CLIENT�˹رպ󣬷������˵�recv��һֱ����-1����ʱ������˳��������recv��һֱ����
			{
				login_user->err( "����ʧ�ܣ������ǿͻ����ѹر�" );
				break;
			}
			else {
				login_user->out("���յ��ÿͻ��˷��͵���Ϣ");//aaaaaaa
				jieshou(recvBuf);
			}
			memset(recvBuf, 0, sizeof(recvBuf));//�ѽ��յ��������



			sprintf(sendBuf, "1");	//��½�ɹ�
			//����
			if (send(sockConnect, sendBuf, strlen(sendBuf) + 1, 0) == SOCKET_ERROR) {
				login_user->err("����ʧ�ܣ������ǿͻ����ѹر�");
				break;
			}

			/**
			 *	����ӳ���
			 */



			
			break;
		}
		closesocket(sockConnect);
		login_user->out("�Ͽ�����");
		delete login_user;
	//}

	//����������
	//if (!closesocket(sock))
	//{
	//	WSAGetLastError();
	//	return 0;
	//}
	//if (!WSACleanup())
	//{
	//	WSAGetLastError();
	//	return 0;
	//}

	//c->out("�������ر�");
	//closesocket(sock);
	//WSACleanup();
	return 0;
}


int Login::jieshou(char* s) {
	const char *sep = ","; //�ָ���յ�����
	char *p;
	string shou[5];
	p = strtok(s, sep);
	shou[0] = p;
	if (shou[0] == "reg") {
		for (int i = 1; i < 3; i++) {
			p = strtok(NULL, sep);
			shou[i] = p;
		}
		c->out("ע���˺ţ�" + shou[1] + " ���룺" + shou[2] + " Email��" + shou[3] + " �ǳƣ�" + shou[4]);
		
			Login::rege(shou[1], shou[2]);
		
		
			//Login::rege(shou[1], shou[2], shou[3], shou[4]);
	
	}
	
	else if (shou[0] == "login") {
		for (int i = 1; i < 3; i++) {
			p = strtok(NULL, sep);
			shou[i] = p;
		}
		c->out("��½�˺ţ�" + shou[1] + " ���룺" + shou[2]);
		Login::logi(shou[0], shou[1]);
	}
	
	return 0;
}

int Login::rege(string username, string password, string email, string nickname)
{
	string query = "INSERT INTO USER (username, password, email, nickname) VALUES ('"
		+ username + "', '" 
		+ password + "', '" 
		+ email + "', '"
		+ nickname + "');";
	db->runSQL(query.data());
	return 0;
}

int Login::rege(string username, string password)
{
	string query = "INSERT INTO USER (username, password) VALUES ('"
		+ username + "', '"
		+ password + "');";
	db->runSQL(query.data());
	return 0;
}

int Login::logi(string username, string password)
{
	return 0;
}

