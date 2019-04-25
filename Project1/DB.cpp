#include "DB.h"

bool DB::connectDB()
{
	{
		
		mysql_init(&mysql);
		if (!(mysql_real_connect(&mysql, host.data(), user.data(), psw.data(), table.data(), port, NULL, 0)))
			//中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去  
		{
			
			string buffer = "连接数据库失败！错误代码：" + (string)mysql_error(&mysql);
			c->err(buffer);
			return false;
		}
		else
		{
			c->out("数据库连接成功！");
			return true;
		}

	}
	return false;
}

bool DB::runSQL(const char* query)
{
	if (mysql_query(&mysql, query))        //执行SQL语句  
	{
		c->err("询问失败！" + (string)mysql_error(&mysql));
		return false;
	}
	else
	{
		c->out("插入成功！");
		return true;
	}
	return false;
}
