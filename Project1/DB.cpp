#include "DB.h"

bool DB::connectDB()
{
	while (dbing) {
		Sleep(10); 
	}
	dbing = true;
		
		mysql_init(&mysql);
		if (!(mysql_real_connect(&mysql, host.data(), user.data(), psw.data(), table.data(), port, NULL, 0)))
			//中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去  
		{
			
			string buffer = "连接数据库失败！错误代码：" + (string)mysql_error(&mysql);
			c->err(buffer);
			dbing = false;
			return false;
		}
		else
		{
			c->out("数据库连接成功！");
			dbing = false;
			return true;
		}

	dbing = false;
	return false;
}

bool DB::runSQL(const char* query)
{
	while (dbing) {
		Sleep(10);
	}
	dbing = true;

	if (mysql_query(&mysql, query))        //执行SQL语句  
	{
		c->err("询问失败！" + (string)mysql_error(&mysql));
		dbing = false;
		return false;
	}
	else
	{
		c->out("插入成功！");
		dbing = false;
		return true;
	}
	dbing = false;
	return false;
}

//查询数据
bool DB::cunzaiDB(string biao, string lie, string zhi)
{
	while (dbing) {
		Sleep(10);
	}
	dbing = true;

	string que = "select * from "
		+ biao +" where "+ lie +" = '"
		+ zhi + "'";
	int res = mysql_query(&mysql, que.data());
	if (res == 0)
	{
		
		MYSQL_RES *result = mysql_store_result(&mysql);
		my_ulonglong rowCount = mysql_num_rows(result);
		if (rowCount == 0)
		{
			dbing = false;
			return false;//不存在
		}
		else
		{
			dbing = false;
			return true;//存在
		}
	}
	dbing = false;
	return false;
}

bool DB::tongshicunzaiDB(string biao, string lie1, string zhi1, string lie2, string zhi2)
{
	while (dbing) {
		Sleep(10);
	}
	dbing = true;

	string que = "select * from "
		+ biao + " where "
		+ lie1 + " = '"
		+ zhi1 + "' and "
		+ lie2 + " = '"
		+ zhi2 + "'";
	int res = mysql_query(&mysql, que.data());
	if (res == 0)
	{

		MYSQL_RES *result = mysql_store_result(&mysql);
		my_ulonglong rowCount = mysql_num_rows(result);
		if (rowCount == 0)
		{
			dbing = false;
			return false;//不存在
		}
		else
		{
			dbing = false;
			return true;//存在
		}
	}
	dbing = false;
	return false;
}

//查询数据
bool DB::QueryDatabase(const char* q) {
	while (dbing) {
		Sleep(10);
	}
	dbing = true;

	char query[150];    //查询语句
	//将数据格式化输出到字符串
	sprintf_s(query, "select * from %s", q);
	//设置编码格式
	mysql_query(&mysql, "set names gbk");

	if (mysql_query(&mysql, query)) {
		printf("Query failed (%s)\n", mysql_error(&mysql));
		dbing = false;
		return false;
	}
	else {
		printf("query success\n");
	}

	res = mysql_store_result(&mysql);
	if (!res) {
		printf("Couldn't get result from %s\n", mysql_error(&mysql));
		dbing = false;
		return false;
	}

	printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));

	// 获取列数
	int j = mysql_num_fields(res);

	//存储字段信息
	char *str_field[32];

	//获取字段名
	for (int i = 0; i < j; i++) {
		str_field[i] = mysql_fetch_field(res)->name;
	}

	//打印字段
	for (int i = 0; i < j; i++)
		printf("%10s\t", str_field[i]);
	printf("\n");

	//打印查询结果
	//MYSQL_ROW mysql_fetch_row(MYSQL_RES *result)
	//Fetches the next row from the result set
	while (column = mysql_fetch_row(res)) {
		for (int i = 0; i < j; i++)
			printf("%10s\t", column[i]);
		printf("\n");
	}
	dbing = false;
	return true;
}

string DB::cha(const char* q)
{
	while (dbing) {
		Sleep(10);
	}
	dbing = true;

	char query[150];    //查询语句
//将数据格式化输出到字符串
	sprintf_s(query, "select * from %s", q);
	//设置编码格式
	mysql_query(&mysql, "set names gbk");

	if (mysql_query(&mysql, query)) {
		//printf("Query failed (%s)\n", mysql_error(&mysql));
		dbing = false;
		return false;
	}
	else {
		//printf("query success\n");
	}

	res = mysql_store_result(&mysql);
	if (!res) {
		//printf("Couldn't get result from %s\n", mysql_error(&mysql));
		dbing = false;
		return false;
	}

	//printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));

	// 获取列数
	int j = mysql_num_fields(res);
	
	int hang = 0;
	// 获取行数
	string bak = std::to_string(mysql_num_rows(res));
	while (column = mysql_fetch_row(res)) {
		for (int i = 0; i < j; i++)
			bak += "," + (string)column[i];
	}
	dbing = false;
	return bak;
}

string DB::sou(const char* q)
{
	while (dbing) {
		Sleep(10);
	}
	dbing = true;

	char query[150];    //查询语句
//将数据格式化输出到字符串
	sprintf_s(query, "%s", q);
	//设置编码格式
	mysql_query(&mysql, "set names gbk");

	if (mysql_query(&mysql, query)) {
		//printf("Query failed (%s)\n", mysql_error(&mysql));
		dbing = false;
		return false;
	}
	else {
		//printf("query success\n");
	}

	res = mysql_store_result(&mysql);
	if (!res) {
		//printf("Couldn't get result from %s\n", mysql_error(&mysql));
		dbing = false;
		return false;
	}

	//printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));

	// 获取列数
	int j = mysql_num_fields(res);

	int hang = 0;
	// 获取行数
	string bak = std::to_string(mysql_num_rows(res));
	while (column = mysql_fetch_row(res)) {
		for (int i = 0; i < j; i++)
			bak += "," + (string)column[i];
	}
	dbing = false;
	return bak;
}

string DB::sou_no_hang(const char* q)
{
	while (dbing) {
		Sleep(10);
	}
	dbing = true;

	char query[150];    //查询语句
//将数据格式化输出到字符串
	sprintf_s(query, "%s", q);
	//设置编码格式
	mysql_query(&mysql, "set names gbk");

	if (mysql_query(&mysql, query)) {
		//printf("Query failed (%s)\n", mysql_error(&mysql));
		dbing = false;
		return false;
	}
	else {
		//printf("query success\n");
	}

	res = mysql_store_result(&mysql);
	if (!res) {
		//printf("Couldn't get result from %s\n", mysql_error(&mysql));
		dbing = false;
		return false;
	}

	//printf("number of dataline returned: %d\n", mysql_affected_rows(&mysql));

	// 获取列数
	int j = mysql_num_fields(res);

	int hang = 0;
	// 获取行数
	string bak;
	while (column = mysql_fetch_row(res)) {
		bak = column[0];
		for (int i = 1; i < j; i++)
			bak += "," + (string)column[i];
	}
	dbing = false;
	return bak;
}