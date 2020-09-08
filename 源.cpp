#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <winsock.h>
#include <mysql.h> 
#include <string.h>
#include <time.h>
#include <conio.h>
#pragma comment (lib, "libmysql.lib")
//==========================================
//程序启动部分功能

void config_init();				// 若配置文件存在则读取，否则进行首次运行配置程序
void readCFG();					
void sql_connect();
void table_init();
int main_entrance();
//==========================================
//学生功能

void student_login();						// 学生注册模块
void student_mainmenu();					// 学生主菜单
void student_register();					// 学生注册模块
void student_select_course();				// 学生选课模块
void student_query_course();				// 学生查课模块
void student_query_result();				// 学生查询选课结果
void student_delete_course();				// 学生删课模块
void student_manage_info();					// 学生个人信息管理模块
void student_search_specific_imformation();	// 学生查询课程详细信息
int student_check_class_exist(char* );		// 判断课程是否存在

//==========================================
//教师功能

void teacher_login();						// 教师登录模块
void teacher_reg();							// 教师注册模块
void teacher_mainmenu();					// 教师主菜单-3个大功能选单
void teacher_select_managemenu();			// 选课管理选单-5个小功能
void teacher_mycourse();					// 查询查询教师自己开设过的课程的选课情况
void teacher_findcourse();					// 查询选择某门课程的学生信息
void teacher_30delete();					// 选课人数少于30则删除
void teacher_totalcourse();					// 统计选课信息
void teacher_sortcourse();					// 排序选课信息
void teacher_course_managemenu();			// 课程管理选单-4个小功能
void teacher_course_list();					// 课程查询详情
void teacher_course_add();					// 加课
void teacher_course_edit();					// 改课选单
void teacher_course_delete();				// 未开课前删课
void teacher_manage_info();					// 改信息

//==========================================
//公共功能
//用法、输入输出格式见函数定义前的详细注释

DWORD WINAPI SelectEventThread(LPVOID pM);
void print_class(char*);
int check_stuId(char* );
int check_phone(char* );
int check_email(char* );
int check_classId(char* );
int check_classClash(char*);
int getState_selecting();										// 获取选课状态 0为未开始选课，1为正在选课时间内，2为选课时间已结束
int getState_starting(char*, char*);							// 获取开课状态 0为未开课，1为已开课
int check_password(int, char*, char*);							// 第一个参数学生为0，教师为1；登录失败返回0，成功返回1
int check_teachId(char*);
int check_timeClash(char*, char*, char*, char*, char*, char*);
time_t convert_dateToTT(int, int, int, int, int, int);
int scanf_pw(char*);
void pw_encode(char* );
void pw_decode(char* );
int scanf_opt(int*, int, int);
char* s_gets(char*, int);										 
void change_color(int, int);									// 快捷更改接下来输出信息的颜色
//==========================================
//全局变量声明

MYSQL mysql;										// 全局mysql连接
MYSQL_RES* result;									// 查询返回结果集
MYSQL_FIELD* field;									// 结果集取出列存放
MYSQL_ROW Row;	// 结果集取出行存放

char stuID[11];	
char teachID[20];
char nowName[20], nowSchool[20];			//登录进来存放自己的id、名字和学院方便后续使用

char dbIP[50], dbUser[50], dbPassWd[50], dbName[50];
int dbPort = 3306;										//数据库连接信息

int currYear = 2020, currTerm = 1;			// 当前上课学期
time_t currStart, selecStart, selecEnd;		// 当前学期开课时间、选课始终时间

HANDLE consoleHWnd;							//更改颜色需要用到的窗口句柄

char pos[100]; // 8,2-13|11,2-13|15,2-17|16,2-17|17,2-17|19,2-17

int main()
{
	mysql_init(&mysql);		// 初始化mysql

	consoleHWnd = GetStdHandle(STD_OUTPUT_HANDLE);//初始化更改颜色
	system("color e1");

	config_init();

	sql_connect();

	main_entrance();

	return 0;
}


