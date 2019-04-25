#include "DB.h"

bool DB::connectDB()
{
	{
		
		mysql_init(&mysql);
		if (!(mysql_real_connect(&mysql, host.data(), user.data(), psw.data(), table.data(), port, NULL, 0)))
			//�м�ֱ����������û��������룬���ݿ������˿ںţ�����дĬ��0����3306�ȣ���������д�ɲ����ٴ���ȥ  
		{
			
			string buffer = "�������ݿ�ʧ�ܣ�������룺" + (string)mysql_error(&mysql);
			c->err(buffer);
			return false;
		}
		else
		{
			c->out("���ݿ����ӳɹ���");
			return true;
		}

	}
	return false;
}

bool DB::runSQL(const char* query)
{
	if (mysql_query(&mysql, query))        //ִ��SQL���  
	{
		c->err("ѯ��ʧ�ܣ�" + (string)mysql_error(&mysql));
		return false;
	}
	else
	{
		c->out("����ɹ���");
		return true;
	}
	return false;
}
