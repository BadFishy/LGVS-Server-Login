#pragma comment(lib, "WS2_32")
#include "login.h"


using namespace std;

int Login::init()
{
	//1.启动SOCKET库，版本为2.0
	WORD wVersionRequested;
	WSADATA wsaData;
	int err;
	wVersionRequested = MAKEWORD(2, 0);
	err = WSAStartup(wVersionRequested, &wsaData);
	if (err != 0)
	{
		c->err("Socket2.0初始化失败，Exit!");
		return -1;
	}
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 0)
	{
		WSACleanup();
		return 0;
	}

	//2.创建套接字,用于监听
	//SOCKET sock;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET)
	{
		c->err("Socket 创建失败，Exit!");
		return -1;
	}

	//3.绑定
	SOCKADDR_IN myaddr; //sockaddr_in相当于sockaddr结构
	memset(&myaddr, 0, sizeof(myaddr));//初始化  清零 
	myaddr.sin_family = AF_INET;//只能为这个值
	myaddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);//将u_long型转换为网络字节序列
	myaddr.sin_port = htons(port);// 将u_short型转换为网络字节序列

	bind(sock, (SOCKADDR*)&myaddr, sizeof(myaddr));//绑定套接字

	//4.设置监听
	listen(sock, 5);
	//cout << "开始监听10099端口..." << endl;
	c->out("开始监听10099端口...");

	//5.等待并接受数据
	//SOCKADDR_IN addrClient;
	len = sizeof(SOCKADDR);
}


int Login::start()
{
	

	//while (true) {

		c->out("等待用户连接中...");
		SOCKET sockConnect = accept(sock, (SOCKADDR*)&addrClient, &len);

		Log *login_user = new Log("login", (string)inet_ntoa(addrClient.sin_addr));
		//c->out ("与[" + (string)inet_ntoa(addrClient.sin_addr) + "]建立连接") ;
		login_user->out("建立连接");
		while (true) {


			char sendBuf[100];
			//string   time = getTime();
			//const char* nowtime = time.data();
			sprintf(sendBuf, "LGVS_Login");
			//发送

			if (send(sockConnect, sendBuf, strlen(sendBuf) + 1, 0) == SOCKET_ERROR) {
				login_user->err( "发送失败，可能是客户端已关闭" );
				break;
			}

			//接收
			char recvBuf[100];
			if (recv(sockConnect, recvBuf, 100, 0) == -1)//TCP CLIENT端关闭后，服务器端的recv会一直返回-1，此时如果不退出，服务端recv会一直接收
			{
				login_user->err( "接收失败，可能是客户端已关闭" );
				break;
			}
			else {
				login_user->out("接收到该客户端发送的信息");//aaaaaaa
				jieshou(recvBuf);
			}
			memset(recvBuf, 0, sizeof(recvBuf));//把接收的数据清空



			sprintf(sendBuf, "1");	//登陆成功
			//发送
			if (send(sockConnect, sendBuf, strlen(sendBuf) + 1, 0) == SOCKET_ERROR) {
				login_user->err("发送失败，可能是客户端已关闭");
				break;
			}

			/**
			 *	待添加程序
			 */



			
			break;
		}
		closesocket(sockConnect);
		login_user->out("断开连接");
		delete login_user;
	//}

	//这两个条件
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

	//c->out("服务器关闭");
	//closesocket(sock);
	//WSACleanup();
	return 0;
}


int Login::jieshou(char* s) {
	const char *sep = ","; //分割接收的数据
	char *p;
	string shou[5];
	p = strtok(s, sep);
	shou[0] = p;
	if (shou[0] == "reg") {
		for (int i = 1; i < 3; i++) {
			p = strtok(NULL, sep);
			shou[i] = p;
		}
		c->out("注册账号：" + shou[1] + " 密码：" + shou[2] + " Email：" + shou[3] + " 昵称：" + shou[4]);
		
			Login::rege(shou[1], shou[2]);
		
		
			//Login::rege(shou[1], shou[2], shou[3], shou[4]);
	
	}
	
	else if (shou[0] == "login") {
		for (int i = 1; i < 3; i++) {
			p = strtok(NULL, sep);
			shou[i] = p;
		}
		c->out("登陆账号：" + shou[1] + " 密码：" + shou[2]);
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