int main_entrance()
{
	HANDLE handle;
	do {
		int option;
		system("cls");
		system("title 学生选课管理系统 - 入口");
		change_color(5, 14);
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t\t○●○●○● 欢迎登录学生选课管理系统 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		change_color(1, 14);
		printf("\n\t\t\t\t\t\t请选择您的身份:\n\n");
		printf("\t\t\t\t\t  ┏━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		printf("\t\t\t\t\t  ┃       ① - 学生       ┃\n");
		printf("\t\t\t\t\t  ┗━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
		printf("\t\t\t\t\t  ┏━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		printf("\t\t\t\t\t  ┃       ② - 老师       ┃\n");
		printf("\t\t\t\t\t  ┗━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
		printf("\t\t\t\t\t  ┏━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		printf("\t\t\t\t\t  ┃     ③ - 退出程序     ┃\n");
		printf("\t\t\t\t\t  ┗━━━━━━━━━━━━━━━━━━━━━━━┛\n\n\n");
		printf("\t\t\t\t\t 请输入1,2,3或直接点击相应标题:");

		sprintf(pos, "6-8,44-67|10-12,44-67|14-16,44-67|1--1,2-17|1--1,2-17|1--1,2-17");
		handle = CreateThread(NULL, 0, SelectEventThread, NULL, 0, NULL);

		scanf_opt(&option, 1, 3);


		TerminateThread(handle,1);

		switch (option)
		{
		case 1:
			student_login();
			break;
		case 2:
			teacher_login();
			break;
		case 3:
			mysql_close(&mysql);		//正常退出，断开数据库连接
			exit(0);
		}
	} while (1);
}

void student_login()
{
	HANDLE handle;
	do {
		system("cls");
		int option1, flag;
		system("title 学生选课管理系统 - 学生登录");
		change_color(5, 14);
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t\t○●○●○● 欢迎登录学生选课管理系统 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		change_color(1, 14);
		printf("\n\t\t\t\t\t\t请选择您的身份:\n\n");
		printf("\t\t\t\t\t  ┏━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		printf("\t\t\t\t\t  ┃       ① - 登录       ┃\n");
		printf("\t\t\t\t\t  ┗━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
		printf("\t\t\t\t\t  ┏━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		printf("\t\t\t\t\t  ┃       ② - 注册       ┃\n");
		printf("\t\t\t\t\t  ┗━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
		printf("\t\t\t\t\t  ┏━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		printf("\t\t\t\t\t  ┃     ③ - 返回上层     ┃\n");
		printf("\t\t\t\t\t  ┗━━━━━━━━━━━━━━━━━━━━━━━┛\n\n\n");
		printf("\t\t\t\t\t 请输入1,2,3或直接点击相应标题:");

		sprintf(pos, "6-8,44-67|10-12,44-67|14-16,44-67|1--1,2-17|1--1,2-17|1--1,2-17");

		handle = CreateThread(NULL, 0, SelectEventThread, NULL, 0, NULL);

		scanf_opt(&option1, 1, 3);

		TerminateThread(handle, 1);

		if (option1 == 1)
		{
			system("cls");
			char stu_passwd[100];
			change_color(5, 14);
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\t\t\t\t○●○●○● 登陆界面--学生 ●○●○●○\n");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			do
			{
				flag = 0;
				change_color(1, 14);
				printf("\n请输入学号：");

				s_gets(stuID, 11);
				if (stuID[0] == 17)//输入ctrl+q返回上一级
				{
					system("cls");
					student_login();
					return;
				}
				if (check_stuId(stuID) == 0)
				{
					change_color(4, 14);
					printf("无效输入！请输入10位数字(若返回上一级，请按Ctrl+Q后回车)\n");
					flag = 1;
					continue;
				}
				char query[100] = "select name,school from students where stuID='";
				strcat(query, stuID);
				strcat(query, "'");
				mysql_store_result(&mysql);
				mysql_query(&mysql, query);
				result = mysql_store_result(&mysql);
				if ((int)mysql_num_rows(result) != 0)
				{
					Row = mysql_fetch_row(result);
					sprintf(nowName, Row[0]);
					sprintf(nowSchool, Row[1]);
				}

				if ((int)mysql_num_rows(result) == 0)
				{
					change_color(4, 14);
					printf("此学号未注册，请重新输入！(若返回上一级，请按Ctrl+Q后回车)\n");
					flag = 1;
				}
			} while (flag);

			change_color(1, 14);
			printf("请输入密码：");
			scanf_pw(stu_passwd);
			while (check_password(0, stuID, stu_passwd) == 0)
			{
				change_color(4, 14);
				printf("密码错误!请重新输入密码：(若返回上一级，输入ctrl+q)");
				change_color(1, 14);
				s_gets(stu_passwd, 20);	//若首次密码输入错误则明文显示
				if (stu_passwd[0] == 17)//返回上一级
				{
					system("cls");
					student_login();
					return;
				}
			}
			student_mainmenu();
			continue;
		}
		else if (option1 == 2)
		{
			student_register();
			continue;
		}
		else if (option1 == 3)
			return;
	} while (1);
}

int check_stuId(char* str)
{
	int b = 1, y = 0;
	while (str[y] != '\0')
		y++;
	if (y != 10)
		b = 0;
	for (int y = 0; y < 10; y++)
	{
		if (str[y] < '0' || str[y]>'9')
			b = 0;
	}
	return b;
}

void sql_connect()
{
	change_color(4, 14);
	if (!(mysql_real_connect(&mysql, dbIP, dbUser, dbPassWd
		, dbName, dbPort, NULL, 0))) {
		printf("无法连接到数据库，错误代码: %s\n\n请检查配置正确性，或考虑删除配置文件进行重新初始化...\n按任意键退出……", mysql_error(&mysql));
		system("pause>nul");
		exit(1);
	}
	if (!mysql_set_character_set(&mysql, "gbk"))
		mysql_character_set_name(&mysql);
	if (mysql_query(&mysql, "select name from `teachers`"))
	{
		printf("\n教师数据表查询失败！请确认数据表是否存在，或考虑删除配置文件进行重新初始化...\n\n按任意键退出……");
		system("pause>nul");
		exit(1);
	}
	mysql_store_result(&mysql);		//取走测试连接存储下来的数据
	if (mysql_query(&mysql, "select name from `students`"))
	{
		printf("\n学生数据表查询失败！请确认数据表是否存在，或考虑删除配置文件进行重新初始化...\n\n按任意键退出……");
		system("pause>nul");
		exit(1);
	}
	mysql_store_result(&mysql);		//取走测试连接存储下来的数据
	if (mysql_query(&mysql, "select 学分 from `classes`"))
	{
		printf("\n课程数据表查询失败！请确认数据表是否存在，或考虑删除配置文件进行重新初始化...\n\n按任意键退出……");
		system("pause>nul");
		exit(1);
	}
	mysql_store_result(&mysql);		//取走测试连接存储下来的数据
}

// 第一个参数学生为0，教师为1；登录失败返回0，成功返回1
int check_password(int who, char* ID, char* password)
{
	char query[300] = "SELECT name FROM ";
	if (who) {
		strcat(query, "teachers WHERE teachID='");
		strcat(query, ID);
		strcat(query, "'");
	}
	else {
		strcat(query, "students WHERE stuID='");
		strcat(query, ID);
		strcat(query, "'");
	}
	strcat(query, " AND passwd=");
	strcat(query, "'");
	pw_encode(password);
	strcat(query, password);
	strcat(query, "'");
	mysql_query(&mysql, query);  // 同时匹配用户名和密码查询
	result = mysql_store_result(&mysql);
	if (result) {     // 防止数据为空造成崩溃
		if ((int)mysql_num_rows(result) != 1) // 若非有且仅有一行数据则登录失败
		{
			mysql_free_result(result);
			return 0;
		}
		else {
			mysql_free_result(result);
			return 1;
		}
	}
	else 
		return 0;
}

void student_mainmenu()
{
	HANDLE handle;
	do {
		system("cls");
		int option;
		system("title 学生选课管理系统 - 学生主菜单");
		change_color(5, 14);
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t\t ○●○●○● 功能界面--学生 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		change_color(4, 14);
		printf("\n登录成功，欢迎您，%s的%s同学。\n", nowSchool, nowName);
		change_color(1, 14);
		printf("\n\t\t\t\t           请选择你需要的操作：\n\n");
		printf("\t\t\t\t             【① 学生选课】\n");
		printf("\t\t\t\t                                 \n");
		printf("\t\t       根据课程名查询  ┐                        ┌ 根据开课学院查询\n");
		printf("\t\t\t\t       ├───  【② 查询课程】 ───┼ 根据课余量排序所有课程\n");
		printf("\t\t     查询课程详细信息  ┘                        └ 根据选课人数排序所有课程\n");
		printf("\t\t\t\t                                 \n");
		printf("\t\t\t\t           【③ 查询选课结果】\n\n");
		printf("\t\t\t\t           【④ 删除选课结果】\n\n");
		printf("\t\t\t\t           【⑤ 个人信息管理】\n\n\n");
		printf("\t\t\t\t             【⑥ 退出登录】\n\n");
		printf(" 请输入1，2，3，4，5，6或直接点击相应标题：");

		sprintf(pos, "8-8,45-56|11-11,45-56|14-14,43-58|16-16,43-58|18-18,43-58|21-21,45-56");/*10 - 12, 21 - 87*/
		handle = CreateThread(NULL, 0, SelectEventThread, NULL, 0, NULL);
		scanf_opt(&option, 1, 6);
		TerminateThread(handle, 1);
		
		if (option == 1)
		{
			student_select_course();
			continue;
		}
		else if (option == 2)
		{
			student_query_course();
			continue;
		}
		else if (option == 3)
		{
			student_query_result();
			change_color(1, 14);
			printf("请按任意键返回上一菜单\n");
			system("pause > nul");
			continue;
		}
		else if (option == 4)
		{
			student_delete_course();
			continue;
		}
		else if (option == 5)
		{
			student_manage_info();
			continue;
		}
		else if (option == 6)
			return;
	} while (1);
}

void student_register()
{
	int contflag = 0;
	system("cls");
	char school[50], major[50], name[50], sexual[5]
		, phone[100], passwd[100], email[100];
	char query[400];
	system("title 学生选课管理系统 - 学生注册");
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t\t○●○●○● 注册 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	change_color(1, 14);
	printf("请输入学号:");
	s_gets(stuID, 11);

	do {
		while (check_stuId(stuID) == 0)//检查输入是否符合规范 
		{
			change_color(4, 14);
			printf("无效输入！请输入10位数字:(若返回上一级，请按Ctrl+Q后回车)");
			change_color(1, 14);
			s_gets(stuID, 11);
			if (stuID[0] == 17)//判断若输入首字符为ctrl+q则返回上层
			{
				system("cls");
				return;
			}
		}
		sprintf(query, "select * from students where stuID='%s'", stuID);
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);
		if ((int)mysql_num_rows(result) != 0)
		{
			change_color(4, 14);
			printf("此学号已注册!请更换学号:(若返回上一级，请按Ctrl+Q后回车)");
			change_color(1, 14);
			s_gets(stuID, 11);
			if (stuID[0] == 17)//判断若输入首字符为ctrl+q则返回上层
			{
				system("cls");
				return;
			}
		}
	} while ((int)mysql_num_rows(result) != 0);

	printf("请输入密码:");
	scanf_pw(passwd);
	char passwd1[30];
	printf("请再次确认密码:");
	scanf_pw(passwd1);
	do {
		if (strcmp(passwd, passwd1) != 0)
		{
			change_color(4, 14);
			printf("两次输入的密码不一致，请重新确认：（若返回上一级，请按Ctrl+Q后回车）");
			s_gets(passwd1, 20);
			if (passwd1[0] == 17)//判断若输入首字符为ctrl+q则返回上层
			{
				system("cls");
				return;
			}
		}
	} while (strcmp(passwd, passwd1) != 0);
	pw_encode(passwd);
	printf("请输入学院:");
	s_gets(school, 20);
	printf("请输入专业:");
	s_gets(major, 30);
	printf("请输入姓名:");
	s_gets(name, 20);
	printf("请输入性别:");
	s_gets(sexual, 3);
	while (strcmp(sexual, "男") != 0 && strcmp(sexual, "女") != 0)
	{
		change_color(4, 14);
		printf("无效输入！请输入男或女:");
		change_color(1, 14);
		s_gets(sexual, 3);
	}
	change_color(1, 14);
	printf("请输入电话:");
	s_gets(phone, 11);
	while (check_phone(phone) == 0)
	{
		change_color(4, 14);
		printf("无效输入！请输入11位电话号:（若返回上一级，请按Ctrl+Q后回车）");
		change_color(1, 14);
		s_gets(phone, 12);
		if (phone[0] == 17)//返回上一级
		{
			system("cls");
			return;
		}
	}

	printf("请输入邮箱:");
	s_gets(email, 30);
	while (check_email(email) == 0)
	{
		change_color(4, 14);
		printf("无效输入！请按照***@***.***格式输入:（若返回上一级，请按Ctrl+Q后回车）");
		s_gets(email, 30);
		if (email[0] == 17)//返回上一级
		{
			system("cls");
			return;
		}
	}
	sprintf(query, "INSERT INTO `students` (`stuID`, `school`, `major`, `name`, `sexual`, `phone`, `passwd`, `email`) "
		"VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')"
		, stuID, school, major, name, sexual, phone, passwd, email);

	if (mysql_query(&mysql, query))
	{
		change_color(4, 14);
		printf("\n数据库读写失败，请重试\n");
	}
	else
	{
		change_color(2, 14);
		printf("\n注册成功！\n");
	}
	change_color(1, 14);
	printf("\n请按任意键返回上一菜单");
	system("pause > nul");
	return;
}

int check_phone(char* str)
{
	int b = 1, y = 0;
	while (str[y] != '\0')
		y++;
	if (y != 11)
		b = 0;
	for (int y = 0; y < 11; y++)
	{
		if (str[y] < '0' || str[y]>'9')
			b = 0;
	}
	return b;
}

void student_select_course()
{
	MYSQL_RES* result1, * result2, * result3, * result4, * result5;
	MYSQL_ROW Row1 = Row, Row2 = Row, Row3 = Row, Row4 = Row, Row5 = Row;	//-为了去除warnings必须先初始化
	system("cls");
	system("title 学生选课管理系统 - 学生选课");
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t\t○●○●○● 学生选课 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	char classID[20];
	change_color(1, 14);
	printf("\n课程信息如下：\n");
	char query[200];
	sprintf(query, "select 课程编号,开课学院,课程名称,学分,课程性质,开课教师,"
		"(限制人数-已选人数) 余课量,学分,学分,学分,学分,学分,学分,学分,学分 from classes");
	change_color(0, 14);
	print_class(query);
	change_color(1, 14);
	
	//判断是否有这门课-------------------------------------------------------------------------------
	do {
		printf("\n请输入您想选的课程编号（若返回上一级，请按Ctrl+Q后回车）：");
		s_gets(classID, 30);
		if (classID[0] == 17)//返回上一级
			return;
	} while (student_check_class_exist(classID));
	
	

	//把学生之前选的课从表中取出，存到字符串中
	sprintf(query, "select class1,class2,class3 from students where stuID='%s'", stuID);
	mysql_store_result(&mysql);
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	if (result)
		Row = mysql_fetch_row(result);

	if ((Row[0] != NULL) && (Row[1] != NULL) && (Row[2] != NULL))//class123都不为空
	{
		change_color(4, 14);
		printf("\n您已选满三节课，无法再次选课！\n");
	}
	else {
		//判断从表中取出的那些课的是不是和学生现在想选的课有重复
		if (((Row[0] != NULL) && (strcmp(classID, Row[0]) == 0))
			|| ((Row[1] != NULL) && (strcmp(classID, Row[1]) == 0))
			|| ((Row[2] != NULL) && (strcmp(classID, Row[2]) == 0)))//strcmp,如果有重复,class1ID==classID||....||....
		{
			change_color(4, 14);
			printf("\n您已选过该课程！\n\n");
		}
		else {
			//把学生已选的课的开课时间，结课时间，时间段取出并保存在字符串中
			//select 开课时间,结课时间,上课时间段 from classes where 课程编号=
			//(select class1 from students where stuID='2019222222')
			sprintf(query, "select 开课时间,结课时间,上课时间段 from classes where 课程编号=(select class1 from students where stuID='%s')", stuID);
			mysql_query(&mysql, query);
			result2 = mysql_store_result(&mysql);
			if (result2)
				Row2 = mysql_fetch_row(result2);

			sprintf(query, "select 开课时间,结课时间,上课时间段 from classes where 课程编号=(select class2 from students where stuID='%s')", stuID);
			mysql_query(&mysql, query);
			result3 = mysql_store_result(&mysql);
			if (result3)
				Row3 = mysql_fetch_row(result3);

			sprintf(query, "select 开课时间,结课时间,上课时间段 from classes where 课程编号=(select class3 from students where stuID='%s')", stuID);
			mysql_query(&mysql, query);
			result4 = mysql_store_result(&mysql);
			if (result4)
				Row4 = mysql_fetch_row(result4);

			sprintf(query, "select 开课时间,结课时间,上课时间段 from classes where 课程编号='%s'", classID);
			mysql_query(&mysql, query);
			result5 = mysql_store_result(&mysql);
			if (result5)
				Row5 = mysql_fetch_row(result5);

			int check1, check2, check3;
			check1 = 0;
			check2 = 0;
			check3 = 0;

			if ((int)mysql_num_rows(result2) != 0 && (int)mysql_num_rows(result5) != 0)
				check1 = check_timeClash(Row2[0], Row2[1], Row2[2], Row5[0], Row5[1], Row5[2]);//比较class1和class2是否时间冲突
			if ((int)mysql_num_rows(result3) != 0 && (int)mysql_num_rows(result5) != 0)
				check2 = check_timeClash(Row3[0], Row3[1], Row3[2], Row5[0], Row5[1], Row5[2]);
			if ((int)mysql_num_rows(result4) != 0 && (int)mysql_num_rows(result5) != 0)
				check3 = check_timeClash(Row4[0], Row4[1], Row4[2], Row5[0], Row5[1], Row5[2]);

			sprintf(query, "select 限制人数,已选人数 from classes where 课程编号='%s'", classID);
			mysql_query(&mysql, query);
			result1 = mysql_store_result(&mysql);
			if (result1)
				Row1 = mysql_fetch_row(result1);
			if (strcmp(Row1[0], Row1[1]) == 0)//超过限制人数
			{
				change_color(4, 14);
				printf("\n此课程已满，无法选择！\n");
			}
			else
			{
				if (check1 == 1 || check2 == 1 || check3 == 1)//判断时间是否和已选的课有冲突
				{
					change_color(4, 14);
					printf("\n此课程和您已选课程时间冲突，无法选择！\n");
				}
				else {
					sprintf(query, "update classes set 已选人数=已选人数+1 where 课程编号='%s'", classID);
					mysql_query(&mysql, query);
					if (Row[0] == NULL)//class1 == null
					{
						//在class1中插入选的课程编号
						sprintf(query, "update students set class1 ='%s' where stuID='%s'", classID, stuID);
						mysql_query(&mysql, query);
						change_color(2, 14);
						printf("\n选课成功！");
					}
					else if ((Row[0] != NULL) && (Row[1] == NULL))//class1 != null并且class2=null
					{
						//在class2中插入选的编号
						sprintf(query, "update students set class2 ='%s' where stuID='%s'", classID, stuID);
						mysql_query(&mysql, query);
						change_color(2, 14);
						printf("\n选课成功！");
					}
					else if ((Row[0] != NULL) && (Row[1] != NULL) && (Row[2] == NULL))//class1 != null并且class2=！null并且class3==null
					{
						//在class3中插入选的编号
						sprintf(query, "update students set class3 ='%s' where stuID='%s'", classID, stuID);
						mysql_query(&mysql, query);
						change_color(2, 14);
						printf("\n选课成功！");
					}
				}
			}
		}
	}
	change_color(1, 14);
	printf("请按任意键返回上一菜单\n");
	system("pause > nul");
}

void student_query_course()
{
	HANDLE handle;
	do {
		char query[500];
		system("cls");
		int option;
		system("title 学生选课管理系统 - 学生查课");
		change_color(5, 14);
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t\t  ○●○●○● 查询课程 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		change_color(1, 14);
		printf("\n\t\t\t\t           请选择查询方式：\n\n");
		printf("\t\t\t\t       【 ① 根据课程名查询 】\n\n");
		printf("\t\t\t\t      【 ② 根据开课学院查询 】\n\n");
		printf("\t\t\t\t   【 ③ 根据课余量排序所有课程 】\n\n");
		printf("\t\t\t\t  【 ④ 根据选课人数排序所有课程 】\n\n");
		printf("\t\t\t\t      【 ⑤ 查看课程详细信息 】\n\n\n");
		printf("\t\t\t\t       【 ⑥ 返回学生主菜单 】\n\n");
		printf("请输入1，2，3，4，5，6或直接点击相应标题:");

		sprintf(pos, "6-6,39-60|8-8,38-62|10-10,35-64|12-12,34-65|14-14,38-62|17-17,39-60");
		handle = CreateThread(NULL, 0, SelectEventThread, NULL, 0, NULL);
		scanf_opt(&option, 1, 6);
		TerminateThread(handle, 1);

		if (option == 1)
		{
			system("cls");
			char class_name[20];
			change_color(5, 14);
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\t\t\t\t○●○●○● 查询课程--课程名查询 ●○●○●○\n");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			change_color(1, 14);
			printf("\n请输入课程名称：");
			s_gets(class_name, 20);
			printf("\n选课结果为：\n\n");
			sprintf(query, "select * from classes where 课程名称='%s'", class_name);
			change_color(0, 14);
			print_class(query);
			change_color(1, 14);
			printf("\n请按任意键返回上一菜单");
			system("pause > nul");
			continue;
		}
		if (option == 2)
		{
			system("cls");
			char school_name[20];
			change_color(5, 14);
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\t\t\t\t○●○●○● 查询课程--开课学院查询 ●○●○●○\n");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			change_color(1, 14);
			printf("\n请输入开课学院名称：");
			s_gets(school_name, 20);
			printf("\n选课结果为：\n\n");
			sprintf(query, "select * from classes where 开课学院='%s'", school_name);
			change_color(0, 14);
			print_class(query);
			change_color(1, 14);
			printf("\n请按任意键返回上一菜单");
			system("pause > nul");
			continue;
		}
		if (option == 3)
		{
			system("cls");
			change_color(5, 14);
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\t\t\t\t○●○●○● 查询课程--课余量排序所有课程 ●○●○●○\n");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			change_color(1, 14);
			printf("\n按课余量排序的所有课程如下：\n\n");
			sprintf(query, "select 课程编号,开课学院,课程名称,学分,课程性质,开课教师,(限制人数-已选人数) 余课量,教材信息,");
			strcat(query, "课程简介,已选人数,限制人数,上课地点,上课时间段,开课时间,结课时间 from classes order by 余课量 desc");
			change_color(0, 14);
			print_class(query);
			change_color(1, 14);
			printf("\n请按任意键返回上一菜单");
			system("pause > nul");
			continue;
		}
		if (option == 4)
		{
			system("cls");
			change_color(5, 14);
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\t\t\t\t○●○●○● 查询课程--选课人数排序所有课程 ●○●○●○\n");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			change_color(1, 14);
			printf("\n根据选课人数排序的所有课程如下：\n\n");
			sprintf(query, "select 课程编号,开课学院,课程名称,学分,课程性质,开课教师,已选人数,教材信息,课程简介,已选人数,限制人数,");
			strcat(query, "上课地点,上课时间段,开课时间,结课时间 from classes order by 已选人数 desc");
			change_color(0, 14);
			print_class(query);
			change_color(1, 14);
			printf("\n请按任意键返回上一菜单");
			system("pause > nul");
			continue;
		}
		if (option == 5)
		{
			student_search_specific_imformation();
			change_color(1, 14);
			printf("请按任意键返回上一菜单\n");
			system("pause > nul");
			continue;
		}
		if (option == 6)
		{
			system("cls");
			return;
		}
	}while (1);
}

void student_query_result()
{
	//sql语句为
	//select c.* from classes c where c.`课程编号`=(select s.class1 from students s where stuID=??????????)
	//or c.`课程编号`=(select s.class2 from students s where stuID=??????????) 
	//or c.`课程编号`=(select s.class3 from students s where stuID=??????????)
	system("cls");
	char query[500] = "select c.* from classes c where c.`课程编号`=(select s.class1 from students s where stuID=";
	strcat(query, stuID);
	strcat(query, ")or c.`课程编号`=(select s.class2 from students s where stuID=");
	strcat(query, stuID);
	strcat(query, ")or c.`课程编号`=(select s.class3 from students s where stuID=");
	strcat(query, stuID);
	strcat(query, ")");
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t\t○●○●○● 查询选课结果 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	change_color(1, 14);
	printf("\n您的选课结果如下：\n\n");
	change_color(0, 14);
	print_class(query);
}

void student_delete_course()
{
	system("cls");
	char classID[100];
	char query[300];
	system("title 学生选课管理系统 - 学生删课");
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t\t○●○●○● 删除选课结果 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	change_color(1, 14);
	char query1[500] = "select c.* from classes c where c.`课程编号`=(select s.class1 from students s where stuID=";
	strcat(query1, stuID);
	strcat(query1, ")or c.`课程编号`=(select s.class2 from students s where stuID=");
	strcat(query1, stuID);
	strcat(query1, ")or c.`课程编号`=(select s.class3 from students s where stuID=");
	strcat(query1, stuID);
	strcat(query1, ")");
	printf("\n您的选课结果如下：\n\n");
	change_color(0, 14);
	print_class(query1);
	change_color(1, 14);
	do {
		printf("\n请输入您想删除的课程编号（若返回上一级，请按Ctrl+Q后回车）：");
		s_gets(classID, 11);
		if (classID[0] == 17)//返回上一级
			return;

	} while (student_check_class_exist(classID));
	//取出学生想删除课的开课时间和上课时间段
	sprintf(query, "select 开课时间,上课时间段 from classes where 课程编号='%s'", classID);
	mysql_store_result(&mysql);
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	if (result)
		Row = mysql_fetch_row(result);

	while (getState_starting(Row[0], Row[1]) == 1)
	{
		change_color(4, 14);
		printf("此课程已开课，无法删除！\n");
		change_color(1, 14);
		do {
			printf("请重新输入课程编号：（若返回上一级，请按Ctrl+Q后回车）\n");
			s_gets(classID, 11);
			if (classID[0] == 17)//返回上一级
				return;
		} while (student_check_class_exist(classID));
		sprintf(query, "select 开课时间,上课时间段 from classes where 课程编号='%s'", classID);
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);
		if (result)
			Row = mysql_fetch_row(result);
	}

	sprintf(query, "select class1,class2,class3 from students where stuID='%s'", stuID);
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	if (result)
		Row = mysql_fetch_row(result);

	if (Row[0] != NULL && strcmp(Row[0], classID) == 0)
	{
		sprintf(query, "update students set class1=null where stuID='%s'", stuID);
		mysql_query(&mysql, query);
		change_color(2, 14);
		printf("\n删除成功!");
		sprintf(query, "update classes set 已选人数=已选人数-1 where 课程编号='%s'", classID);
		mysql_query(&mysql, query);
	}
	if (Row[1] != NULL && strcmp(Row[1], classID) == 0)
	{
		sprintf(query, "update students set class2=null where stuID='%s'", stuID);
		mysql_query(&mysql, query);
		change_color(2, 14);
		printf("\n删除成功!");
		sprintf(query, "update classes set 已选人数=已选人数-1 where 课程编号='%s'", classID);
		mysql_query(&mysql, query);
	}
	if (Row[2] != NULL && strcmp(Row[2], classID) == 0)
	{
		sprintf(query, "update students set class3=null where stuID='%s'", stuID);
		mysql_query(&mysql, query);
		change_color(2, 14);
		printf("\n删除成功!");
		sprintf(query, "update classes set 已选人数=已选人数-1 where 课程编号='%s'", classID);
		mysql_query(&mysql, query);
	}
	change_color(1, 14);
	printf("请按任意键返回上一菜单\n");
	system("pause > nul");
}

void student_manage_info()
{
	HANDLE handle;
	do {
		system("cls");
		int option;
		char phone[20];
		char passwd[20];
		char passwd1[20];
		char email[50];
		system("title 学生选课管理系统 - 学生个人信息管理");
		change_color(5, 14);
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t\t○●○●○● 个人信息管理--学生 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		change_color(1, 14);
		printf("\n\t\t\t\t           请选择需要修改的信息：\n\n");
		printf("\t\t\t\t                【 ① 电话 】\n\n");
		printf("\t\t\t\t                【 ② 密码 】\n\n");
		printf("\t\t\t\t                【 ③ 邮箱 】\n\n\n");
		printf("\t\t\t\t           【 ④ 返回学生主菜单 】\n\n");
		printf("\n请输入1，2，3，4或直接点击相应标题：");

		sprintf(pos, "6-6,48-60|8-8,48-60|10-10,48-60|13-13,43-65|1--1,2-17|1--1,2-17");
		handle = CreateThread(NULL, 0, SelectEventThread, NULL, 0, NULL);
		scanf_opt(&option, 1, 4);
		TerminateThread(handle, 1);

		switch (option)
		{
		case 1:
		{
			system("cls");
			change_color(5, 14);
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\t\t\t\t○●○●○● 个人信息管理--学生--电话 ●○●○●○\n");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			change_color(1, 14);
			printf("\n请输入新的电话：");
			s_gets(phone, 11);
			while (check_phone(phone) == 0)
			{
				change_color(4, 14);
				printf("输入无效！请输入11位电话号：(若返回上一级，请按Ctrl+Q后回车)");
				change_color(1, 14);
				s_gets(phone, 11);
				if (phone[0] == 17)//判断若输入首字符为ctrl+q则返回上层
				{
					system("cls");
					return;
				}
			}
			char query[200] = "update students set phone='";
			strcat(query, phone);
			strcat(query, "' where stuID='");
			strcat(query, stuID);
			strcat(query, "'");
			mysql_query(&mysql, query);
			change_color(2, 14);
			printf("\n修改成功!\n");
			change_color(1, 14);
			printf("请按任意键返回上一菜单\n");
			system("pause > nul");
			continue;
			break;
		}
		case 2:
		{
			system("cls");
			change_color(5, 14);
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\t\t\t\t○●○●○● 个人信息管理--学生--密码 ●○●○●○\n");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			change_color(1, 14);
			printf("\n请输入新的密码：");
			scanf_pw(passwd);
			printf("请再次确认新的密码：");
			scanf_pw(passwd1);
			do {
				if (strcmp(passwd, passwd1) != 0)
				{
					change_color(4, 14);
					printf("两次输入的密码不一致，请重新确认：（若返回上一级，请按Ctrl+Q后回车）");
					s_gets(passwd1, 20);
					if (passwd1[0] == 17)//判断若输入首字符为ctrl+q则返回上层
					{
						system("cls");
						return;
					}
				}
			} while (strcmp(passwd, passwd1) != 0);
			pw_encode(passwd);
			char query1[200] = "update students set passwd='";
			strcat(query1, passwd);
			strcat(query1, "' where stuID='");
			strcat(query1, stuID);
			strcat(query1, "'");
			mysql_query(&mysql, query1);
			change_color(2, 14);
			printf("\n修改成功!\n\n");
			change_color(1, 14);
			printf("请按任意键返回上一菜单\n");
			system("pause > nul");
			continue;
			break;
		}
		case 3:
		{
			system("cls");
			change_color(5, 14);
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\t\t\t\t○●○●○● 个人信息管理--学生--邮箱 ●○●○●○\n");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			change_color(1, 14);
			printf("\n请输入新的邮箱：");
			s_gets(email, 30);
			while (check_email(email) == 0)
			{
				change_color(4, 14);
				printf("无效输入！请按照***@***.***格式输入：（若返回上一级，请按Ctrl+Q后回车）");
				change_color(1, 14);
				s_gets(email, 30);
				if (email[0] == 17)//判断若输入首字符为ctrl+q则返回上层
				{
					system("cls");
					return;
				}
			}
			char query2[200] = "update students set email='";
			strcat(query2, email);
			strcat(query2, "' where stuID='");
			strcat(query2, stuID);
			strcat(query2, "'");
			mysql_query(&mysql, query2);
			change_color(2, 14);
			printf("\n修改成功!\n");
			change_color(1, 14);
			printf("\n请按任意键返回上一菜单\n");
			system("pause > nul");
			continue;
			break;
		}
		case 4:
		{
			system("cls");
			return;
		}
		}
	}while (1);
}

void student_search_specific_imformation()
{
	char classID[100];
	system("cls");
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t\t○●○●○● 查看课程详细信息--学生 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	change_color(1, 14);
	printf("\n全部课程如下：\n\n");
	char query3[200] = "select 课程编号,开课学院,课程名称,学分,学时,课程性质,开课教师,教材信息,";
	strcat(query3, "课程简介,已选人数,限制人数,上课地点,上课时间段,开课时间,结课时间 from classes");
	change_color(0, 14);
	print_class(query3);
	change_color(1, 14);
	do {
		printf("\n请输入您想查看详细信息的课程编号（若返回上一级，请按Ctrl+Q后回车）：");
		s_gets(classID, 11);
		if (classID[0] == 17)//返回上一级
			return;

	} while (student_check_class_exist(classID));
	printf("\n\n");
	char query[200] = "select 开课时间,结课时间,上课时间段,上课地点,限制人数,已选人数 from classes where 课程编号='";
	strcat(query, classID);
	strcat(query, "'");
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	if (result)
	{
		int fieldCount = mysql_field_count(&mysql);
		if (fieldCount > 0)
		{
			int row = (int)mysql_num_rows(result);
			int column = mysql_num_fields(result);
			change_color(0, 14);
			for (int i = 0; field = mysql_fetch_field(result); i++) {
				//获得属性名 
				if (i == 0 || i == 1)
					printf("%-27s |", field->name);
				else if (i == 2)
					printf("%-17s |", field->name);
				else if (i == 3 || i == 4 || i == 5)
					printf("%-8s |", field->name);
				else
					printf(" %-22s |", field->name);
			}
			printf("\n");
			while (Row = mysql_fetch_row(result)) {
				for (int j = 0; j < column; j++)
				{
					if (j == 0 || j == 1)
						printf("%-27s |", Row[j]);
					else if (j == 2)
						printf("%-17s |", Row[j]);
					else if (j == 3 || j == 4 || j == 5)
						printf("%-8s |", Row[j]);
					else
						printf("%-22s |", Row[j]);
				}
				printf("\n\n");
				change_color(1, 14);
			}
		}
	}
	char query1[200] = "select 教材信息 from classes where 课程编号='";
	strcat(query1, classID);
	strcat(query1, "'");
	mysql_query(&mysql, query1);
	result = mysql_store_result(&mysql);
	if (result)
	{
		change_color(0, 14);
		printf("教材信息\n");
		Row = mysql_fetch_row(result);
		printf("%s\n\n", Row[0]);
		change_color(1, 14);
	}

	char query2[200] = "select 课程简介 from classes where 课程编号='";
	strcat(query2, classID);
	strcat(query2, "'");
	mysql_query(&mysql, query2);
	result = mysql_store_result(&mysql);
	if (result)
	{
		change_color(0, 14);
		printf("课程简介\n");
		Row = mysql_fetch_row(result);
		printf("%s\n\n", Row[0]);
		change_color(1, 14);
	}
}

//传入查询语句打印查询结果
void print_class(char* query)
{
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	if (result)
	{
		int fieldCount = mysql_field_count(&mysql);
		if (fieldCount > 0)
		{
			int row = (int)mysql_num_rows(result);
			int column = mysql_num_fields(result);
			for (int i = 0; field = mysql_fetch_field(result),i<7; i++) {
				//获得属性名 
				if (i == 0)
					printf(" %-8s|", field->name);
				else if (i == 1||i==2)
					printf(" %-20s|", field->name);
				else
					printf(" %-8s|", field->name);
			}

			printf("\n");
			while (Row = mysql_fetch_row(result)) 
			{
				for (int j = 0; j < column-8; j++) 
				{
					if (j == 0)
						printf(" %-8s|", Row[j]);
					else if (j == 1 || j == 2)
						printf(" %-20s|", Row[j]);
					else
						printf(" %-8s|", Row[j]);
				}
				printf("\n");
			}
		}
	}
}

int check_email(char* str)
{
	int sum = 0;
	int k = 0, m = 0;
	while (str[sum] != '\0')
		sum++;
	int a[50];
	int b[50];

	if (sum > 50)
		return 0;

	if (str[0] == '@' || str[sum-1] == '.')
		return 0;

	for (int i = 0; i < sum; i++)
	{
		if (str[i] == '@')
		{
			a[k] = i;
			k++;
		}
		if (str[i] == '.')
		{
			b[m] = i;
			m++;
		}
	}
	if (k > 1 || m > 1 || k == 0 || m == 0 || a[0] > b[0] || b[0] == (a[0] + 1)||(m==1&&str[sum]=='.'))
		return 0;
	else
		return 1;
}

//快捷判断两个课时间重叠与否，输入格式：
//一课的开课时间、结课时间、具体上课时间段、二课的开课时间、结课时间、具体上课时间段；有冲突返回1
int check_timeClash(char* time1_sweek, char* time1_eweek, char* time1_day, char* time2_sweek, char* time2_eweek, char* time2_day)
{
	char term1[10], term2[10], sweek1[5], sweek2[5], eweek1[5], eweek2[5];
	int s1i, e1i, s2i, e2i;  // 一课的开课周数，结课周数，二课的开课，结课周数

	if (strcmp(time1_day, time2_day)) //上课时间段不一样直接pass
		return 0;

	if (time1_sweek[3] != time2_sweek[3])//开课年份不一样直接pass
		return 0;
	else
	{
		sprintf(term1, "%c%c", time1_sweek[15], time1_sweek[16]);
		sprintf(term2, "%c%c", time2_sweek[15], time2_sweek[16]);
		if (strcmp(term1, term2))   //开课年份一样学期不一样也pass
			return 0;
	}

	if (time1_sweek[24] > 127)    //取出开课周数
		sprintf(sweek1, "%c", time1_sweek[23]);
	else
		sprintf(sweek1, "%c%c", time1_sweek[23], time1_sweek[24]);

	if (time2_sweek[24] > 127)
		sprintf(sweek2, "%c", time2_sweek[23]);
	else
		sprintf(sweek2, "%c%c", time2_sweek[23], time2_sweek[24]);

	s1i = atoi(sweek1);
	s2i = atoi(sweek2);

	if (time1_eweek[24] > 127)    //取出开课周数
		sprintf(eweek1, "%c", time1_eweek[23]);
	else
		sprintf(eweek1, "%c%c", time1_eweek[23], time1_eweek[24]);

	if (time2_eweek[24] > 127)
		sprintf(eweek2, "%c", time2_eweek[23]);
	else
		sprintf(eweek2, "%c%c", time2_eweek[23], time2_eweek[24]);

	e1i = atoi(eweek1);
	e2i = atoi(eweek2);

	if (s1i < s2i)
	{     //若课1开得比课2早，必须先比课1结束
		if (e1i < s2i)
			return 0;
		else
			return 1;
	}
	else
	{
		if (e2i < s1i)  //若课1开得比课2早，课1必须先比课2结束
			return 0;
		else
			return 1;
	}
}

// 根据id查询课程是否存在
// 若存在返回0，不存在返回1
int student_check_class_exist(char* classID)
{

	char query10[100] = "select * from classes where 课程编号='";
	strcat(query10, classID);
	strcat(query10, "'");
	mysql_store_result(&mysql);
	mysql_query(&mysql, query10);
	result = mysql_store_result(&mysql);
	if ((int)mysql_num_rows(result) == 0)
	{
		change_color(4, 14);
		printf("无此课程，请重新输入！\n");
		change_color(1, 14);
		return 1;
	}
	return 0;
}

void teacher_mainmenu()
{
	HANDLE handle;
	do {
		system("cls");
		int option2 = 0;
		system("title 学生选课管理系统 - 教师主菜单");
		change_color(5, 14);
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t\t    ○●○●○● 教师主菜单 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		change_color(4, 14);
		printf("\n登录成功，欢迎您，%s的%s老师。\n", nowSchool, nowName);
		change_color(1, 14);
		printf("\n\t\t\t\t             请选择您需要的服务:\n");
		printf("\t\t\t\t                                  \n");
		printf("\t\t          查看开设的课程 ┐                       ┌  查询学生信息\n");
		printf("\t\t\t\t         ├─── 【 ① 选课管理 】──┼  删除课程(选课结束后)\n");
		printf("\t\t                排序课程 ┘                       └  统计课程数目\n");
		printf("\t\t\t\t                                 \n");
		printf("\t\t                 查询课程 ┐                      ┌  添加课程\n");
		printf("\t\t\t\t          ├── 【 ② 课程管理 】──┤\n");
		printf("\t\t                 排序课程 ┘                      └  删除课程(选课开始前)\n");
		printf("\t\t\t\t                                 \n");
		printf("\t\t\t\t              【 ③ 信息管理 】──┬  修改密码\n");   //printf("\t\t\t\t 修改邮箱 ── 【 ③ 信息管理 】──  修改密码\n");
		printf("\t\t\t\t                                 └  修改邮箱\n\n");   //printf("\t\t\t\t                                 \n\n");
		printf("\t\t\t\t              【 ④ 退出登录 】\n\n");		
		printf("\n  请输入1，2，3，4或直接点击相应标题:");			

		sprintf(pos, "9-9,46-62|13-13,46-62|16-16,46-62|19-19,46-62|1--1,2-17|1--1,2-17");
		/*sprintf(pos, "8-10,26-87|12-14,33-87|15-17,46-75|19-19,46-62|1--1,2-17|1--1,2-17");*/
		handle = CreateThread(NULL, 0, SelectEventThread, NULL, 0, NULL);

		scanf_opt(&option2, 1, 4);

		TerminateThread(handle, 1);
		switch (option2)
		{
		case 1:
			teacher_select_managemenu();
			break;
		case 2:
			teacher_course_managemenu();
			break;
		case 3:
			teacher_manage_info();
			break;
		case 4:
			return;
			break;
		}
	} while (1);
}

void teacher_select_managemenu() 
{
	HANDLE handle;
	do {
		system("cls");
		int option2 = 0;
		system("title 学生选课管理系统 - 教师选课管理");
		change_color(5, 14);
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t\t  ○●○●○● 选课菜单 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		change_color(1, 14);
		printf("\n\t\t\t\t          请选择您需要的服务:\n\n");
		printf("\t\t\t\t     【 ① 查看您开设的选课情况 】\n\n");
		printf("\t\t\t\t  【 ② 查询选择某门课程的学生信息 】\n\n");
		printf("\t\t\t\t   【 ③ 删除您的课程(选课结束后) 】\n\n");
		printf("\t\t\t\t  【 ④ 统计您自己开设过的课程数目 】\n\n");
		printf("\t\t\t\t【 ⑤ 按选课人数排序所有开设过的课程 】\n\n\n");
		printf("\t\t\t\t        【 ⑥ 返回上一个菜单 】\n\n");
		printf("\n请输入1,2,3,4,5，6或直接点击相应标题:");

		sprintf(pos, "6-6,37-65|8-8,34-68|10-10,35-67|12-12,34-68|14-14,32-70|17-17,40-62");
		handle = CreateThread(NULL, 0, SelectEventThread, NULL, 0, NULL);

		scanf_opt(&option2, 1, 6);

		TerminateThread(handle, 1);

		switch (option2)
		{
		case 1:
			teacher_mycourse();
			break;
		case 2:
			teacher_findcourse();
			continue;
			break;
		case 3:
			teacher_30delete();
			break;
		case 4:
			teacher_totalcourse();
			break;
		case 5:
			teacher_sortcourse();
			break;
		case 6:
			return;
			break;
		}
	} while (1);
}

void teacher_mycourse()
{
	char classID[100];
	system("cls");
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t\t○●○●○● 开设课程查询界面 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	change_color(1, 14);
	printf("\n以下是您开设的课程\n");
	char query[200] = "SELECT * FROM `classes`WHERE 开课教师 = '";
	strcat(query, nowName);
	strcat(query, "'");
	change_color(0, 14);
	print_class(query);//打印相应查询内容
	change_color(1, 14);
	change_color(1, 14);
	printf("输入课程编号以查看该课程的详细信息\n");
	s_gets(classID, 11);
	do {
		char query10[100] = "select * from classes where 课程编号='";
		strcat(query10, classID);
		strcat(query10, "'");
		mysql_store_result(&mysql);
		mysql_query(&mysql, query10);
		result = mysql_store_result(&mysql);
		if ((int)mysql_num_rows(result) == 0)
		{
			change_color(4, 14);
			printf("无此课程，请重新输入！(若返回上一级，请按Ctrl+Q后回车)\n");
			change_color(1, 14);
			s_gets(classID, 11);
			if (classID[0] == 17)
			{
				system("cls");
				return;
			}
		}
	} while ((int)mysql_num_rows(result) == 0);
	sprintf(query,"select 开课时间,结课时间,上课时间段,上课地点,限制人数,已选人数 from classes where 课程编号='");
	strcat(query, classID);
	strcat(query, "'");
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	if (result)
	{
		int fieldCount = mysql_field_count(&mysql);
		if (fieldCount > 0)
		{
			change_color(0, 14);
			int row = (int)mysql_num_rows(result);
			int column = mysql_num_fields(result);
			for (int i = 0; field = mysql_fetch_field(result); i++) {
				//获得属性名 
				if (i == 0 || i == 1)
					printf("%-27s |", field->name);
				else if (i == 2)
					printf("%-17s |", field->name);
				else if (i == 3 || i == 4 || i == 5)
					printf("%-8s |", field->name);
				else
					printf(" %-20s|", field->name);
			}
			printf("\n");
			while (Row = mysql_fetch_row(result)) {
				for (int j = 0; j < column; j++)
				{
					if (j == 0 || j == 1)
						printf("%-27s |", Row[j]);
					else if (j == 2)
						printf("%-17s |", Row[j]);
					else if (j == 3 || j == 4 || j == 5)
						printf("%-8s |", Row[j]);
					else
						printf("%-20s |", Row[j]);
				}
				printf("\n");

			}
		}
		}change_color(1, 14);
	printf("\n按任意键返回上一菜单...\n");
	system("pause>nul");
	return;
}

void teacher_findcourse()
{
	HANDLE handle;
	system("cls");
	int column;
	int flag = 0;
	int option2 = 0;
	char* classID;
	char courseName[200] = "";
	char studentName[200] = "";
	char query[500];
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t\t ○●○●○● 学生信息查询界面 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	change_color(1, 14);
	printf("\n\t\t\t\t            请选择您的查询方式:\n");
	printf("\n\t\t\t\t         【 ① 根据课程名称查询 】\n");
	printf("\n\t\t\t\t         【 ② 根据学生姓名查询 】\n");
	printf("\n\t\t\t\t           【 ③ 返回上一菜单 】\n\n");
	printf("\n请输入，2，3或直接点击相应标题:");

	sprintf(pos, "6-6,40-66|8-8,40-66|10-10,43-63|1--1,2-31|1--1,2-20|1--1,2-17");
	handle = CreateThread(NULL, 0, SelectEventThread, NULL, 0, NULL);

	scanf_opt(&option2, 1, 3);
	TerminateThread(handle, 1);
	switch (option2)
	{
	case 1:
		do
		{
			do
			{
				change_color(1, 14);
				printf("请输入您想要查询的课程名称：");
				s_gets(courseName, 20);
				if (courseName[0] == 17)//判断若输入首字符为ctrl+q则返回上层
				{
					system("cls");
					teacher_findcourse();
					return;
				}
				sprintf(query, "SELECT 课程编号 FROM `classes`WHERE 课程名称 = '%s'", courseName);
				mysql_query(&mysql, query);
				result = mysql_store_result(&mysql);
				if ((int)mysql_num_rows(result) == 0)
				{
					flag = 1;
					change_color(4, 14);
					printf("课程编号输入错误（若返回上一级，请按Ctrl+Q后回车）\n");
					mysql_free_result(result);
				}
				else
					flag = 0;
			} while (flag == 1);
			Row = mysql_fetch_row(result);
			classID = Row[0];


			change_color(1, 14);
			sprintf(query, "SELECT stuID 学生编号,school 所属院系,major 所属专业,name 学生姓名,sexual 性别"
				" FROM `students`WHERE class1 = '%s' OR class2 = '%s' OR class3 = '%s'"
				, classID, classID, classID);
			mysql_query(&mysql, query);
			result = mysql_store_result(&mysql);
			column = mysql_num_fields(result);
			if ((int)mysql_num_rows(result) == 0)
			{
				flag = 1;
				change_color(4, 14);
				printf("该课程暂无学生选课（若返回上一级，请按Ctrl+Q后回车）\n");
				mysql_free_result(result);
			}
			else
				flag = 0;
		} while (flag == 1);

		change_color(0, 14);
		for (int i = 0; field = mysql_fetch_field(result); i++)
			printf("%18s |", field->name);

		printf("\n");

		while (Row = mysql_fetch_row(result))
		{
			for (int j = 0; j < column; j++)
				printf("%18s |", Row[j]);
			printf("\n");
		}
		change_color(1, 14);
		printf("\n");
		sprintf(query, "select stuID 学生编号,phone 学生电话,email 电子邮箱,class1 选课1,class2 选课2,class3 选课3 "
			"FROM `students`WHERE class1 = '%s' OR class2 = '%s' OR class3 = '%s'"
			, classID, classID, classID);
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);
		change_color(0, 14);
		for (int i = 0; field = mysql_fetch_field(result); i++)
			printf("%18s |", field->name);

		printf("\n");

		while (Row = mysql_fetch_row(result))
		{
			for (int j = 0; j < column + 1; j++)
				printf("%18s |", Row[j]);
			printf("\n");
		}
		change_color(1, 14);
		printf("\n按任意键返回上一菜单...\n");
		system("pause>nul");
		teacher_findcourse();
		return;
		break;
	case 2:
		do {
			change_color(1, 14);
			printf("请输入您想要查询的学生名称：");
			s_gets(studentName, 20);
			if (studentName[0] == 17)//判断若输入首字符为ctrl+q则返回上层
			{
				system("cls");
				teacher_findcourse();
				return;
			}
			sprintf(query, "SELECT stuID 学生编号,school 所属院系,major 所属专业,name 学生姓名,sexual 性别 "
				"FROM `students`WHERE name = '%s'", studentName);
			mysql_query(&mysql, query);
			result = mysql_store_result(&mysql);

			column = mysql_num_fields(result);
			if ((int)mysql_num_rows(result) == 0)
			{
				flag = 1;
				change_color(4, 14);
				printf("无结果，请重新输入（若返回上一级，请按Ctrl+Q后回车）\n");
				mysql_free_result(result);
			}
			else
				flag = 0;
		} while (flag == 1);
		change_color(0, 14);
		for (int i = 0; field = mysql_fetch_field(result); i++)
			printf("%17s |", field->name);

		printf("\n");

		while (Row = mysql_fetch_row(result))
		{
			for (int j = 0; j < column; j++)
				printf("%17s |", Row[j]);
			printf("\n");
		}
		change_color(1, 14);
		printf("\n");
		sprintf(query, "SELECT stuID 学生编号,phone 学生电话,email 电子邮箱,"
			"(select 课程名称 from classes where 课程编号=(select class1 from students where  name = '%s')) 选课1,"
			"(select 课程名称 from classes where 课程编号=(select class2 from students where  name = '%s')) 选课2,"
			"(select 课程名称 from classes where 课程编号=(select class3 from students where name = '%s')) 选课3"
			" FROM `students`WHERE name = '%s'"
			, studentName, studentName, studentName, studentName);
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);
		column = mysql_num_fields(result);
		change_color(0, 14);
		for (int i = 0; field = mysql_fetch_field(result); i++)
			printf("%17s |", field->name);

		printf("\n");

		while (Row = mysql_fetch_row(result))
		{
			for (int j = 0; j < column; j++)
				printf("%17s |", Row[j]);
			printf("\n");
		}
		change_color(1, 14);
		printf("\n按任意键返回上一菜单...\n");
		system("pause>nul");
		teacher_findcourse();
		return;
		break;
	case 3:
		return;
		break;
	default:
		change_color(4, 14);
		printf("无效，请重新输入!\n");
	}
}

void teacher_30delete()
{
	system("cls");
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t\t○●○●○● 开设课程删除界面 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	change_color(1, 14);
	printf("\n以下是您开设的课程\n");
	int stu_num;
	int flag = 0;
	int option2 = 0;
	char courseName[200] = "niconiconi";
	char studentName[200] = "niconiconi";
	char query[200] = "SELECT 课程编号,开课学院,课程名称,课程性质,开课教师,限制人数,已选人数,学分,学分,学分,学分,学分,学分,学分,学分 FROM `classes`WHERE 开课教师 = '";
	char query1[200];
	char query2[200];
	strcat(query, nowName);
	strcat(query, "'");
	change_color(0, 14);
	print_class(query);//打印相应内容
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	change_color(1, 14);
	printf("\n请选择您需要的服务:\n\n");
	printf("  ① - 进行课程的删除\n");
	printf("  ② - 返回上一菜单\n\n");
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	change_color(1, 14);
	printf("\n请输入1或2:");
	scanf_opt(&option2, 1, 2);
	switch (option2)
	{
	case 1:
		do {
			change_color(1, 14);
			printf("请输入您想要删除的课程的编号：");
			s_gets(courseName, 20);
			if (courseName[0] == 17)//判断若输入首字符为ctrl+q则返回上层
			{
				system("cls");
				teacher_30delete();
				return;
			}
			sprintf(query, "SELECT 课程编号,开课学院,课程名称,课程性质,开课教师,限制人数,已选人数,学分,学分,学分,学分,学分,学分,学分,学分 FROM `classes`WHERE 课程编号 = '%s'"
				, courseName);
			mysql_query(&mysql, query);
			result = mysql_store_result(&mysql);
			if ((int)mysql_num_rows(result) == 0)
			{
				flag = 1;
				change_color(4, 14);
				printf("无结果，请重新输入（若返回上一级，请按Ctrl+Q后回车）\n");
				mysql_free_result(result);
			}
			else 
				flag = 0;
		} while (flag == 1);
		change_color(0, 14);
		print_class(query);
		change_color(1, 14);
		sprintf(query, "SELECT `已选人数` FROM `classes`WHERE 课程编号 = '%s'"
			, courseName);
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);
		Row = mysql_fetch_row(result);
		stu_num = atoi(Row[0]);

		if (getState_selecting() == 2)
		{
			if (stu_num < 30)
			{
				change_color(5, 14);
				printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
				change_color(1, 14);
				printf("\n确认要删除该课程以及相应学生选课信息吗？\n");
				printf("  ① - 是\n");
				printf("  ② - 否\n\n");
				change_color(5, 14);
				printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
				change_color(1, 14);
				printf("\n请输入1或2:");
				scanf_opt(&option2, 1, 2);

				switch (option2)
				{
				case 1:
					sprintf(query, "DELETE FROM `classes` WHERE `课程编号` = '%s'"
						, courseName);
					mysql_query(&mysql, query);

					sprintf(query1, "SELECT stuID FROM `students` WHERE class1 = '%s'"
						, courseName);
					mysql_query(&mysql, query1);
					result = mysql_store_result(&mysql);
					Row = mysql_fetch_row(result);
					if (Row != nullptr)
					{
						sprintf(query2, "update students set class1 = null where stuID='%s'"
							, Row[0]);
						mysql_query(&mysql, query2);//class1
					}

					sprintf(query1, "SELECT stuID FROM `students` WHERE class2 = '%s'"
						, courseName);
					mysql_query(&mysql, query1);
					result = mysql_store_result(&mysql);
					Row = mysql_fetch_row(result);
					if (Row != nullptr)
					{
						sprintf(query2, "update students set class2 = null where stuID='%s'"
							, Row[0]);
						mysql_query(&mysql, query2);//class2
					}

					sprintf(query1, "SELECT stuID FROM `students` WHERE class3 = '%s'"
						, courseName);
					mysql_query(&mysql, query1);
					result = mysql_store_result(&mysql);
					Row = mysql_fetch_row(result);
					if (Row != nullptr)
					{
						sprintf(query2, "update students set class3 = null where stuID='%s'"
							, Row[0]);
						mysql_query(&mysql, query2);//class3
					}

					change_color(2, 14);
					printf("\n删除成功！\n");
					change_color(1, 14);
					printf("\n按任意键返回上一菜单...\n");
					system("pause>nul");
					teacher_30delete();
					return;
					break;
				case 2:
					teacher_30delete();
					return;
					break;
				default:
					change_color(4, 14);
					printf("无效，请重新输入!\n");
				}
			}
			else
			{
				change_color(4, 14);
				printf("\n非常抱歉，该课程选课人数已超过三十人，您不能对此课程进行删除操作\n");
				change_color(1, 14);
				printf("\n按任意键返回上一菜单...\n");
				system("pause>nul");
				teacher_30delete();
				return;
				break;
			}
		}
		else
		{
			change_color(4, 14);
			printf("\n非常抱歉，该课程选课未结束，您不能对此课程进行删除操作\n");
			change_color(1, 14);
			printf("\n按任意键返回上一菜单...\n");
			system("pause>nul");
			teacher_30delete();
			return;
		}
		break;
	case 2:
		return;
		break;
	default:
		change_color(4, 14);
		printf("无效，请重新输入!\n");
	}
}

void teacher_totalcourse() {
	system("cls");
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t\t○●○●○● 开设课程统计界面 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	char query[100];
	sprintf(query, "select * from `classes` where `开课教师` = '%s' ", nowName);
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	change_color(1, 14);
	printf("\n您总共开设了%d门课程\n", (int)mysql_num_rows(result));
	printf("\n按任意键返回上一菜单...\n");
	system("pause>nul");
	return;
}

void teacher_sortcourse() {
	char query4[100];
	system("cls");
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t\t○●○●○● 开设课程排序界面 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	change_color(1, 14);
	printf("\n以下是您开设课程的排序结果\n");
	sprintf(query4, "select * from `classes` where `开课教师` = '%s' order by `已选人数` "
		, nowName);
	change_color(0, 14);
	print_class(query4);
	change_color(1, 14);
	printf("\n按任意键返回上一菜单...\n");
	system("pause>nul");
	return;
}

void teacher_course_managemenu()
{
	HANDLE handle;
	do {
		system("cls");
		int option2 = 0;
		system("title 学生选课管理系统 - 教师课程管理");
		change_color(5, 14);
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t\t○●○●○● 课程管理菜单 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		change_color(1, 14);
		printf("\n\t\t\t\t          请选择您需要的服务:\n\n");
		printf("\t\t\t\t         【 ① 查询相应课程 】\n\n");
		printf("\t\t\t\t         【 ② 添加您的课程 】\n\n");
		printf("\t\t\t\t         【 ③ 修改您的课程 】\n\n");
		printf("\t\t\t\t   【 ④ 删除您的课程(选课开始前) 】\n\n\n");
		printf("\t\t\t\t        【 ⑤ 返回上一个菜单 】\n\n");
		printf("\n请输入,2,3,4,5或直接点击相应标题:");
		sprintf(pos, "6-6,41-61|8-8,41-61|10-10,41-61|12-12,35-67|15-15,40-62|1-1,40-62");
		handle = CreateThread(NULL, 0, SelectEventThread, NULL, 0, NULL);

		scanf_opt(&option2, 1, 5);

		TerminateThread(handle, 1);
		char query1[200] = "SELECT * FROM `classes` LIMIT 0, 1000";
		switch (option2)
		{
		case 1:
			teacher_course_list();
			break;
		case 2:
			teacher_course_add();
			break;
		case 3:
			teacher_course_edit();
			break;
		case 4:
			teacher_course_delete();
			break;
		case 5:
			return;
			break;
		}
	} while (1);
}

// 将可读时间转为time_t
time_t convert_dateToTT(int yyyy, int mm, int dd, int hh, int min, int ss)
{
	struct tm timeT;
	memset(&timeT, 0, sizeof(timeT));
	timeT.tm_year = yyyy - 1900;	// 标准时间计算从1900开始，故先减
	timeT.tm_mon = mm - 1;			// 月份存储从0开始，故减1
	timeT.tm_mday = dd;
	timeT.tm_hour = hh;
	timeT.tm_min = min;
	timeT.tm_sec = ss;
	return mktime(&timeT);
}

// 获取选课状态 0为未开始选课，1为正在选课时间内，2为选课时间已结束
int getState_selecting() {
	time_t nowTime;
	nowTime = time(NULL);			// 获得当前时间
	if (nowTime - selecStart < 0)
		return 0;					// 当前时间比开始选课时间早时
	else if (nowTime - selecEnd > 0)
		return 2;					// 当前时间比结束选课时间晚时
	else
		return 1;					// 当前时间比结束选课时间早，比开始时间晚
}

// 输入开课时间与时间段返回是否开课,0为未开课，1为已开课
//输入格式为开课时间和上课时间段
int getState_starting(char* sweek, char* stime) {
	char tmp[50];
	int year, term, week, day, hr, min;
	time_t ttsTime, nowTime;

	sprintf(tmp, "%c%c%c%c", sweek[0], sweek[1], sweek[2], sweek[3]);
	year = atoi(tmp);

	if (year < currYear)		//开课年份小于当前学期则为已开，大于未开
		return 1;
	else if (year > currYear)
		return 0;

	sprintf(tmp, "%c%c", sweek[15], sweek[16]);
	if (!strcmp(tmp, "一"))
		term = 1;
	else
		term = 2;

	if (term < currTerm)		// 开课学期小于当前学期则为已开
		return 1;

	// 能到这的都是本学期的课，取出开课周数，若仍不为汉字取多一位
	if (sweek[24] > 127)
		sprintf(tmp, "%c", sweek[23]);
	else
		sprintf(tmp, "%c%c", sweek[23], sweek[24]);

	week = atoi(tmp);
	sprintf(tmp, "%c%c", stime[2], stime[3]);

	if (!strcmp(tmp, "一"))		// swtich不能以char数组做case，很无奈
		day = 1;
	else if (!strcmp(tmp, "二"))
		day = 2;
	else if (!strcmp(tmp, "三"))
		day = 3;
	else if (!strcmp(tmp, "四"))
		day = 4;
	else if (!strcmp(tmp, "五"))
		day = 5;
	else if (!strcmp(tmp, "六"))
		day = 6;
	else
		day = 7;

	if (stime[6] > '9')			// 第六位如果是冒号就只取一位，若不是全部顺延一位
	{
		sprintf(tmp, "%c", stime[5]);
		hr = atoi(tmp);
		sprintf(tmp, "%c%c", stime[7], stime[8]);
		min = atoi(tmp);
	}
	else {
		sprintf(tmp, "%c%c", stime[5], stime[6]);
		hr = atoi(tmp);
		sprintf(tmp, "%c%c", stime[8], stime[9]);
		min = atoi(tmp);
	}
	// 在开课时间的基础上加上间隔时间得到真实开课时间
	ttsTime = currStart + 604800LL * (week - 1LL) +
		86400LL * (day - 1LL)
		+ 3600LL * hr + 60LL * min;
	nowTime = time(NULL);
	if (ttsTime - nowTime > 0)
		return 0;
	else
		return 1;
}

//删掉并重新创建表
void table_init() {
	char query[1000];
	sprintf(query, "SET FOREIGN_KEY_CHECKS=0;");
	mysql_query(&mysql, query);
	sprintf(query, "DROP TABLE IF EXISTS `classes`;");
	mysql_query(&mysql, query);
	sprintf(query, "CREATE TABLE `classes` ("
		"`课程编号` varchar(20)  NOT NULL,"
		"`开课学院` varchar(255) DEFAULT NULL,"
		"`课程名称` varchar(255) DEFAULT NULL,"
		"`学分` float(255,1) DEFAULT NULL,"
		"`学时` float(255,1) DEFAULT NULL,"
		"`课程性质` varchar(255) DEFAULT NULL,"
		"`开课教师` varchar(255) DEFAULT NULL,"
		"`开课时间` varchar(255) DEFAULT NULL,"
		"`结课时间` varchar(255) DEFAULT NULL,"
		"`上课时间段` varchar(255) DEFAULT NULL,"
		"`上课地点` varchar(255) DEFAULT NULL,"
		"`限制人数` int DEFAULT NULL,"
		"`已选人数` int DEFAULT NULL,"
		"`教材信息` text CHARACTER SET utf8 COLLATE utf8_general_ci,"
		"`课程简介` text CHARACTER SET utf8 COLLATE utf8_general_ci,"
		"PRIMARY KEY (`课程编号`)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;");
	mysql_query(&mysql, query);
	sprintf(query, "DROP TABLE IF EXISTS `students`;");
	mysql_query(&mysql, query);
	sprintf(query, "CREATE TABLE `students` ("
		"`stuID` varchar(20) NOT NULL,"
		"`school` varchar(100) DEFAULT NULL,"
		"`major` varchar(255) DEFAULT NULL,"
		"`name` varchar(100) DEFAULT NULL,"
		"`sexual` varchar(10) DEFAULT NULL,"
		"`phone` varchar(20) DEFAULT NULL,"
		"`passwd` varchar(100) DEFAULT NULL,"
		"`email` varchar(100) DEFAULT NULL,"
		"`class1` varchar(255) DEFAULT NULL,"
		"`class2` varchar(255) DEFAULT NULL,"
		"`class3` varchar(255) DEFAULT NULL,"
		"PRIMARY KEY (`stuID`)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;");
	mysql_query(&mysql, query);
	sprintf(query, "DROP TABLE IF EXISTS `teachers`;");
	mysql_query(&mysql, query);
	sprintf(query, "CREATE TABLE `teachers` ("
		"`teachID` varchar(20) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,"
		"`school` varchar(100) DEFAULT NULL,"
		"`name` varchar(100) DEFAULT NULL,"
		"`passwd` varchar(100) DEFAULT NULL,"
		"`email` varchar(100) DEFAULT NULL,"
		"PRIMARY KEY (`teachID`)"
		") ENGINE=InnoDB DEFAULT CHARSET=utf8;");
	mysql_query(&mysql, query);
	change_color(2, 14);
	printf("\n初始化数据库表完成...\n");
}

// 若配置文件存在则读取，否则进行首次运行配置程序
void config_init() {
	FILE* inFile;
	char query[3000], * pr = query;
	char in[30] = "";
	int ini;
	inFile = fopen("cssystem.ini", "r");
	if (inFile != NULL)
	{
		readCFG();
		return;				// 如果配置文件存在则跳过首次使用设置部分，进入读配置部分
	}

	int ret = 0, flag = 0, flag2 = 0;
	do
	{
		flag = 0;
		sprintf(dbIP, "");
		sprintf(dbName, "");
		sprintf(dbUser, "");
		sprintf(dbPassWd, "");
		sprintf(in, "");
		system("title 学生选课管理系统 - 首次使用设置系统");
		change_color(5, 14);
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t\t○●○●○● 欢迎使用学生选课管理系统 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		change_color(1, 14);
		printf("在开始使用之前，请先对系统进行设置。\n\n\n数据库部分（将在全部输入后检查连接可用性）——\n");
		printf("\n请输入MySQL数据库地址（留空默认为127.0.0.1）：");

		s_gets(dbIP, 20);
		if (!strcmp(dbIP, ""))
			sprintf(dbIP, "127.0.0.1");

		change_color(1, 14);
		printf("请输入MySQL数据库端口号（留空默认为3306）：");
		s_gets(in, 20);
		if (!strcmp(in, ""))
			sprintf(in, "3306");
		dbPort = atoi(in);

		change_color(1, 14);
		printf("请输入MySQL数据库用户名（留空默认为root）：");
		s_gets(dbUser, 20);
		if (!strcmp(dbUser, ""))
			sprintf(dbUser, "root");

		change_color(1, 14);
		printf("请输入MySQL数据库密码：");
		s_gets(dbPassWd, 20);
		if (!strcmp(dbPassWd, ""))
			sprintf(dbPassWd, "123456");

		change_color(1, 14);
		printf("请输入MySQL数据库库名称（留空默认为courseselectionsystem）：");
		s_gets(dbName, 30);
		if (!strcmp(dbName, ""))
			sprintf(dbName, "courseselectionsystem");

		if (!mysql_real_connect(&mysql, dbIP, dbUser, dbPassWd, dbName, dbPort, NULL, 0))
		{
			change_color(4, 14);
			printf("\n\n数据库连接失败！请确认配置是否正确，按回车重新配置……\n");
			flag = 1;
			continue;
		}
		if (!mysql_set_character_set(&mysql, "gbk"))
			mysql_character_set_name(&mysql);

		sprintf(query, "select name from `teachers`");	
		if (!mysql_query(&mysql, query)) { //通过select是否错误来判断是否已经存在表
			do
			{
				mysql_store_result(&mysql);
				flag2 = 0;
				change_color(4, 14);
				printf("数据库中已有表，请选择是否删除以初始化表头（1：初始化/2：保留内容）（输入1或2）：");
				ini = _getch();
				if (ini == '1')
				{
					printf("初始化\n");
					table_init();
					continue;
				}
				else if (ini == '2')
				{
					printf("保留内容\n");
					continue;
				}
				else
				{
					flag2 = 1;
					change_color(4, 14);
					printf("\n无效，请重新输入！");
				}

			} while (flag2);
		}
		else {
			table_init();
		}
		mysql_store_result(&mysql);
		mysql_close(&mysql);
	} while (flag);


	do {
		flag = 0;
		change_color(1, 14);
		printf("\n\n学期部分——\n");
		printf("当前学年（输入0-8）：202");
		ini = _getch();
		if (ini <= '8' && ini >= '0')
		{
			printf("%c\n", ini);
			currYear = ini - 48;
			continue;
		}
		else
		{
			change_color(4, 14);
			printf("\n无效，请重新输入！\n");
			flag = 1;
			continue;
		}

	} while (flag);

	do
	{
		flag = 0;
		change_color(1, 14);
		printf("当前学期（输入1或2）：202%d-202%d学年第  学期\b\b\b\b\b\b",currYear,currYear+1);
		ini = _getch();
		if (ini == '1')
		{
			printf("一\n");
			currTerm = 1;
			continue;
		}
		else if (ini == '2')
		{
			printf("二\n");
			currTerm = 2;
			continue;
		}
		else
		{
			flag = 1;
			change_color(4, 14);
			printf("无效，请重新输入！\n");
		}

	} while (flag);

	int date[5];
	char tmp[20];
	char* p;		//用于const化输入字符串使用sscanf代替scanf
	do {
		flag = 0;
		change_color(1, 14);
		printf("输入当前学期开学时间（格式yyyy-mm-dd，如输入2020-8-31，必须为周一）：");
		s_gets(tmp, 19);
		p = tmp;
		ret = sscanf(p,"%d-%d-%d", &date[0], &date[1], &date[2]);
		if(ret != 3)
		{

			change_color(4, 14);
			printf("无效格式，请重新输入。\n");
			flag = 1;
			continue;

		}
		if (date[0] < 2020 || date[0]>2029 || date[1] > 12
			|| date[1] < 1 || date[2] < 1 || date[2]>31)
		{
			flag = 1;
			change_color(4, 14);
			printf("不符合日期规范，请重新输入。\n");
			continue;
		}

		currStart = convert_dateToTT(date[0], date[1], date[2], 0, 0, 0);
		strftime(tmp, sizeof(tmp), "%A", localtime(&currStart));
		if (strcmp(tmp, "Monday"))
		{
			flag = 1;
			change_color(4, 14);
			printf("该日不是周一，请重新输入。\n");
		}

	} while (flag);

	do {
		flag = 0;
		change_color(1, 14);
		printf("输入当前选课开始时间（格式yyyy-mm-dd hh:mm，如输入2020-8-31 9:00）：");
		s_gets(tmp, 19);
		p = tmp;
		ret = sscanf(p,"%d-%d-%d %d:%d"
			, &date[0], &date[1], &date[2], &date[3], &date[4]);
		if (ret != 5)
		{
			change_color(4, 14);
			printf("无效格式，请重新输入。\n");
			flag = 1;
			continue;

		}
		if (date[0] < 2020 || date[0]>2029 || date[1] > 12 || date[1] < 1
			|| date[2] < 1 || date[2]>31 || date[3] < 0 || date[3]>23
			|| date[4] < 0 || date[4]>59)
		{
			flag = 1;
			change_color(4, 14);
			printf("不符合日期规范，请重新输入。\n");
			continue;
		}
		selecStart = convert_dateToTT(
			date[0], date[1], date[2], date[3], date[4], 0);
	} while (flag);

	do {
		flag = 0;
		change_color(1, 14);
		printf("输入当前选课结束时间（格式yyyy-mm-dd-hh:mm，如输入2020-8-31 9:00）：");
		s_gets(tmp, 19);
		p = tmp;
		ret = sscanf(p, "%d-%d-%d %d:%d"
			, &date[0], &date[1], &date[2], &date[3], &date[4]);
		if (ret != 5)
		{
			change_color(4, 14);
			printf("无效格式，请重新输入。\n");
			flag = 1;
			continue;
		}
		if (date[0] < 2020 || date[0]>2029 || date[1] > 12 || date[1] < 1
			|| date[2] < 1 || date[2]>31 || date[3] < 0 || date[3]>23
			|| date[4] < 0 || date[4]>59)
		{
			flag = 1;
			change_color(4, 14);
			printf("\n不符合日期规范，请重新输入。");
			continue;
		}

		selecEnd = convert_dateToTT(date[0], date[1], date[2], date[3], date[4], 0);
		if (selecEnd < selecStart) {
			flag = 1;
			change_color(4, 14);
			printf("\n选课结束时间不能早于开课时间，请重新输入。");
			continue;
		}
	} while (flag);

	FILE* outFile;
	if ((outFile = fopen("cssystem.ini", "w")) == NULL)
	{
		change_color(4, 14);
		printf("\n打开配置文件失败！请尝试手动删除cfg配置文件后重试！");
		return;
	}
	char enpw[50];
	strcpy(enpw, dbPassWd);
	pw_encode(enpw);
	fprintf(outFile, "# 学生选课管理系统配置文件\n# 手动修改请遵从文件格式（包括行数）\n\n# 数据库地址\n%s\n\n# 端口号\n%d\n\n# 用户名\n%s\n\n# 密码\n%s\n\n# 库名\n%s\n\n# 当前学期信息（学年、学期、开学时间、选课开始时间、结束时间）\n202%d\n%d\n%ld\n%ld\n%ld", dbIP, dbPort, dbUser, enpw, dbName, currYear, currTerm, (long)currStart, (long)selecStart, (long)selecEnd);
	fclose(outFile);
	change_color(1, 14);
	printf("\n\n首次使用设置完毕，按任意键开始使用系统……");
	system("pause>nul");
}

// 读取配置文件
void readCFG() {
	FILE* inFile;
	char record[50] = { 0 };
	int lineNum = 0;		//当前行数
	if ((inFile = fopen("cssystem.ini", "r")) == NULL)
	{
		change_color(4, 14);
		printf("\n打开配置文件失败！请尝试手动删除cfg配置文件并执行首次使用设置！");
		getchar();
		exit(1);
	}
	while (!feof(inFile)) {
		lineNum++;
		memset(record, 0, sizeof(record));
		fgets(record, sizeof(record) - 1, inFile);
		char* pos;
		pos = strchr(record, '\n');
		if (pos) 
			*pos = '\0';	//把取出的换行符替换为结束
		switch (lineNum) {
		case 5:
			sprintf(dbIP, record);
			break;
		case 8:
			dbPort = atoi(record);
			break;
		case 11:
			sprintf(dbUser, record);
			break;
		case 14:
			pw_decode(record);
			sprintf(dbPassWd, record);
			break;
		case 17:
			sprintf(dbName, record);
			break;
		case 21:			//很奇怪为什么到20后行数需要加一
			currYear = atoi(record);
			break;
		case 22:
			currTerm = atoi(record);
			break;
		case 23:
			currStart = atol(record);
			break;
		case 24:
			selecStart = atol(record);
			break;
		case 25:
			selecEnd = atol(record);
			break;
		default:
			break;
		}
	}
	fclose(inFile);
}

void teacher_login() {
	char query[100];
	char password[100];
	HANDLE handle;
	do {
		system("cls");
		mysql_store_result(&mysql);
		int option1;
		system("title 学生选课管理系统 - 教师登录");
		change_color(5, 14);
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t\t○●○●○● 欢迎登录学生选课管理系统 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		change_color(1, 14);
		printf("\n\t\t\t\t\t\t请选择您的身份:\n\n");
		printf("\t\t\t\t\t  ┏━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		printf("\t\t\t\t\t  ┃       ① - 登录       ┃\n");
		printf("\t\t\t\t\t  ┗━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
		printf("\t\t\t\t\t  ┏━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		printf("\t\t\t\t\t  ┃       ② - 注册       ┃\n");
		printf("\t\t\t\t\t  ┗━━━━━━━━━━━━━━━━━━━━━━━┛\n\n");
		printf("\t\t\t\t\t  ┏━━━━━━━━━━━━━━━━━━━━━━━┓\n");
		printf("\t\t\t\t\t  ┃     ③ - 返回上层     ┃\n");
		printf("\t\t\t\t\t  ┗━━━━━━━━━━━━━━━━━━━━━━━┛\n\n\n");
		printf("\t\t\t\t\t 请输入1,2,3或直接点击相应标题:");

		sprintf(pos, "6-8,44-67|10-12,44-67|14-16,44-67|1--1,2-17|1--1,2-17|1--1,2-17");
		handle = CreateThread(NULL, 0, SelectEventThread, NULL, 0, NULL);

		scanf_opt(&option1, 1, 3);

		TerminateThread(handle, 1);
		if (option1 == 1)
		{
			int flag = 0;
			system("cls");			// 清屏，保证重复输入时美观
			system("title 学生选课管理系统 - 教师登录");
			change_color(5, 14);
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\t\t\t\t○●○●○● 欢迎登录学生选课管理系统 ●○●○●○\n");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			do {
				flag = 0;
				change_color(1, 14);
				printf("请输入用户名：");
				change_color(1, 14);
				s_gets(teachID, 11);
				if (teachID[0] == 17)//判断若输入首字符为ctrl+q则返回上层
				{
					system("cls");
					teacher_login();
					return;
				}
				if (!check_teachId(teachID))
				{
					change_color(4, 14);
					printf("学号格式错误！请重试！(若返回上一级，请按Ctrl+Q后回车)\n");
					flag = 1;
					continue;
				}
				change_color(1, 14);
				printf("请输入密码：");
				scanf_pw(password);
				if (password[0] == 17)//判断若输入首字符为ctrl+q则返回上层
				{
					system("cls");
					teacher_login();
					return;
				}
				if (!check_password(1, teachID, password))
				{
					change_color(4, 14);
					printf("用户名或密码错误！请重试！(若返回上一级，请按Ctrl+Q后回车)\n");
					flag = 1;
					continue;
				}
			} while (flag);
			mysql_store_result(&mysql);
			sprintf(query, "select school,name from teachers where teachID='%s'", teachID);
			mysql_query(&mysql, query);
			result = mysql_store_result(&mysql);
			if (result)
			{										// 防止数据为空造成崩溃
				if ((int)mysql_num_rows(result) == 1)	// 若非有且仅有一行数据则登录失败
				{
					Row = mysql_fetch_row(result);
					sprintf(nowName, Row[1]);
					sprintf(nowSchool, Row[0]);
				}
				teacher_mainmenu();
			}
		}
		else if (option1 == 2)
		{
			teacher_reg();
			continue;
		}
		else if (option1 == 3)
			return;
	} while (1);
}

void teacher_course_list()
{
	char classID[100];
	system("cls");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t\t○●○●○● 课程查询 - 教师 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n全部课程如下：\n");
	char query3[200] = "select 课程编号,开课学院,课程名称,学分,学时,课程性质,开课教师,教材信息,";
	strcat(query3, "学分,学分,学分,学分,学分,学分,学分 from classes");
	change_color(0, 14);
	print_class(query3);
	change_color(1, 14);
	printf("输入课程编号以查看该课程的详细信息\n");
	s_gets(classID, 11);
	do {
		char query10[100] = "select * from classes where 课程编号='";
		strcat(query10, classID);
		strcat(query10, "'");
		mysql_store_result(&mysql);
		mysql_query(&mysql, query10);
		result = mysql_store_result(&mysql);
		if ((int)mysql_num_rows(result) == 0)
		{
			change_color(4, 14);
			printf("无此课程，请重新输入！(若返回上一级，请按Ctrl+Q后回车)\n");
			change_color(1, 14);
			s_gets(classID, 11);
			if (classID[0] == 17)
			{
				system("cls");
				return;
			}
		}
	} while ((int)mysql_num_rows(result) == 0);
	char query[200] = "select 开课时间,结课时间,上课时间段,上课地点,限制人数,已选人数 from classes where 课程编号='";
	strcat(query, classID);
	strcat(query, "'");
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	if (result)
	{
		int fieldCount = mysql_field_count(&mysql);
		if (fieldCount > 0)
		{
			change_color(0, 14);
			int row = (int)mysql_num_rows(result);
			int column = mysql_num_fields(result);
			for (int i = 0; field = mysql_fetch_field(result); i++) {
				//获得属性名 
				if (i == 0 || i == 1)
					printf("%-27s |", field->name);
				else if (i == 2)
					printf("%-17s |", field->name);
				else if (i == 3 || i == 4 || i == 5)
					printf("%-8s |", field->name);
				else
					printf(" %-20s|", field->name);
			}
			printf("\n");
			while (Row = mysql_fetch_row(result)) {
				for (int j = 0; j < column; j++)
				{
					if (j == 0 || j == 1)
						printf("%-27s |", Row[j]);
					else if (j == 2)
						printf("%-17s |", Row[j]);
					else if (j == 3 || j == 4 || j == 5)
						printf("%-8s |", Row[j]);
					else
						printf("%-20s |", Row[j]);
				}
				printf("\n");
			}
		}
		change_color(1, 14);
	}

	char query1[200] = "select 教材信息 from classes where 课程编号='";
	strcat(query1, classID);
	strcat(query1, "'");
	mysql_query(&mysql, query1);
	result = mysql_store_result(&mysql);
	if (result)
	{
		change_color(0, 14);
		int fieldCount = mysql_field_count(&mysql);
		if (fieldCount > 0)
		{
			field = mysql_fetch_field(result);
			//获得属性名 
			printf("%s:", field->name);
			printf("\n");
			Row = mysql_fetch_row(result);
			printf("%s", Row[0]);
			printf("\n\n");
			change_color(1, 14);
		}
	}

	char query2[200] = "select 课程简介 from classes where 课程编号='";
	strcat(query2, classID);
	strcat(query2, "'");
	mysql_query(&mysql, query2);
	result = mysql_store_result(&mysql);
	if (result)
	{
		int fieldCount = mysql_field_count(&mysql);
		if (fieldCount > 0)
		{
			change_color(0, 14);
			field = mysql_fetch_field(result);
			//获得属性名 
			printf("%s:", field->name);
			printf("\n");
			Row = mysql_fetch_row(result);
			printf("%s", Row[0]);
			printf("\n\n");
			change_color(1, 14);
		}
	}
	change_color(1, 14);
	printf("\n按任意键返回上一菜单...\n");
	system("pause>nul");
	return;
}

void teacher_course_edit() {
	system("cls");
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t\t○●○●○● 开设课程修改界面 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	change_color(1, 14);
	printf("以下是您开设的课程\n\n");
	char query[600];
	int stu_num;
	int flag = 0;
	int option2 = 0;
	char courseName[200];
	sprintf(query, "SELECT * FROM `classes`WHERE 开课教师 = '%s'", nowName);
	change_color(0, 14);
	print_class(query);//打印相应查询内容
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	change_color(1, 14);
	printf("\n请选择您需要的服务:\n\n");
	printf("  ① - 修改课程内容\n");
	printf("  ② - 结束修改并返回上一菜单\n\n");
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请输入1或2:");
	change_color(1, 14);
	scanf_opt(&option2, 1, 2);
	switch (option2)
	{
	case 1:
		do {
			change_color(1, 14);
			printf("请输入您想要修改的课程的编号：");
			s_gets(courseName,20);
			if (courseName[0] == 17)//判断若输入首字符为ctrl+q则返回上层
			{
				system("cls");
				teacher_course_edit();
				return;
			}
			sprintf(query, "SELECT 课程编号,课程名称,开课教师,限制人数 FROM `classes`WHERE 课程编号 = '%s'", courseName);
			mysql_query(&mysql, query);
			result = mysql_store_result(&mysql);
			if ((int)mysql_num_rows(result) == 0)
			{
				flag = 1;
				change_color(4, 14);
				printf("无结果，请重新输入(若返回上一级，请按Ctrl+Q后回车)\n");
				mysql_free_result(result);
			}
			else 
				flag = 0;
		} while (flag == 1);
		change_color(0, 14);
		if (result)
		{
			int fieldCount = mysql_field_count(&mysql);
			if (fieldCount > 0)
			{
				change_color(0, 14);
				int row = (int)mysql_num_rows(result);
				int column = mysql_num_fields(result);
				for (int i = 0; field = mysql_fetch_field(result); i++) {
					//获得属性名 
					if (i == 0 || i == 1)
						printf("%-27s |", field->name);
					else if (i == 2)
						printf("%-17s |", field->name);
					else if (i == 3 || i == 4 || i == 5)
						printf("%-8s |", field->name);
					else
						printf(" %-20s|", field->name);
				}
				printf("\n");
				while (Row = mysql_fetch_row(result)) {
					for (int j = 0; j < column; j++)
					{
						if (j == 0 || j == 1)
							printf("%-27s |", Row[j]);
						else if (j == 2)
							printf("%-17s |", Row[j]);
						else if (j == 3 || j == 4 || j == 5)
							printf("%-8s |", Row[j]);
						else
							printf("%-20s |", Row[j]);
					}
					printf("\n");
				}
			}
		}

		printf("教材信息:\n");
		sprintf(query, "select 教材信息 from classes where 课程编号='%s'", courseName);
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);

		if (result)
		{
			if (Row = mysql_fetch_row(result))
				printf("%s\n\n", Row[0]);
		}

		printf("课程信息:\n");
		sprintf(query, "select 课程简介 from classes where 课程编号='%s'", courseName);
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);
		if (result)
		{
			Row = mysql_fetch_row(result);
			printf("%s", Row[0]);
			printf("\n\n");
			change_color(1, 14);
		}

		change_color(1, 14);
		sprintf(query, "SELECT `已选人数` FROM `classes`WHERE 课程编号 = '%s'", courseName);
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);
		Row = mysql_fetch_row(result);
		stu_num = atoi(Row[0]);
		if (stu_num == 0)
		{
			change_color(5, 14);
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			change_color(1, 14);
			printf("\n未有学生选课，请选择您需要的修改的内容:\n");
			printf("  ① - 教材信息\n");
			printf("  ② - 课程简介\n");
			printf("  ③ - 限制人数\n\n");
			printf("  ④ - 返回上一个菜单\n\n");
			change_color(5, 14);
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			change_color(1, 14);
			printf("请输入1，2，3或4:");
			scanf_opt(&option2, 1, 4);
			char content[1000];
			switch (option2)
			{
			case 1:
				change_color(1, 14);
				printf("教材信息修改\n");
				printf("请输入您的修改内容：");
				s_gets(content, 20);
				sprintf(query, "UPDATE `classes` SET `教材信息`='%s' WHERE`课程编号`='%s'"
					, content, courseName);
				mysql_query(&mysql, query);
				change_color(2, 14);
				printf("修改成功！\n");
				change_color(1, 14);
				printf("按任意键返回上一菜单...\n");
				system("pause>nul");
				teacher_course_edit();
				return;
				break;

			case 2:
				change_color(1, 14);
				printf("课程简介修改\n");
				printf("请输入您的修改内容：");
				s_gets(content,499);
				sprintf(query, "UPDATE `classes` SET `课程简介`='%s' WHERE`课程编号`='%s'"
					, content, courseName);
				mysql_query(&mysql, query);
				change_color(2, 14);
				printf("修改成功！\n");
				change_color(1, 14);
				printf("按任意键返回上一菜单...\n");
				system("pause>nul");
				teacher_course_edit();
				return;
				break;

			case 3:
				change_color(1, 14);
				printf("\n限制人数修改\n");
				sprintf(query, "SELECT `限制人数` FROM `classes`WHERE 课程编号 = '%s'"
					, courseName);
				mysql_query(&mysql, query);
				result = mysql_store_result(&mysql);
				Row = mysql_fetch_row(result);
				stu_num = atoi(Row[0]);
				switch (stu_num)
				{
				case 80:
					change_color(5, 14);
					printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
					change_color(1, 14);
					printf("您目前的限制人数为80人，是否修改成100人？\n");
					printf("  ① - 是\n");
					printf("  ② - 否\n\n");
					change_color(5, 14);
					printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
					change_color(1, 14);
					printf("\n请输入1或2:");
					scanf_opt(&option2, 1, 2);
					switch (option2)
					{
					case 1:
						sprintf(query, "UPDATE `classes` SET `限制人数`='100' WHERE`课程编号`='%s'"
							, courseName);
						mysql_query(&mysql, query);
						change_color(2, 14);
						printf("修改成功！\n");
						change_color(1, 14);
						printf("按任意键返回上一菜单...\n");
						system("pause>nul");
						teacher_course_edit();
						return;
						break;
					case 2:
						teacher_course_edit();
						return;
						break;
					default:
						change_color(4, 14);
						printf("无效，请重新输入!\n");
					}
					break;
				case 100:
					change_color(5, 14);
					printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
					change_color(1, 14);
					printf("您目前的限制人数为100人，是否修改成80人？\n");
					printf("  ① - 是\n");
					printf("  ② - 否\n\n");
					change_color(5, 14);
					printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
					change_color(1, 14);
					printf("\n请输入1或2:");
					scanf_opt(&option2, 1, 2);
					switch (option2)
					{
					case 1:
						sprintf(query, "UPDATE `classes` SET `限制人数`='80' WHERE`课程编号`='%s'"
							, courseName);
						mysql_query(&mysql, query);
						change_color(2, 14);
						printf("修改成功！\n");
						change_color(1, 14);
						printf("按任意键返回上一菜单...\n");
						system("pause>nul");
						teacher_course_edit();
						return;
						break;
					case 2:
						teacher_course_edit();
						return;
						break;
					default:
						change_color(4, 14);
						printf("无效，请重新输入!\n");
					}
					break;
				}
			case 4:
				return;
				break;
			default:
				change_color(4, 14);
				printf("无效，请重新输入!\n");
			}
		}
		else
		{
			change_color(4, 14);
			printf("您选择的该课程已有学生选择，只能修改限制人数");
			change_color(1, 14);
			printf("\n限制人数修改\n");
			sprintf(query, "SELECT `限制人数` FROM `classes`WHERE 课程编号 = '%s'"
				, courseName);
			mysql_query(&mysql, query);
			result = mysql_store_result(&mysql);
			Row = mysql_fetch_row(result);
			stu_num = atoi(Row[0]);

			switch (stu_num) {
			case 80:
				change_color(5, 14);
				printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
				change_color(1, 14);
				printf("您目前的限制人数为80人，是否修改成100人？\n");
				printf("  ① - 是\n");
				printf("  ② - 否\n\n");
				change_color(5, 14);
				printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
				change_color(1, 14);
				printf("\n请输入1或2:");
				scanf_opt(&option2, 1, 2);
				switch (option2)
				{
				case 1:
					sprintf(query, "UPDATE `classes` SET `限制人数`='100' WHERE`课程编号`='%s'"
						, courseName);
					mysql_query(&mysql, query);
					change_color(2, 14);
					printf("修改成功！\n");
					change_color(1, 14);
					printf("按任意键返回上一菜单...\n");
					system("pause>nul");
					teacher_course_edit();
					return;
					break;
				case 2:
					teacher_course_edit();
					return;
					break;
				default:
					change_color(4, 14);
					printf("无效，请重新输入!\n");
				}
				break;
			case 100:
				sprintf(query, "SELECT `已选人数` FROM `classes`WHERE 课程编号 = '%s'"
					, courseName);
				mysql_query(&mysql, query);
				result = mysql_store_result(&mysql);
				Row = mysql_fetch_row(result);
				if (atoi(Row[0]) > 80)
				{
					change_color(4, 14);
					printf("非常抱歉，您的课程已选人数已经超过80人，无法修改限制人数\n");
					change_color(1, 14);
					printf("按任意键返回上一菜单...\n");
					system("pause>nul");
					teacher_course_edit();
					return;
				}
				else {
					change_color(5, 14);
					printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
					change_color(1, 14);
					printf("您目前的限制人数为100人，是否修改成80人？\n");
					printf("  ① - 是\n");
					printf("  ② - 否\n\n");
					change_color(5, 14);
					printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
					change_color(1, 14);
					printf("\n请输入1或2:");
					scanf_opt(&option2, 1, 2);
					switch (option2)
					{
					case 1:
						sprintf(query, "UPDATE `classes` SET `限制人数`='80' WHERE`课程编号`='%s'"
							, courseName);
						mysql_query(&mysql, query);
						change_color(2, 14);
						printf("修改成功！\n");
						change_color(1, 14);
						printf("按任意键返回上一菜单...\n");
						system("pause>nul");
						teacher_course_edit();
						return;
						break;
					case 2:
						teacher_course_edit();
						return;
						break;
					default:
						change_color(4, 14);
						printf("无效，请重新输入!\n");
					}
				}
				break;
			default:
				change_color(4, 14);
				printf("无效，请重新输入!\n");
			}
		}
		break;
	case 2:
		return;
		break;
	default:
		change_color(4, 14);
		printf("无效，请重新输入!\n");
	}
}

void teacher_course_delete() {
	system("cls");
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t\t○●○●○● 开设课程删除界面 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	change_color(1, 14);
	printf("以下是您开设的课程\n");
	int flag = 0;
	int option2 = 0;
	char courseName[200];
	char query[200] = "SELECT * FROM `classes`WHERE 开课教师 = '";
	strcat(query, nowName);
	strcat(query, "'");
	change_color(0, 14);
	print_class(query);//打印相应查询内容
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	change_color(1, 14);
	printf("请选择您需要的服务:\n\n");
	printf("  ① - 进行课程的删除\n");
	printf("  ② - 返回上一菜单\n\n");
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	change_color(1, 14);
	printf("\n请输入1或2:");
	scanf_opt(&option2, 1, 2);
	switch (option2)
	{
	case 1:
		if (getState_selecting() != 0)
		{
			change_color(4, 14);
			printf("非常抱歉，选课已开始，您不能进行删除操作\n");
			change_color(1, 14);
			printf("按任意键返回上一菜单...\n");
			system("pause>nul");
			teacher_course_delete();
			return;
		}
		do {
			change_color(1, 14);
			printf("请输入您想要删除的课程的编号：");
			s_gets(courseName, 20);
			if (courseName[0] == 17)//判断若输入首字符为ctrl+q则返回上层
			{
				system("cls");
				teacher_course_delete();
				return;
			}
			sprintf(query, "SELECT * FROM `classes`WHERE 课程编号 = '%s'", courseName);
			mysql_query(&mysql, query);
			result = mysql_store_result(&mysql);
			if ((int)mysql_num_rows(result) == 0)
			{
				flag = 1;
				change_color(4, 14);
				printf("无结果，请重新输入(若返回上一级，请按Ctrl+Q后回车)\n");
				mysql_free_result(result);
			}
			else
				flag = 0;
		} while (flag == 1);
		change_color(0, 14);
		print_class(query);

		change_color(5, 14);
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		change_color(1, 14);
		printf("确认要删除该课程吗？\n");
		printf("  ① - 是\n");
		printf("  ② - 否\n\n");
		change_color(5, 14);
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		change_color(1, 14);
		printf("\n请输入1或2:");
		scanf_opt(&option2, 1, 2);
		switch (option2)
		{
		case 1:
			sprintf(query, "DELETE FROM `classes` WHERE `课程编号` = '%s'"
				, courseName);
			mysql_query(&mysql, query);
			change_color(2, 14);
			printf("删除成功！\n");
			change_color(1, 14);
			printf("按任意键返回上一菜单...\n");
			system("pause>nul");
			teacher_course_delete();
			return;
			break;
		case 2:
			teacher_course_delete();
			return;
			break;
		default:
			change_color(4, 14);
			printf("无效，请重新输入!\n");
		}
		break;
	case 2:
		return;
		break;
	default:
		change_color(4, 14);
		printf("无效，请重新输入!\n");
	}
}

void teacher_manage_info()
{
	HANDLE handle;
	do {
		system("cls");
		int option2;
		char passwd[20];
		char passwd1[20];
		char email[50];
		char query[200];
		system("title 学生选课管理系统 - 教师信息修改");
		change_color(5, 14);
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t\t○●○●○● 信息修改界面--教师 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		change_color(1, 14);
		printf("\n\t\t\t\t           请选择需要修改的信息：\n\n");
		printf("\t\t\t\t                【 ① 密码 】\n\n");
		printf("\t\t\t\t                【 ② 邮箱 】\n\n\n");
		printf("\t\t\t\t           【 ③ 返回上一个菜单 】\n\n");
		printf("\n请输入1，2，3或直接点击相应标题：");

		sprintf(pos, "6-6,48-60|8-8,48-60|11-11,43-65|1-1,43-65|1--1,2-17|1--1,2-17");
		handle = CreateThread(NULL, 0, SelectEventThread, NULL, 0, NULL);
		scanf_opt(&option2, 1, 3);
		TerminateThread(handle, 1);
		switch (option2)
		{
		case 1:
		{
			system("cls");
			change_color(5, 14);
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\t\t\t\t○●○●○● 信息修改界面--教师--密码 ●○●○●○\n");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			change_color(1, 14);
			printf("\n请输入新的密码：");
			scanf_pw(passwd);
			printf("请再次确认新的密码：");
			scanf_pw(passwd1);

			while (strcmp(passwd, passwd1) != 0)
			{
				if (passwd1[0] == 17)//判断若输入首字符为ctrl+q则返回上层
				{
					system("cls");
					teacher_manage_info();
					return;
				}
				else
				{
					change_color(4, 14);
					printf("两次输入不一致!请重新确认:(若返回上一级，请按Ctrl+Q后回车)");
					s_gets(passwd1, 20);
				}
			}
			pw_encode(passwd);
			sprintf(query, "update teachers set passwd='%s' where teachID='%s'", passwd, teachID);
			mysql_query(&mysql, query);
			change_color(2, 14);
			printf("\n修改成功!\n");
			change_color(1, 14);
			printf("按任意键返回上一菜单...\n");
			system("pause>nul");
			teacher_manage_info();
			return;
			break;
		}
		case 2:
		{
			system("cls");
			change_color(5, 14);
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\t\t\t\t○●○●○● 信息修改界面--学生--邮箱 ●○●○●○\n");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			change_color(1, 14);
			printf("\n请输入新的邮箱：");
			s_gets(email, 20);
			while (check_email(email) == 0)
			{
				if (email[0] == 17)//判断若输入首字符为ctrl+q则返回上层
				{
					system("cls");
					teacher_manage_info();
					return;
				}
				else
				{
					change_color(4, 14);
					printf("无效输入！请按照***@***.***格式输入:");
					change_color(1, 14);
					s_gets(email, 20);
				}
			}
			sprintf(query, "update teachers set email='%s' where teachID='%s'", email, teachID);
			mysql_query(&mysql, query);	//将更改后的邮箱存至mysql数据库
			change_color(2, 14);
			printf("\n修改成功!\n");
			change_color(1, 14);
			printf("按任意键返回上一菜单...\n");
			system("pause>nul");
			teacher_manage_info();
			return;
			break;
		}
		case 3:
		{
			return;
			break;
		}
		default:
			change_color(4, 14);
			printf("无效，请重新输入!\n");
		}
	}while(1);
}

// 判断教师id是否符合10位数字，符合返回1
int check_teachId(char* str)
{
	int b = 1, y = 0;
	while (str[y] != '\0')
		y++;
	if (y != 10)
		b = 0;
	for (int y = 0; y < 10; y++)
	{
		if (str[y] < '0' || str[y]>'9')
			b = 0;
	}
	return b;
}

//快捷验证sql语句能否查出现有课与之有冲突，有则返回1
int check_classClash(char* query) {
	mysql_store_result(&mysql);		//清除数据防止出错
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	if ((int)mysql_num_rows(result) != 0)
	{
		change_color(4, 14);
		printf("\n与现有课程冲突！\n");
		return 1;
	}
	return 0;
}

//加课
void teacher_course_add()
{
	system("cls");
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t\t○●○●○● 课程添加界面 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	char classId[20], name[21], credit[10], learnTime[10], property[10]
		, startTime[100], endTime[100], time[100], classroom[20]
		, limit[5], intro[500], book[21];
	char term[50];
	char query[1000];
	float in_f;
	int in, in1,ret;
	int reflag = 0;					//部分语句块变为一就重新执行的标志
	int numClass1, numClass2;		//[1]为必修，[2]为选修课数量
	char in_s[20];
	change_color(1, 14);
	printf("\n请选择开课时间——\n");
	do {
		reflag = 0;
		change_color(1, 14);
		printf("\n学年部分（输入0-8）：202");
		in = _getch();
		if (in <= '9' && in >= '0')
		{
			printf("%c\n", in);
			in -= 48;
			sprintf(startTime, "202%d-202%d学年第", in, in + 1);
			continue;
		}
		else
		{
			change_color(4, 14);
			printf("\n无效，请重新输入！\n");
			reflag=1;
			continue;
		}

	} while (reflag);
	
	do
	{
		reflag = 0;
		change_color(1, 14);
		printf("\n开课学期（输入1或2）：202%d-202%d学年第  学期\b\b\b\b\b\b", in, in + 1);
		in = _getch();
		if (in == '1')
		{
			change_color(1, 14);
			printf("一\n");
			strcat(startTime, "一学期");
			continue;
		}
		else if (in == '2')
		{
			change_color(1, 14);
			printf("二\n");
			strcat(startTime, "二学期");
			continue;
		}
		else 
		{
			reflag = 1;
			change_color(4, 14);
			printf("\n无效，请重新输入！");
		}
		
	} while (reflag);

	mysql_store_result(&mysql);				//清除数据防止出错
	sprintf(query, "select 课程编号,课程性质 from classes where "
		"开课教师='%s' and 课程性质='必修' and 开课时间 like '%s"
		, nowName, startTime);
	strcat(query, "%'");
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	numClass1 = (int)mysql_num_rows(result);		// 取所选学期必修课已开数
	sprintf(query, "select 课程编号,课程性质 from classes where "
		"开课教师='%s' and 课程性质='选修' and 开课时间 like '%s"
		, nowName, startTime);
	strcat(query, "%'");
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	numClass2 = (int)mysql_num_rows(result);		// 取所选学期选修课已开数

	strcpy(term, startTime);				//拷贝一份作为开课学期

	change_color(1, 14);
	printf("您在 %s 已经开了 %d 门必修课、 %d 门选修课，还能再开 %d 门选修课\n"
		, startTime, numClass1, numClass2, 2 - numClass2);

	change_color(1, 14);
	printf("\n选择课程性质：");
	printf("\n1、必修\n2、选修\n\n请选择本课程性质(1/2）:");
	do
	{
		reflag = 0;
		in = _getch();
		if (in == '1')
		{
			change_color(1, 14);
			printf("必修\n");
			sprintf(property, "%s", "必修");
			continue;
		}
		else if (in == '2')
		{
			if (numClass2 > 1)
			{
				change_color(4, 14);
				printf("您本学期的选修课开课数量已达上限！\n");
				reflag = 1;
				continue;
			}
			change_color(1, 14);
			printf("选修\n");
			sprintf(property, "%s", "选修");
			continue;
		}
		else
		{
			reflag = 1;
			change_color(4, 14);
			printf("\n无效，请重新输入！");
		}

	} while (reflag);

	do
	{
		change_color(1, 14);
		printf("请输入课程编号（6位数字）：");
		s_gets(classId,11);
		while (!check_classId(classId))
		{
			change_color(4, 14);
			printf("输入无效！请重新输入：");
			change_color(1, 14);
			s_gets(classId, 11);
		}
		//准备验证是否有相同ID的课
		sprintf(query, "select 课程性质 from classes where "
			"课程编号='%s'", classId);
	} while (check_classClash(query));

	do
	{
		change_color(1, 14);
		printf("请输入课程名称：");
		s_gets(name, 20);
		//准备验证是否有相同名字的课
		sprintf(query, "select 课程性质 from classes where "
			"课程名称='%s'", name);
	} while (check_classClash(query));

	char* p;
	char tmp[20];
	change_color(1, 14);
	printf("请输入课程学分（1-4，允许一位小数）：");
	s_gets(tmp, 19);
	p = tmp;
	ret = sscanf(p, "%f", &in_f);
	while (ret != 1 || in_f > 4.0 || in_f < 1.0)
	{
		change_color(4, 14);
		printf("无效，请重新输入：");
		s_gets(tmp, 19);
		p = tmp;
		ret = sscanf(p, "%f", &in_f);
	}
	sprintf(credit, "%.1f", in_f);			// 学分浮点转字符串

	change_color(1, 14);
	printf("请输入课程学时（允许一位小数）：");
	s_gets(tmp, 19);
	p = tmp;
	ret = sscanf(p, "%f", &in_f);
	while (ret != 1)
	{
		change_color(4, 14);
		printf("输入无效，请重新输入：");
		s_gets(tmp, 19);
		p = tmp;
		ret = sscanf(p, "%f", &in_f);
	}
	sprintf(learnTime, "%.1f", in_f);		// 学时浮点转字符串

	do
	{
		reflag = 0;		//防止无限循环
		strcpy(startTime, term);			//将之前存好的学期重新写入避免循环后重复拼入
		change_color(1, 14);
		printf("请输入开课周次（输入1-20间整数）：");
		s_gets(tmp, 19);
		p = tmp;
		ret = sscanf(p, "%d", &in);
		while (ret != 1 || in > 20 || in < 1)
		{
			change_color(4, 14);
			printf("输入无效，请重新输入：");
			s_gets(tmp, 19);
			p = tmp;
			ret = sscanf(p, "%d", &in);
		}

		int sw;
		sw = in;							//存好开课周次判断结课大于开课

		sprintf(in_s, "第%d周", in);		//周次整型转字符串
		strcpy(endTime, startTime);			//开课结课时间同一学期直接复制
		strcat(startTime, in_s);

		change_color(1, 14);
		printf("请输入结课周次（输入1-20间整数）：");
		s_gets(tmp, 19);
		p = tmp;
		ret = sscanf(p, "%d", &in);
		while (ret != 1 || in > 20 || in < 1 || in < sw)
		{
			change_color(4, 14);
			printf("输入无效，请重新输入：");
			s_gets(tmp, 19);
			p = tmp;
			ret = sscanf(p, "%d", &in);
		}
		sprintf(in_s, "第%d周", in);		//周次整型转字符串
		strcat(endTime, in_s);

		change_color(1, 14);
		printf("\n该课程最终开课时间：%s", startTime);
		printf("\n该课程最终结课时间：%s\n\n", endTime);

		printf("\n时间节次表：\n1、8:00-8:50\n2、9:00-9:50\n3、10:00-10:50\n"
			"4、11:00-11:50\n5、13:30-14:20\n6、14:30-15:20\n7、15:30-16:20\n"
			"8、16:30-17:20\n9、18:30-19:20\n10、19:30-20:20");
		printf("\n输入格式：[周几(1-7)] [第几(1-10)节]\n"
			"若具体上课时间为每周三第五节（13:30-14:20），则输入应为：3 5");
		printf("\n请输入具体上课时间段：");
		s_gets(tmp, 19);
		p = tmp;
		ret = sscanf(p, "%d %d", &in,&in1);
		while (ret != 2 || in > 7 || in < 1 || in1 < 1 || in1 > 10)
		{
			change_color(4, 14);
			printf("输入无效，请重新输入：");
			s_gets(tmp, 19);
			p = tmp;
			ret = sscanf(p, "%d %d", &in, &in1);
		}
		switch (in) {		//周整型转字符串
		case 1:
			sprintf(time, "周一 ");
			break;
		case 2:
			sprintf(time, "周二 ");
			break;
		case 3:
			sprintf(time, "周三 ");
			break;
		case 4:
			sprintf(time, "周四 ");
			break;
		case 5:
			sprintf(time, "周五 ");
			break;
		case 6:
			sprintf(time, "周六 ");
			break;
		case 7:
			sprintf(time, "周日 ");
			break;
		default:
			break;
		}
		switch (in1) {		//时间段整型转字符串
		case 1:
			strcat(time, "8:00-8:50");
			break;
		case 2:
			strcat(time, "9:00-9:50");
			break;
		case 3:
			strcat(time, "10:00-10:50");
			break;
		case 4:
			strcat(time, "11:00-11:50");
			break;
		case 5:
			strcat(time, "13:30-14:20");
			break;
		case 6:
			strcat(time, "14:30-15:20");
			break;
		case 7:
			strcat(time, "15:30-16:20");
			break;
		case 8:
			strcat(time, "16:30-17:20");
			break;
		case 9:
			strcat(time, "18:30-19:20");
			break;
		case 10:
			strcat(time, "19:30-20:20");
			break;
		default:
			break;
		}
		change_color(1, 14);
		printf("\n该课程最终上课时间段为：%s", time);

		sprintf(query, "select 开课时间,结课时间,上课时间段 from classes where "
			"开课教师='%s' and 上课时间段='%s' and 开课时间 like '%s"
			, nowName, time, term);
		strcat(query, "%'");				//准备取出该老师该学期可能有时间冲突的课
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);
		while (Row = mysql_fetch_row(result))
		{
			if (check_timeClash(Row[0], Row[1], Row[2], startTime, endTime, time))
			{
				reflag = 1;
				break;
			}
		}
		if (reflag)
		{
			change_color(4, 14);
			printf("\n\n上课时间有冲突，请重新输入！\n\n");
			continue;
		}
		change_color(1, 14);
		printf("\n\n上课地点格式：楼号-房间号。1表示教一楼2表示教二楼；房间号为3位数字"
			"\n请输入上课地点：");
		s_gets(tmp, 19);
		p = tmp;
		ret = sscanf(p, "%d-%d", &in, &in1);
		while (ret != 2 || in > 2 || in < 1 || in1 < 100 || in1 > 999)
		{
			change_color(4, 14);
			printf("无效，请重新输入：");
			s_gets(tmp, 19);
			p = tmp;
			ret = sscanf(p, "%d-%d", &in, &in1);
		}
		sprintf(classroom, "%d-%d", in, in1);
		sprintf(query, "select 开课时间,结课时间,上课时间段 from classes where "
			"上课地点='%s'", classroom);					//准备验证是否有相同教室的课
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);
		if ((int)mysql_num_rows(result)) {		//若有，则判断时间是否冲突
			while (Row = mysql_fetch_row(result))
			{
				if (check_timeClash(Row[0], Row[1], Row[2], startTime, endTime, time))
				{
					reflag = 1;
					break;
				}
			}
			if (reflag)
			{
				printf("\n\n该教师与其他课程有冲突，请重新输入！\n\n");
				continue;
			}
		}

	} while (reflag == 1);

	change_color(1, 14);
	printf("\n\n请选择人数上限(80/100）：");
	s_gets(tmp, 19);
	p = tmp;
	ret = sscanf(p, "%d", &in);
	rewind(stdin);
	while (ret != 1 || !(in == 80 || in == 100))
	{
		change_color(4, 14);
		printf("无效，请重新输入：");
		s_gets(tmp, 19);
		p = tmp;
		ret = sscanf(p, "%d", &in);
	}
	if (in == 80)
		sprintf(limit, "80");
	else 
		sprintf(limit, "100");
	change_color(1, 14);
	printf("请输入课程介绍（250字内）：");
	s_gets(intro,499);

	change_color(1, 14);
	printf("请输入课程教材（50字以内）：");
	s_gets(book,100);

	sprintf(query, "INSERT INTO `classes` (`课程编号`, `开课学院`, `课程名称`, "
		"`学分`, `学时`, `课程性质`, `开课教师`, `开课时间`, `结课时间`, "
		"`上课时间段`, `上课地点`, `限制人数`, `已选人数`, `课程简介`, `"
		"教材信息`) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', "
		"'%s', '%s' ,'%s', '%s', '%s', '%s', '%s')"
		, classId, nowSchool, name, credit, learnTime, property, nowName
		, startTime, endTime, time, classroom, limit, "0", intro, book);

	if (mysql_query(&mysql, query))
	{
		change_color(4, 14);
		printf("\n数据库读写失败，请重试\n");
	}
	else
	{
		change_color(2, 14);
		printf("\n加课成功！\n");
	}
	change_color(1, 14);
	printf("\n按任意键返回上层菜单……");
	system("pause>nul");
	return;
}

//验证课id是否满足6位数字，符合则返回1
int check_classId(char* str)
{
	int b = 1, y = 0;
	while (str[y] != '\0')
		y++;
	if (y != 6)
		b = 0;
	for (int y = 0; y < 6; y++)
	{
		if (str[y] < '0' || str[y]>'9')
			b = 0;
	}
	return b;
}

void teacher_reg()
{
	system("cls");
	char school[50], name[50], phone[100], passwd[100], email[100];
	char query[300];
	system("title 学生选课管理系统 - 教师注册");
	change_color(5, 14);
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t\t○●○●○● 注册 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	change_color(1, 14);
	printf("请输入教师工号:");
	s_gets(teachID, 12);
	do {
		while (check_stuId(teachID) == 0)//检查输入是否符合规范 
		{
			change_color(4, 14);
			printf("无效输入！请输入10位数字:(若返回上一级，请按Ctrl+Q后回车)");
			change_color(1, 14);
			s_gets(teachID, 20);

			if (teachID[0] == 17)//判断若输入首字符为ctrl+q则返回上层
			{
				system("cls");
				return;
			}
		}
		sprintf(query, "select * from teachers where teachID='%s'", teachID);
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);
		if ((int)mysql_num_rows(result) != 0)
		{
			change_color(4, 14);
			printf("此教师工号已注册!请更换教师工号:(若返回上一级，请按Ctrl+Q后回车)");
			change_color(1, 14);
			s_gets(teachID, 20);
			if (teachID[0] == 17)//判断若输入首字符为ctrl+q则返回上层
			{
				system("cls");
				return;
			}
		}
	} while ((int)mysql_num_rows(result) != 0);

	char passwd1[30];
	printf("请输入密码:");
	scanf_pw(passwd);

	change_color(1, 14);
	printf("请再次确认密码:");
	scanf_pw(passwd1);
	do {
		if (strcmp(passwd, passwd1) != 0)
		{
			change_color(4, 14);
			printf("两次输入的密码不一致，请重新确认：（若返回上一级，请按Ctrl+Q后回车）");
			change_color(1, 14);
			s_gets(passwd1, 20);
			if (passwd1[0] == 17)//判断若输入首字符为ctrl+q则返回上层
			{
				system("cls");
				teacher_reg();
				return;
			}
		}
	} while (strcmp(passwd, passwd1) != 0);
	pw_encode(passwd);

	printf("请输入学院:");
	s_gets(school, 20);
	printf("请输入姓名:");
	s_gets(name, 20);
	printf("请输入电话:");
	s_gets(phone, 20);
	while (check_phone(phone) == 0)
	{
		change_color(4, 14);
		printf("无效输入！请输入11位电话号:(若返回上一级，请按Ctrl+Q后回车)");
		change_color(1, 14);
		s_gets(phone, 20);
		if (phone[0] == 17)//判断若输入首字符为ctrl+q则返回上层
		{
			system("cls");
			return;
		}
	}

	printf("请输入邮箱:");
	s_gets(email, 20);
	while (check_email(email) == 0)
	{
		change_color(4, 14);
		printf("无效输入！请按照***@***.***格式输入:");
		change_color(1, 14);
		s_gets(email, 20);
		if (email[0] == 17)//判断若输入首字符为ctrl+q则返回上层
		{
			system("cls");
			/*teacher_login();*/
			return;
		}
	}
	sprintf(query, "INSERT INTO `teachers` (`teachID`, `school`, `name`, `passwd`, `email`) "
		"VALUES ('%s', '%s', '%s', '%s', '%s')", teachID, school, name, passwd, email);
	if (mysql_query(&mysql, query))
	{
		change_color(4, 14);
		printf("\n数据库读写失败，请重试\n");
	}
	else
	{
		change_color(2, 14);
		printf("\n注册成功！\n");
	}
	change_color(1, 14);
	printf("\n请按任意键返回上一菜单\n");
	system("pause > nul");
	return;
}

//输入密码用*代替显示，输入为待赋值字符串数组
int scanf_pw(char* str)
{
	int i;
	for (i = 0;; )
	{
		str[i] = _getch();
		if (str[i] == 13 )	//如果是回车符就加结束字符
		{
			str[i] = '\0';
			break;
		}
		if (i > 19)		//如果是到20位就加结束字符
			str[i] = '\0';
		else if (str[i] == 8 || str[i] == 127)
		{

			if (i > 0)	//若减后不为0都能往前 
			{
				i--; 
				printf("\b \b");
				str[i] = '\0';
			}
		}
		//保证输入字符串长度不大于20且不为莫名其妙的字符
		else if (i < 20 && str[i] < 127 && str[i] >39)
		{
			printf("*");
			i++;
		}
	}
	printf("\n");
	return 1;
}

//字符串原地使用base64进行加密
//来源：github开源代码
//输入格式：字符串指针
void pw_encode(char* str)
{
	long lenth;
	long str_lenth;
	char* en_result;
	int i, j;
	//定义base64编码表  
	char base64_table[100] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

	//计算经过base64编码后的字符串长度  
	str_lenth = (long)strlen(str);

	if (str_lenth % 3 == 0)
		lenth = str_lenth / 3 * 4;
	else
		lenth = (str_lenth / 3 + 1) * 4;

	en_result = (char*)malloc(sizeof(char) * lenth + 1);
	en_result[lenth] = '\0';

	//以3个8位字符为一组进行编码  
	for (i = 0, j = 0; i < lenth - 2; j += 3, i += 4)
	{
		//取出第一个字符的前6位并找出对应的结果字符 
		en_result[i] 
			= base64_table[str[j] >> 2]; 

		//将第一个字符的后位与第二个字符的前4位进行组合并找到对应的结果字符
		en_result[i + 1] 
			= base64_table[(str[j] & 0x3) << 4 | (str[j + 1] >> 4)];

		//将第二个字符的后4位与第三个字符的前2位组合并找出对应的结果字符
		en_result[i + 2] = base64_table[(str[j + 1] & 0xf) << 2 | (str[j + 2] >> 6)];

		//取出第三个字符的后6位并找出结果字符
		en_result[i + 3] = base64_table[str[j + 2] & 0x3f];
	}

	switch (str_lenth % 3)
	{
	case 1:
		en_result[i - 2] = '=';
		en_result[i - 1] = '=';
		break;
	case 2:
		en_result[i - 1] = '=';
		break;
	}

	sprintf(str, "%s", en_result);
}

//字符串原地使用base64进行解密
//来源：github开源代码
//输入格式：字符串指针
void pw_decode(char* str)
{
	//根据base64表，以字符找到对应的十进制数据  

	long lenth;
	long str_lenth;
	char* de_result;
	int i, j;
	int table[] = { 0,0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,0,0,0,0,0,
					0,0,0,0,0,0,0,62,0,0,0,
					63,52,53,54,55,56,57,58,
					59,60,61,0,0,0,0,0,0,0,0,
					1,2,3,4,5,6,7,8,9,10,11,12,
					13,14,15,16,17,18,19,20,21,
					22,23,24,25,0,0,0,0,0,0,26,
					27,28,29,30,31,32,33,34,35,
					36,37,38,39,40,41,42,43,44,
					45,46,47,48,49,50,51
	};

	//计算解码后的字符串长度  
	lenth = (long)strlen(str);

	//判断编码后的字符串后是否有=  
	if (strstr(str, "=="))
		str_lenth = lenth / 4 * 3 - 2;
	else if (strstr(str, "="))
		str_lenth = lenth / 4 * 3 - 1;
	else
		str_lenth = lenth / 4 * 3;

	de_result = (char*)malloc(sizeof(unsigned char) * str_lenth + 1);
	de_result[str_lenth] = '\0';

	//以4个字符为一位进行解码  
	for (i = 0, j = 0; i < lenth - 2; j += 3, i += 4)
	{
		//取出第一个字符对应base64表的十进制数的前6位与第二个字符对应base64表的十进制数的后2位进行组合  
		de_result[j] = 
			((unsigned char)table[str[i]]) << 2 | (((unsigned char)table[str[i + 1]]) >> 4); 
		
		//取出第二个字符对应base64表的十进制数的后4位与第三个字符对应bas464表的十进制数的后4位进行组合
		de_result[j + 1] = 
			(((unsigned char)table[str[i + 1]]) << 4) | (((unsigned char)table[str[i + 2]]) >> 2);   

		//取出第三个字符对应base64表的十进制数的后2位与第4个字符进行组合
		de_result[j + 2] = 
			(((unsigned char)table[str[i + 2]]) << 6) | ((unsigned char)table[str[i + 3]]);   
	}
	sprintf(str, "%s", de_result);
}

// 菜单选项输入，避免只读取空格前内容造成错误输入可以正常运行的BUG
// 输入格式：选项option指针，最小选项，最大选项
int scanf_opt(int* optPtr, int optMin, int optMax) {
	int flag;
	char in_s[10];
	int i = 0;
	do
	{
		flag = 0;
		i = _getch();
		i -= 48;
		*optPtr = i;
		if ( *optPtr > optMax || *optPtr < optMin)
		{
			change_color(4, 14);
			printf("\n输入无效，请您重新输入：");
			change_color(1, 14);
			flag = 1;
			continue;
		}
	} while (flag);
	return 1;
}

// 用于取代gets，删除最后的回车符
// 输入格式：字符串指针，长度限制
char* s_gets(char* str, int n)
{
	char* in;
	int flag,i;
	do 
	{
		i = 0;
		flag = 0;
		change_color(1, 14);
		in = fgets(str, n + 2, stdin);
		if (in)
		{
			while (str[i] != '\n' && str[i] != '\0')
				i++;
			if (str[i] == '\n')
				str[i] = '\0';
			else
			{
				change_color(4, 14);
				printf("输入内容超过长度限制，请重新输入：");
				flag = 1;
				rewind(stdin);
			}
		}
	} while (flag);
	return in;
}

//更改下一次输出的颜色
//输入格式：文字颜色，背景颜色
//0 = 黑色       8  = 灰色
//1 = 蓝色       9  = 淡蓝色
//2 = 绿色       10 = 淡绿色
//3 = 浅绿色     11 = 淡浅绿色
//4 = 红色       12 = 淡红色
//5 = 紫色       13 = 淡紫色
//6 = 黄色       14 = 淡黄色
//7 = 白色       15 = 亮白色
void change_color(int text, int bg)
{
	bg <<= 4;
	SetConsoleTextAttribute(consoleHWnd, bg + text);
}

DWORD WINAPI SelectEventThread(LPVOID pM)
{
	CONSOLE_SELECTION_INFO selectionInf;
	char* poss = pos;
	int p[6][4];
	//printf("子线程的线程ID号为：%d\n", GetCurrentThreadId());
	sscanf(poss, "%d-%d,%d-%d|%d-%d,%d-%d|%d-%d,%d-%d|%d-%d,%d-%d|%d-%d,%d-%d|%d-%d,%d-%d"
		, &p[0][0], &p[0][1], &p[0][2], &p[0][3]
		, &p[1][0], &p[1][1], &p[1][2], &p[1][3]
		, &p[2][0], &p[2][1], &p[2][2], &p[2][3]
		, &p[3][0], &p[3][1], &p[3][2], &p[3][3]
		, &p[4][0], &p[4][1], &p[4][2], &p[4][3]
		, &p[5][0], &p[5][1], &p[5][2], &p[5][3]);
	int x, y;

	while (1) {
		GetConsoleSelectionInfo(&selectionInf);
		//printf(" \b");
		y = selectionInf.dwSelectionAnchor.Y;
		x = selectionInf.dwSelectionAnchor.X;
		for (int i = 0; i < 6; i++) {
			if (y >= p[i][0] && y <= p[i][1] && x <= p[i][3] && x >= p[i][2]) {
				/*mouse_event(MOUSEEVENTF_RIGHTDOWN, 0, 0, XBUTTON1, 0);
				mouse_event(MOUSEEVENTF_RIGHTUP, 0, 0, XBUTTON2, 0);
				printf(" \b");*/
				keybd_event(49 + i, 0, 0, 0);
				keybd_event(49 + i, 0, 2, 0);
			}
		}
		Sleep(200);
	}
}