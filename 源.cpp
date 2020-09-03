#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <winsock.h>
#include <mysql.h> 
#include <string.h>
#include <time.h>
#pragma comment (lib, "libmysql.lib")

void config_init();				// 若配置文件存在则读取，否则进行首次运行配置程序
void readCFG();

int check_stuId(char* str);
int main_entrance();
void student_login();
void sql();
int check_password(int who, char* ID, char* password);
void student_mainmenu();
void student_reg();
int check_phone(char* str);
int check_email(char* str);
void student_select_course();
void student_query_course();
void student_query_result();
void student_delete_course();
void student_manage_course();
void student_search_specific_imformation();
void select_class(char* query);
int check_timeClash(char* time1_sweek, char* time1_eweek, char* time1_day, char* time2_sweek, char* time2_eweek, char* time2_day);
void check_class_exist(char* classID);

int check_classId(char* str);
void select_class(char* query);
void teacher_login();   // 教师登录模块
void teacher_reg();		//教师注册模块
void teacher_mainmenu();  // 教师主菜单-3个大功能选单
void select_managemenu();  // 选课管理选单-5个小功能
void sm_mycourse();    //  查询查询教师自己开设过的课程的选课情况
void sm_findcourse();   //  查询选择某门课程的学生信息
void sm_lessthan30delete();  //  选课人数少于30则删除
void sm_sortcourse();   //  统计选课信息
void sm_rankcourse();   //排序选课信息
void course_managemenu();  // 课程管理选单-4个小功能
void cm_list1();     //  课程查询显示
void cm_list2();    // 课程查询操作
void cm_add();     //  加课
void cm_edit();     //  改课选单
void cm_delete();    //  未开课前删课
void personal_managemenu();  // 个人信息管理选单-2个小功能
void pm_edit();   //  改密码
int getState_selecting();		// 获取选课状态 0为未开始选课，1为正在选课时间内，2为选课时间已结束
int getState_starting(char*, char*);		// 获取开课状态 0为未开课，1为已开课
int check_password(int, char*, char*);	// 第一个参数学生为0，教师为1；登录失败返回0，成功返回1
void sql();
int check_phone(char*);
int check_classId(char*);
int check_teachId(char*);
int check_timeClash(char*, char*, char*, char*, char*, char*);

MYSQL mysql;     //创造一个MYSQL句柄
MYSQL_RES* result;
MYSQL_RES* result1;
MYSQL_RES* result2;
MYSQL_RES* result3;
MYSQL_RES* result4;
MYSQL_RES* result5;
MYSQL_RES* result6;
MYSQL_RES* result7;

MYSQL_FIELD* field;
MYSQL_ROW nextRow;
MYSQL_ROW Row;
MYSQL_ROW Row1;
MYSQL_ROW Row2;
MYSQL_ROW Row3;
MYSQL_ROW Row4;
MYSQL_ROW Row5;
char stuID[11];
time_t convert_dateToTT(int, int, int, int, int, int);

char teachID[100] = "2019222222";
char nowName[20], nowSchool[20];			//登录进来先获取自己的名字和学院方便后续使用


char dbIP[50] = "", dbUser[50] = "", dbPassWd[50] = "", dbName[50] = "";
int dbPort = 3306;

int currYear = 2020, currTerm = 1;			// 当前上课学期
time_t currStart, selecStart, selecEnd;		// 当前学期开课时间、选课始终时间




int main()
{
	mysql_init(&mysql);		// 初始化mysql

	config_init();

	sql();
	
	main_entrance();
	
}

int main_entrance()
{
	int option;
	system("cls");
	system("title 学生选课管理系统 - 入口");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 欢迎登录学生选课管理系统 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请选择您的身份:\n");
	printf("  ① - 学生\n");
	printf("  ② - 老师\n");
	printf("  ③ - 退出程序\n\n");
	printf("请输入1，2或3：");
	int ret = scanf("%d", &option);
	while (ret != 1 || option > 3 || option < 1)
	{
		while (getchar() != '\n');
		{
			printf("输入无效，请您重新输入：");
			ret = scanf("%d", &option);
		}
	}
	switch (option)
	{
	case 1:
		system("cls");
		student_login();
		break;
	case 2:
		system("cls");
		teacher_login();
		break;
	case 3:
		exit(0);
	}
	return 0;
}

void student_login()
{
	int option1;
	system("title 学生选课管理系统 - 学生登录");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 欢迎登录学生选课管理系统--学生 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请选择您要进行的操作:\n");
	printf("  ① - 登录\n");
	printf("  ② - 注册\n");
	printf("  ③ - 返回上层\n\n");
	printf("请输入1，2或3：");
	int ret1 = scanf("%d", &option1);
	while (ret1 != 1 || option1 > 3 || option1 < 1)
	{
		while (getchar() != '\n');
		{
			printf("输入无效！请重新输入：");
			ret1 = scanf("%d", &option1);
		}
	}

	if (option1 == 1)
	{
		system("cls");
		char stu_passwd[100];
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t○●○●○● 登陆界面--学生 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\n请输入学号：");
		scanf("%s", stuID);

		char query[100] = "select name,school from students where stuID='";
		strcat(query, stuID);
		strcat(query, "'");
		mysql_store_result(&mysql);
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);
		if (result)
		{
			nextRow = mysql_fetch_row(result);
			sprintf(nowName, nextRow[0]);
			sprintf(nowSchool, nextRow[1]);
		}
		
		while (mysql_num_rows(result) == 0 || check_stuId(stuID) == 0)
		{
			if (check_stuId(stuID) == 0)
				printf("无效输入！请输入10位数字\n");
			else if (mysql_num_rows(result) == 0)
				printf("此学号未注册，请重新输入！\n");
			scanf("%s", stuID);
			char query1[100] = "select name,school from students where stuID='";
			strcat(query1, stuID);
			strcat(query1, "'");
			mysql_store_result(&mysql);
			mysql_query(&mysql, query1);
			result = mysql_store_result(&mysql);
			if (result)
			{
				nextRow = mysql_fetch_row(result);
				sprintf(nowName, nextRow[0]);
				sprintf(nowSchool, nextRow[1]);
			}
		}

		printf("请输入密码：");
		scanf("%s", stu_passwd);
		while (check_password(0, stuID, stu_passwd) == 0)
		{
			printf("密码错误!请重新输入密码：");
			scanf("%s", stu_passwd);
		}
		student_mainmenu();
	}
	else if (option1 == 2)
	{
		student_reg();
		printf("\n请按任意键返回上一菜单");
		system("pause > nul");
		student_login();
	}
	else if (option1 == 3)
	{
		main_entrance();
	}
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

void sql()
{
	
	
	if (!(mysql_real_connect(&mysql, dbIP, dbUser, dbPassWd, dbName, dbPort, NULL, 0))) {
		printf("无法连接到数据库，错误代码: %s\n", mysql_error(&mysql));
		getchar();
		exit(1);
	}
	if (!mysql_set_character_set(&mysql, "gbk"))
	{
		
		mysql_character_set_name(&mysql);
	}
}

// 第一个参数学生为0，教师为1；登录失败返回0，成功返回1
int check_password(int who, char* ID, char* password) 
{
	char query[200] = "SELECT * FROM ";
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
	strcat(query, password);
	strcat(query, "'");
	mysql_query(&mysql, query);  // 同时匹配用户名和密码查询
	result = mysql_store_result(&mysql);
	if (result) {     // 防止数据为空造成崩溃
		if (mysql_num_rows(result) != 1) // 若非有且仅有一行数据则登录失败
		{
			mysql_free_result(result);
			return 0;
		}
		else {
			mysql_free_result(result);
			return 1;
		}
	}
	else {
		return 0;
	}
}

void student_mainmenu()
{
	system("cls");
	int a;
	system("title 学生选课管理系统 - 学生主菜单");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 功能界面--学生 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n登录成功，欢迎您，\n%s 的 %s 同学。\n", nowSchool, nowName);
	printf("\n请选择你需要的操作：\n");
	printf("  ① - 学生选课\n");
	printf("  ② - 查询课程 \n");
	printf("  ③ - 查询选课结果\n");
	printf("  ④ - 删除选课结果\n");
	printf("  ⑤ - 个人信息管理\n");
	printf("  ⑥ - 查看课程详细信息\n\n");
	printf("  ⑦ - 退出登录\n");
	printf("请输入1，2，3，4，5，6或7：");
	int ret2 = scanf("%d", &a);
	while (ret2 != 1 || a > 7 || a < 1)
	{
		while (getchar() != '\n');
		{
			printf("输入无效！请重新输入：");
			ret2 = scanf("%d", &a);
		}
	}
	if (a == 1)
	{
		student_select_course();
		printf("请按任意键返回上一菜单\n");
		system("pause > nul");
		student_mainmenu();
	}
	else if (a == 2)
	{
		student_query_course();
		printf("请按任意键返回上一菜单\n");
		system("pause > nul");
		student_mainmenu();
	}
	else if (a == 3)
	{
		student_query_result();
		printf("请按任意键返回上一菜单\n");
		system("pause > nul");
		student_mainmenu();
	}
	else if (a == 4)
	{
		student_delete_course();
		printf("请按任意键返回上一菜单\n");
		system("pause > nul");
		student_mainmenu();
	}
	else if (a == 5)
	{
		student_manage_course();
		printf("请按任意键返回上一菜单\n");
		system("pause > nul");
		student_mainmenu();
	}
	else if (a == 6)
	{
		student_search_specific_imformation();
		printf("请按任意键返回上一菜单\n");
		system("pause > nul");
		student_mainmenu();
	}
	else if (a == 7)
	{
		main_entrance();
	}
}

void student_reg()
{
	system("cls");
	char stuID[11], school[50], major[50], name[50], sexual[5], phone[100], passwd[100], email[100];
	system("title 学生选课管理系统 - 学生注册");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 注册 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("请输入学号:");
	scanf("%s", stuID);
	char query[100] = "select * from students where stuID='";
	strcat(query, stuID);
	strcat(query, "'");
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	while (result)
	{
		printf("此学号已注册!请更换学号:");
		scanf("%s", stuID);
		char query11[100] = "select * from students where stuID='";
		strcat(query11, stuID);
		strcat(query11, "'");
		mysql_query(&mysql, query11);
		result = mysql_store_result(&mysql);
	}


	while (check_stuId(stuID) == 0)//检查输入是否符合规范 
	{
		printf("无效输入！请输入10位数字:");
		scanf("%s", stuID);
	}
	char query1[200] = "insert into students(stuID) values(  ";
	strcat(query1, "'");
	strcat(query1, stuID);
	strcat(query1, "'");
	strcat(query1, ")");
	mysql_query(&mysql, query1);

	printf("请输入学院:");
	scanf("%s", school);
	char query2[200] = "update students set school=' ";
	strcat(query2, school);
	strcat(query2, "' where stuID='");
	strcat(query2, stuID);
	strcat(query2, "'");
	mysql_query(&mysql, query2);

	printf("请输入专业:");
	scanf("%s", major);
	char query3[200] = "update students set major=' ";
	strcat(query3, major);
	strcat(query3, "' where stuID='");
	strcat(query3, stuID);
	strcat(query3, "'");
	mysql_query(&mysql, query3);

	printf("请输入姓名:");
	scanf("%s", name);
	char query4[200] = "update students set name=' ";
	strcat(query4, name);
	strcat(query4, "' where stuID='");
	strcat(query4, stuID);
	strcat(query4, "'");
	mysql_query(&mysql, query4);

	printf("请输入性别:");
	scanf("%s", sexual);
	while (strcmp(sexual, "男") != 0 && strcmp(sexual, "女") != 0)
	{
		printf("请输入男或女:");
		scanf("%s", sexual);
	}
	char query5[200] = "update students set sexual=' ";
	strcat(query5, sexual);
	strcat(query5, "' where stuID='");
	strcat(query5, stuID);
	strcat(query5, "'");
	mysql_query(&mysql, query5);

	printf("请输入电话:");
	scanf("%s", phone);
	while (check_phone(phone) == 0)
	{
		printf("无效输入！请输入11位电话号:");
		scanf("%s", phone);
	}
	char query6[200] = "update students set phone=' ";
	strcat(query6, phone);
	strcat(query6, "' where stuID='");
	strcat(query6, stuID);
	strcat(query6, "'");
	mysql_query(&mysql, query6);

	printf("请输入密码:");
	scanf("%s", passwd);
	char query7[200] = "update students set passwd=' ";
	strcat(query7, passwd);
	strcat(query7, "' where stuID='");
	strcat(query7, stuID);
	strcat(query7, "'");
	mysql_query(&mysql, query7);

	printf("请输入邮箱:");
	scanf("%s", email);
	while (check_email(email) == 0)
	{
		printf("无效输入！请按照***@***.***格式输入:");
		scanf("%s", email);
	}
	char query8[200] = "update students set email=' ";
	strcat(query8, email);
	strcat(query8, "' where stuID='");
	strcat(query8, stuID);
	strcat(query8, "'");
	mysql_query(&mysql, query8);

	printf("\n注册成功！\n");

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
	system("cls");
	system("title 学生选课管理系统 - 学生选课");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 学生选课 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	char classID[50];
	printf("\n课程信息如下：\n");
	char query[50] = "select * from classes";
	select_class(query);
	printf("\n请输入您想选的课程编号：");
	scanf("%s", classID);
	//判断是否有这门课-------------------------------------------------------------------------------
	check_class_exist(classID);
	
	//把学生之前选的课从表中取出，存到字符串中
	char query1[100] = "select class1,class2,class3 from students where stuID='";
	strcat(query1, stuID);
	strcat(query1, "'");
	mysql_store_result(&mysql);
	mysql_query(&mysql, query1);
	result = mysql_store_result(&mysql);
	if (result)
	{
		nextRow = mysql_fetch_row(result);
	}

	if ((nextRow[0] != NULL) && (nextRow[1] != NULL) && (nextRow[2] != NULL))//class123都不为空
	{
		printf("\n您已选满三节课，无法再次选课！\n");
	}
	else {
		//判断从表中取出的那些课的是不是和学生现在想选的课有重复
		if (((nextRow[0] != NULL) && (strcmp(classID, nextRow[0]) == 0)) || ((nextRow[1] != NULL) && (strcmp(classID, nextRow[1]) == 0)) || ((nextRow[2] != NULL) && (strcmp(classID, nextRow[2]) == 0)))//strcmp,如果有重复,class1ID==classID||....||....
		{
			printf("\n您已选过该课程！\n\n");
		}
		else {
			//把学生已选的课的开课时间，结课时间，时间段取出并保存在字符串中
			//select 开课时间,结课时间,上课时间段 from classes where 课程编号=
			//(select class1 from students where stuID='2019222222')
			char query6[200] = "select 开课时间,结课时间,上课时间段 from classes where 课程编号=(select class1 from students where stuID='";
			strcat(query6, stuID);
			strcat(query6, "')");
			mysql_store_result(&mysql);
			mysql_query(&mysql, query6);
			result2 = mysql_store_result(&mysql);
			if (result2)
			{
				Row1 = mysql_fetch_row(result2);
			}

			char query7[200] = "select 开课时间,结课时间,上课时间段 from classes where 课程编号=(select class2 from students where stuID='";
			strcat(query7, stuID);
			strcat(query7, "')");
			mysql_store_result(&mysql);
			mysql_query(&mysql, query7);
			result3 = mysql_store_result(&mysql);
			if (result3)
			{
				Row2 = mysql_fetch_row(result3);
			}

			char query8[200] = "select 开课时间,结课时间,上课时间段 from classes where 课程编号=(select class3 from students where stuID='";
			strcat(query8, stuID);
			strcat(query8, "')");
			mysql_store_result(&mysql);
			mysql_query(&mysql, query8);
			result4 = mysql_store_result(&mysql);
			if (result4)
			{
				Row3 = mysql_fetch_row(result4);
			}

			char query9[200] = "select 开课时间,结课时间,上课时间段 from classes where 课程编号=";
			strcat(query9, classID);
			mysql_store_result(&mysql);
			mysql_query(&mysql, query9);
			result5 = mysql_store_result(&mysql);
			if (result5)
			{
				Row4 = mysql_fetch_row(result5);
			}

			int check1, check2, check3;
			check1 = 0;
			check2 = 0;
			check3 = 0;

			if (mysql_num_rows(result2) != 0 && mysql_num_rows(result5) != 0)
				check1 = check_timeClash(Row1[0], Row1[1], Row1[2], Row4[0], Row4[1], Row4[2]);//比较class1和class2是否时间冲突
			if (mysql_num_rows(result3) != 0 && mysql_num_rows(result5) != 0)
				check2 = check_timeClash(Row2[0], Row2[1], Row2[2], Row4[0], Row4[1], Row4[2]);
			if (mysql_num_rows(result4) != 0 && mysql_num_rows(result5) != 0)
				check3 = check_timeClash(Row3[0], Row3[1], Row3[2], Row4[0], Row4[1], Row4[2]);

			char query2[100] = "select 限制人数,已选人数 from classes where 课程编号='";
			strcat(query2, classID);
			strcat(query2, "'");
			mysql_store_result(&mysql);
			mysql_query(&mysql, query2);
			result1 = mysql_store_result(&mysql);
			if (result1)
			{
				Row = mysql_fetch_row(result1);
			}
			if (strcmp(Row[0], Row[1]) == 0)//超过限制人数
			{
				printf("\n此课程已满，无法选择！\n");
			}
			else
			{
				if (check1 == 1 || check2 == 1 || check3 == 1)//判断时间是否和已选的课有冲突
				{
					printf("\n此课程和您已选课程时间冲突，无法选择！\n");
				}
				else {
					if (nextRow[0] == NULL)//class1 == null
					{
						//在class1中插入选的课程编号
						//update students set class1 = '2' where stuID = '2019222222'
						char query3[100] = "update students set class1 ='";
						strcat(query3, classID);
						strcat(query3, "'where stuID='");
						strcat(query3, stuID);
						strcat(query3, "'");
						mysql_query(&mysql, query3);
						printf("\n选课成功！");
					}
					else if ((nextRow[0] != NULL) && (nextRow[1] == NULL))//class1 != null并且class2=null
					{
						//在class2中插入选的编号
						char query4[100] = "update students set class2 ='";
						strcat(query4, classID);
						strcat(query4, "'where stuID='");
						strcat(query4, stuID);
						strcat(query4, "'");
						mysql_query(&mysql, query4);
						printf("\n选课成功！");
					}
					else if ((nextRow[0] != NULL) && (nextRow[1] != NULL) && (nextRow[2] == NULL))//class1 != null并且class2=！null并且class3==null
					{
						//在class3中插入选的编号
						char query5[100] = "update students set class3 ='";
						strcat(query5, classID);
						strcat(query5, "'where stuID='");
						strcat(query5, stuID);
						strcat(query5, "'");
						mysql_query(&mysql, query5);
						printf("\n选课成功！");
					}
				}
			}
		}
	}
}

void student_query_course()
{
	system("cls");
	int a;
	system("title 学生选课管理系统 - 学生查课");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 查询课程 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请选择查询方式：\n");
	printf("  ① - 根据课程名查询\n");
	printf("  ② - 根据开课学院查询\n");
	printf("  ③ - 根据课余量排序所有课程\n");
	printf("  ④ - 根据选课人数排序所有课程\n");
	printf("  ⑤ - 退出程序\n");
	printf("请输入1，2，3，4或5:");
	int ret3 = scanf("%d", &a);
	while (ret3 != 1 || a > 5 || a < 1)
	{
		while (getchar() != '\n');
		{
			printf("输入无效！请重新输入：");
			ret3 = scanf("%d", &a);
		}
	}
	if (a == 1)
	{
		system("cls");
		char class_name[100];
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t○●○●○● 查询课程--课程名查询 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\n请输入课程名称：");
		scanf("%s", class_name);
		printf("\n选课结果为：\n");
		char query1[100] = "select * from classes where 课程名称='";
		strcat(query1, class_name);
		strcat(query1, "'");
		select_class(query1);
		printf("\n请按任意键返回上一菜单");
		system("pause > nul");
		student_query_course();
	}
	if (a == 2)
	{
		system("cls");
		char school_name[100];
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t○●○●○● 查询课程--开课学院查询 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\n请输入开课学院名称：");
		scanf("%s", school_name);
		printf("\n选课结果为：\n");
		char query2[100] = "select * from classes where 开课学院='";
		strcat(query2, school_name);
		strcat(query2, "'");
		select_class(query2);
		printf("\n请按任意键返回上一菜单");
		system("pause > nul");
		student_query_course();
	}
	if (a == 3)
	{
		system("cls");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t○●○●○● 查询课程--课余量排序所有课程 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\n按课余量排序的所有课程如下：\n");
		char query3[100] = "select *,(限制人数-已选人数) 余课量 from classes order by 余课量 desc";
		select_class(query3);
		printf("\n请按任意键返回上一菜单");
		system("pause > nul");
		student_query_course();
	}
	if (a == 4)
	{
		system("cls");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t○●○●○● 查询课程--选课人数排序所有课程 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\n根据选课人数排序的所有课程如下：\n");
		char query4[100] = "select * from classes order by 已选人数 desc";
		select_class(query4);
		printf("\n请按任意键返回上一菜单");
		system("pause > nul");
		student_query_course();
	}
	if (a == 5)
	{
		exit(0);
	}
}

void student_query_result()
{
	//select c.* from classes c where c.`课程编号`=(select s.class1 from students s where stuID=2019222222)
	//or c.`课程编号`=(select s.class2 from students s where stuID=2019222222) 
	//or c.`课程编号`=(select s.class3 from students s where stuID=2019222222)
	system("cls");
	char query[500] = "select c.* from classes c where c.`课程编号`=(select s.class1 from students s where stuID=";
	strcat(query, stuID);
	strcat(query, ")or c.`课程编号`=(select s.class2 from students s where stuID=");
	strcat(query, stuID);
	strcat(query, ")or c.`课程编号`=(select s.class3 from students s where stuID=");
	strcat(query, stuID);
	strcat(query, ")");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 查询选课结果 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n您的选课结果如下：\n");
	select_class(query);
}

void student_delete_course()
{
	system("cls");
	char classID[100];
	system("title 学生选课管理系统 - 学生删课");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 删除选课结果 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n您的已选课程如下:\n");
	student_query_result();
	printf("\n请输入课程编号以删除课程:");
	scanf("%s", classID);
	check_class_exist(classID);
	//取出学生想删除课的开课时间和上课时间段======================================================================================================
	char query12[100] = "select 开课时间,上课时间段 from classes where 课程编号='";
	strcat(query12, classID);
	strcat(query12, "'");
	mysql_store_result(&mysql);
	mysql_query(&mysql, query12);
	result7 = mysql_store_result(&mysql);
	if (result7)
	{
		Row5 = mysql_fetch_row(result7);
	}
	//===========================================================================================
	while (getState_starting(Row5[0], Row5[1]) == 1)
	{
		printf("此课程已开课，无法删除！\n");
		printf("请重新输入课程编号：\n");
		scanf("%s", classID);
		check_class_exist(classID);
		char query12[100] = "select 开课时间,上课时间段 from classes where 课程编号='";
		strcat(query12, classID);
		strcat(query12, "'");
		mysql_store_result(&mysql);
		mysql_query(&mysql, query12);
		result7 = mysql_store_result(&mysql);
		if (result7)
		{
			Row5 = mysql_fetch_row(result7);
		}
	}
	//=============================================================================================
	char query[100] = "select class1,class2,class3 from students where stuID='";
	strcat(query, stuID);
	strcat(query, "'");
	mysql_store_result(&mysql);
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	if (result)
	{
		nextRow = mysql_fetch_row(result);
	}

	if (nextRow[0] != NULL && strcmp(nextRow[0], classID) == 0)
	{
		char query1[100] = "update students set class1=null where stuID='";
		strcat(query1, stuID);
		strcat(query1, "'");
		mysql_query(&mysql, query1);
		printf("\n删除成功!");
		char query11[100] = "update classes set 已选人数=已选人数-1 where 课程编号='";
		strcat(query11, classID);
		strcat(query11, "'");
		mysql_query(&mysql, query11);
	}
	if (nextRow[1] != NULL && strcmp(nextRow[1], classID) == 0)
	{
		char query2[100] = "update students set class2=null where stuID='";
		strcat(query2, stuID);
		strcat(query2, "'");
		mysql_query(&mysql, query2);
		printf("\n删除成功!");
		char query22[100] = "update classes set 已选人数=已选人数-1 where 课程编号='";
		strcat(query22, classID);
		strcat(query22, "'");
		mysql_query(&mysql, query22);
	}
	if (nextRow[2] != NULL && strcmp(nextRow[2], classID) == 0)
	{
		char query3[100] = "update students set class3=null where stuID='";
		strcat(query3, stuID);
		strcat(query3, "'");
		mysql_query(&mysql, query3);
		printf("\n删除成功!");
		char query33[100] = "update classes set 已选人数=已选人数-1 where 课程编号='";
		strcat(query33, classID);
		strcat(query33, "'");
		mysql_query(&mysql, query33);
	}
	if (nextRow[0] != NULL && strcmp(nextRow[0], classID) != 0 && nextRow[1] != NULL && strcmp(nextRow[1], classID) != 0 && nextRow[2] != NULL && strcmp(nextRow[2], classID) != 0)
	{
		printf("\n您未选修该课程！\n");
	}
}

void student_manage_course()
{
	system("cls");
	int num;
	char phone[20];
	char passwd[20];
	char passwd1[20];
	char email[50];
	system("title 学生选课管理系统 - 学生个人信息管理");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 个人信息管理--学生 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请选择需要修改的信息：\n");
	printf("  ① - 电话\n");
	printf("  ② - 密码\n");
	printf("  ③ - 邮箱\n");
	printf("  ④ - 退出程序\n");
	printf("\n请输入1，2，3或4：");
	int ret4 = scanf("%d", &num);
	while (ret4 != 1 || num > 4 || num < 1)
	{
		while (getchar() != '\n');
		{
			printf("输入无效!请重新输入：");
			ret4 = scanf("%d", &num);
		}
	}

	switch (num)
	{
		case 1:
		{
			system("cls");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\t\t\t○●○●○● 个人信息管理--学生--电话 ●○●○●○\n");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\n请输入新的电话：");
			scanf("%s", &phone);
			while (check_phone(phone) == 0)
			{
				printf("输入无效！请输入11位电话号：");
				scanf("%s", &phone);
			}
			char query[200] = "update students set phone=' ";
			strcat(query, phone);
			strcat(query, "' where stuID='");
			strcat(query, stuID);
			strcat(query, "'");
			mysql_query(&mysql, query);
			printf("\n修改成功!\n");
			printf("请按任意键返回上一菜单\n");
			system("pause > nul");
			student_manage_course();
			break;
		}
		case 2:
		{
			system("cls");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\t\t\t○●○●○● 个人信息管理--学生--密码 ●○●○●○\n");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\n请输入新的密码：");
			scanf("%s", passwd);
			printf("请再次确认新的密码:");
			scanf("%s", passwd1);
			while (strcmp(passwd, passwd1) != 0)
			{
				printf("两次输入的密码不一致，请重新确认：");
				scanf("%s", passwd1);
			}
			char query1[200] = "update students set passwd=' ";
			strcat(query1, passwd);
			strcat(query1, "' where stuID='");
			strcat(query1, stuID);
			strcat(query1, "'");
			mysql_query(&mysql, query1);
			printf("\n修改成功!\n\n");
			printf("请按任意键返回上一菜单\n");
			system("pause > nul");
			student_manage_course();
			break;
		}
		case 3:
		{
			system("cls");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\t\t\t○●○●○● 个人信息管理--学生--邮箱 ●○●○●○\n");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\n请输入新的邮箱：");
			scanf("%s", email);
			while (check_email(email) == 0)
			{
				printf("无效输入！请按照***@***.***格式输入：");
				scanf("%s", email);
			}
			char query2[200] = "update students set email=' ";
			strcat(query2, email);
			strcat(query2, "' where stuID='");
			strcat(query2, stuID);
			strcat(query2, "'");
			mysql_query(&mysql, query2);
			printf("\n修改成功!\n");
			printf("\n请按任意键返回上一菜单\n");
			system("pause > nul");
			student_manage_course();
			break;
		}
		case 4:
		{
			exit(0);
		}
	}
}

void student_search_specific_imformation()
{
	char classID[100];
	system("cls");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 查看课程详细信息--学生 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n全部课程如下：\n");
	char query[50] = "select * from classes";
	select_class(query);
	printf("输入课程编号以查看该课程的详细信息\n");
	scanf("%s", classID);
	check_class_exist(classID);
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	if (result)
	{
		int fieldCount = mysql_field_count(&mysql);
		if (fieldCount > 0)
		{
			int row = mysql_num_rows(result);
			int column = mysql_num_fields(result);
			for (int i = 0; field = mysql_fetch_field(result); i++) {
				//获得属性名 
				if (i > 6)
				{
					printf("%-30s", field->name);
					printf(" |");
				}
			}
			printf("\n");
			while (nextRow = mysql_fetch_row(result)) {
				for (int j = 7; j < column ; j++) {
					printf("%-30s", nextRow[j]);
					printf(" |");
				}
				printf("\n");
			}
		}
	}
}

void select_class(char* query)
{
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	if (result)
	{
		int fieldCount = mysql_field_count(&mysql);
		if (fieldCount > 0)
		{
			int row = mysql_num_rows(result);
			int column = mysql_num_fields(result);
			for (int i = 0; field = mysql_fetch_field(result),i<7; i++) {
				//获得属性名 
				printf("%10s", field->name);
				printf(" |");
			}
			printf("\n");
			while (nextRow = mysql_fetch_row(result)) {
				for (int j = 0; j < column-8; j++) {
					printf("%10s", nextRow[j]);
					printf(" |");
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
	{
		return 0;
	}

	if (str[0] == '@' || str[sum] == '.')
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
	if (k > 1 || m > 1 || k == 0 || m == 0 || a[0] > b[0] || b[0] == (a[0] + 1))
		return 0;
	else
		return 1;
}

//快捷判断两个课时间重叠与否，输入格式：
//一课的开课时间、结课时间、具体上课时间段、二课的开课时间、结课时间、具体上课时间段；有冲突返回1
int check_timeClash(char* time1_sweek, char* time1_eweek, char* time1_day, char* time2_sweek, char* time2_eweek, char* time2_day)
{
	char term1[10], term2[10], sweek1[5], sweek2[5], eweek1[5], eweek2[5], day1[10], day2[10];
	int s1i, e1i, s2i, e2i;  // 一课的开课周数，结课周数，二课的开课，结课周数

	if (strcmp(time1_day, time2_day)) {  //上课时间段不一样直接pass
		return 0;
	}

	if (time1_sweek[3] != time2_sweek[3]) { //开课年份不一样直接pass
		return 0;
	}
	else {
		sprintf(term1, "%c%c", time1_sweek[15], time1_sweek[16]);
		sprintf(term2, "%c%c", time2_sweek[15], time2_sweek[16]);
		if (strcmp(term1, term2)) {   //开课年份一样学期不一样也pass
			return 0;
		}
	}

	if (time1_sweek[24] > 127) {     //取出开课周数
		sprintf(sweek1, "%c", time1_sweek[23]);
	}
	else {
		sprintf(sweek1, "%c%c", time1_sweek[23], time1_sweek[24]);
	}
	if (time2_sweek[24] > 127) {
		sprintf(sweek2, "%c", time2_sweek[23]);
	}
	else {
		sprintf(sweek2, "%c%c", time2_sweek[23], time2_sweek[24]);
	}
	s1i = atoi(sweek1);
	s2i = atoi(sweek2);

	if (time1_eweek[24] > 127) {     //取出开课周数
		sprintf(eweek1, "%c", time1_eweek[23]);
	}
	else {
		sprintf(eweek1, "%c%c", time1_eweek[23], time1_eweek[24]);
	}
	if (time2_eweek[24] > 127) {
		sprintf(eweek2, "%c", time2_eweek[23]);
	}
	else {
		sprintf(eweek2, "%c%c", time2_eweek[23], time2_eweek[24]);
	}
	e1i = atoi(eweek1);
	e2i = atoi(eweek2);

	if (s1i < s2i) {     //若课1开得比课2早，必须先比课1结束
		if (e1i < s2i) {
			return 0;
		}
		else {
			return 1;
		}
	}
	else {
		if (e2i < s1i) {    //若课1开得比课2早，课1必须先比课2结束
			return 0;
		}
		else {
			return 1;
		}
	}
}

void check_class_exist(char * classID)
{
	MYSQL_RES* result8;
	char query10[100] = "select * from classes where 课程编号='";
	strcat(query10, classID);
	strcat(query10, "'");
	mysql_store_result(&mysql);
	mysql_query(&mysql, query10);
	result8 = mysql_store_result(&mysql);
	while (mysql_num_rows(result8) == 0)
	{
		printf("无此课程，请重新输入！\n");
		scanf("%s", classID);
		char query10[100] = "select * from classes where 课程编号='";
		strcat(query10, classID);
		strcat(query10, "'");
		mysql_store_result(&mysql);
		mysql_query(&mysql, query10);
		result8 = mysql_store_result(&mysql);
	}
}
void teacher_mainmenu()
{
	system("cls");
	int option2 = 0;
	system("title 学生选课管理系统 - 教师主菜单");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 教师主菜单 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n登录成功，欢迎您，\n%s 的 %s 老师。\n",nowSchool,nowName);
	printf("\n请选择您需要的服务:\n");
	printf("  ① - 选课管理\n");
	printf("  ② - 课程管理\n");
	printf("  ③ - 个人信息管理\n\n");
	printf("  ④ - 退出登录\n\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请输入1，2，3或4:");
	while (scanf("%d", &option2) != 1 || option2 > 4 || option2 < 1)
	{
		printf("无效，请重新输入!\n");
		fflush(stdin);
	}
	switch (option2)
	{
	case 1:
		select_managemenu();
		break;
	case 2:
		course_managemenu();
		break;
	case 3:
		pm_edit();
		break;
	case 4:
		main_entrance();
		break;
	}

}

void select_managemenu() {
	system("cls");
	int option2 = 0;
	system("title 学生选课管理系统 - 教师选课管理");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 选课菜单 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请选择您需要的服务:\n");
	printf("  ① - 查看选课情况\n");
	printf("  ② - 查询选择某门课程的学生信息\n");
	printf("  ③ - 删除选课(选课结束后)\n");
	printf("  ④ - 统计教师自己开设过的课程数目\n");
	printf("  ⑤ - 按选课人数排序所有开设过的课程\n\n");
	printf("  ⑥ - 返回上一个菜单\n\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请输入1,2,3,4,5或6:");
	while (scanf("%d", &option2) != 1 || option2 > 6 || option2 < 1)
	{
		printf("无效，请重新输入!\n");
		fflush(stdin);
	}
	switch (option2)
	{
	case 1:
		sm_mycourse();
		break;
	case 2:
		sm_findcourse();
		break;
	case 3:
		sm_lessthan30delete();
		break;
	case 4:
		sm_sortcourse();
		break;
	case 5:
		sm_rankcourse();
		break;
	case 6:
		teacher_mainmenu();
		break;
	}
}

void sm_mycourse()
{
	system("cls");
	char query[200] = "SELECT * FROM `classes`WHERE 开课教师 = '";
	strcat(query, nowName);
	strcat(query, "'");
	select_class(query);//打印相应查询内容
	printf("按任意键返回上一菜单...\n");
	system("pause>nul");
	select_managemenu();
}

void sm_findcourse()
{
	system("cls");
	int column;
	int row;
	int flag = 0;
	int option2 = 0;
	char courseName[200] = "";
	char studentName[200] = "";
	char query[200];

	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请选择您的查询方式:\n\n");
	printf("  ① - 根据课程名称查询\n");
	printf("  ② - 根据学生姓名查询\n");
	printf("  ③ - 返回上一菜单\n\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请输入1，2或3:");
	while (scanf("%d", &option2) != 1 || option2 > 3 || option2 < 1)
	{
		printf("无效，请重新输入!\n");
		fflush(stdin);
	}
	switch (option2)
	{
	case 1:
		do {
			printf("请输入您想要查询的课程名称：");
			scanf("%s", courseName);
			sprintf(query, "SELECT * FROM `classes`WHERE 课程名称 = '%s'", courseName);
			mysql_query(&mysql, query);
			result = mysql_store_result(&mysql);
			column = mysql_num_fields(result);
			if (mysql_num_rows(result) == 0)
			{
				flag = 1;
				printf("无结果，请重新输入\n");
				mysql_free_result(result);
			}
			else {
				flag = 0;
			}
		} while (flag == 1);

		for (int i = 0; field = mysql_fetch_field(result); i++)
		{
			printf("%10s", field->name);
			printf(" |");
		}
		printf("\n");
		while (nextRow = mysql_fetch_row(result))
		{
			for (int j = 0; j < column; j++)
			{
				printf("%10s", nextRow[j]);
				printf(" |");
			}
			printf("\n");
		}
		do {
			printf("请输入您想要查询相应学生信息的课程编号：");
			scanf("%s", courseName);
			sprintf(query, "SELECT * FROM `students`WHERE class1 = '%s' OR class2 = '%s' OR class3 = '%s'"
				, courseName, courseName, courseName);
			mysql_query(&mysql, query);
			result = mysql_store_result(&mysql);
			column = mysql_num_fields(result);
			if (mysql_num_rows(result) == 0)
			{
				flag = 1;
				printf("无结果，请重新输入\n");
				mysql_free_result(result);
			}
			else {
				flag = 0;
			}
		} while (flag == 1);
		for (int i = 0; field = mysql_fetch_field(result); i++)
		{
			printf("%10s", field->name);
			printf(" |");
		}
		printf("\n");
		while (nextRow = mysql_fetch_row(result))
		{
			for (int j = 0; j < column; j++)
			{
				printf("%10s", nextRow[j]);
				printf(" |");
			}
			printf("\n");
		}

		printf("按任意键返回上一菜单...\n");
		system("pause>nul");
		sm_findcourse();
		break;
	case 2:
		do {
			printf("请输入您想要查询的学生名称：");
			scanf("%s", studentName);
			char query[200] = "SELECT * FROM `students`WHERE name = '";
			strcat(query, studentName);
			strcat(query, "'");
			mysql_query(&mysql, query);
			result = mysql_store_result(&mysql);

			column = mysql_num_fields(result);
			if (mysql_num_rows(result) == 0)
			{
				flag = 1;
				printf("无结果，请重新输入\n");
				mysql_free_result(result);
			}
			else {
				flag = 0;
			}
		} while (flag == 1);
		for (int i = 0; field = mysql_fetch_field(result); i++)
		{
			printf("%10s", field->name);
			printf(" |");
		}
		printf("\n");
		while (nextRow = mysql_fetch_row(result))
		{
			for (int j = 0; j < column; j++)
			{
				printf("%10s", nextRow[j]);
				printf(" |");
			}
			printf("\n");
		}
		printf("按任意键返回上一菜单...\n");
		system("pause>nul");
		sm_findcourse();
		break;
	case 3:
		select_managemenu();
		break;
	default:
		printf("无效，请重新输入!\n");
	}
}

void sm_lessthan30delete() {
	system("cls");
	int column;
	int row;
	int stu_num;
	int flag = 0;
	int option2 = 0;
	char courseName[200] = "niconiconi";
	char studentName[200] = "niconiconi";
	char query[200] = "SELECT * FROM `classes`WHERE 开课教师 = '";
	char query1[200];
	char query2[200];
	strcat(query, nowName);
	strcat(query, "'");
	select_class(query);//打印相应内容
	printf("以上是您开设的课程\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("请选择您需要的服务:\n\n");
	printf("  ① - 进行课程的删除\n");
	printf("  ② - 返回上一菜单\n\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请输入1或2:");
	while (scanf("%d", &option2) != 1 || option2 > 2 || option2 < 1)
	{
		printf("无效，请重新输入!\n");
		fflush(stdin);
	}
	switch (option2)
	{
	case 1:
		do {
			printf("请输入您想要删除的课程的编号：");
			scanf("%s", courseName);
			sprintf(query, "SELECT * FROM `classes`WHERE 课程编号 = '%s'"
				, courseName);
			mysql_query(&mysql, query);
			result = mysql_store_result(&mysql);
			column = mysql_num_fields(result);
			if (mysql_num_rows(result) == 0)
			{
				flag = 1;
				printf("无结果，请重新输入\n");
				mysql_free_result(result);
			}
			else {
				flag = 0;
			}
		} while (flag == 1);
		for (int i = 0; field = mysql_fetch_field(result); i++)
		{
			printf("%10s", field->name);
			printf(" |");
		}
		printf("\n");
		while (nextRow = mysql_fetch_row(result))
		{
			for (int j = 0; j < column; j++)
			{
				printf("%10s", nextRow[j]);
				printf(" |");
			}
			printf("\n");
		}

		sprintf(query, "SELECT `已选人数` FROM `classes`WHERE 课程编号 = '%s'"
			, courseName);
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);
		nextRow = mysql_fetch_row(result);
		stu_num = atoi(nextRow[0]);


		if (getState_selecting() == 2)
		{
			if (stu_num < 30)
			{
				printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
				printf("确认要删除该课程以及相应学生选课信息吗？\n");
				printf("  ① - 是\n");
				printf("  ② - 否\n\n");
				printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
				printf("\n请输入1或2:");
				while (scanf("%d", &option2) != 1 || option2 > 2 || option2 < 1)
				{
					printf("无效，请重新输入!\n");
					fflush(stdin);
				}
				/*printf("===============%s===============", courseName);*/
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
					nextRow = mysql_fetch_row(result);
					if (nextRow != nullptr)
					{
						sprintf(query2, "update students set class1 = null where stuID='%s'"
							, nextRow[0]);
						mysql_query(&mysql, query2);//class1
					}

					sprintf(query1, "SELECT stuID FROM `students` WHERE class2 = '%s'"
						, courseName);
					mysql_query(&mysql, query1);
					result = mysql_store_result(&mysql);
					nextRow = mysql_fetch_row(result);
					if (nextRow != nullptr)
					{
						sprintf(query2, "update students set class2 = null where stuID='%s'"
							, nextRow[0]);
						mysql_query(&mysql, query2);//class2
					}

					sprintf(query1, "SELECT stuID FROM `students` WHERE class3 = '%s'"
						, courseName);
					mysql_query(&mysql, query1);
					result = mysql_store_result(&mysql);
					nextRow = mysql_fetch_row(result);
					if (nextRow != nullptr)
					{
						sprintf(query2, "update students set class3 = null where stuID='%s'"
							, nextRow[0]);
						mysql_query(&mysql, query2);//class3
					}


					printf("删除成功！\n");
					printf("按任意键返回上一菜单...\n");
					system("pause>nul");
					cm_delete();
					break;
				case 2:
					cm_delete();
					break;
				default:
					printf("无效，请重新输入!\n");
				}
			}
			else
			{
				printf("非常抱歉，该课程选课人数已超过三十人，您不能对进行删除操作\n");
				printf("按任意键返回上一菜单...\n");
				system("pause>nul");
				cm_delete();
				break;
			}
		}
		else
		{
			printf("非常抱歉，该课程选课未结束，您不能对进行删除操作\n");
			printf("按任意键返回上一菜单...\n");
			system("pause>nul");
			cm_delete();
		}

		break;
	case 2:
		select_managemenu();
		break;
	default:
		printf("无效，请重新输入!\n");


	}
}

void sm_sortcourse() {
	char query[100];
	sprintf(query, "select * from `classes` where `开课教师` = '%s' ", nowName);
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	printf("\n您总共开设了%d门课程\n", mysql_num_rows(result));
	printf("\n按任意键返回上一菜单...\n");
	system("pause>nul");
	select_managemenu();
}



void sm_rankcourse() {
	char query4[100];
	sprintf(query4, "select * from `classes` where `开课教师` = '%s' order by `已选人数` "
		, nowName);
	select_class(query4);
	printf("按任意键返回上一菜单...\n");
	system("pause>nul");
	select_managemenu();

}

void course_managemenu()
{
	system("cls");
	int option2 = 0;
	system("title 学生选课管理系统 - 教师课程管理");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 课程管理菜单 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请选择您需要的服务:\n");
	printf("  ① - 课程查询\n");
	printf("  ② - 添加课程\n");
	printf("  ③ - 修改课程\n");
	printf("  ④ - 删除课程(选课开始前)\n\n");
	printf("  ⑤ - 返回上一个菜单\n\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请输入1,2,3,4或5:");
	while (scanf("%d", &option2) != 1 || option2 > 5 || option2 < 1)
	{
		printf("无效，请重新输入!\n");
		fflush(stdin);
	}
	char query1[200] = "SELECT * FROM `classes` LIMIT 0, 1000";
	switch (option2)
	{
	case 1:
		cm_list1();
		break;
	case 2:
		/*cm_add;*/
		break;
	case 3:
		cm_edit();
		break;
	case 4:
		cm_delete();
		break;
	case 5:
		teacher_mainmenu();
		break;
	}
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
	{
		return 0;					// 当前时间比开始选课时间早时
	}
	else if (nowTime - selecEnd > 0)
	{
		return 2;					// 当前时间比结束选课时间晚时
	}
	else {
		return 1;					// 当前时间比结束选课时间早，比开始时间晚
	}
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
	{
		return 1;
	}
	else if (year > currYear)
	{
		return 0;
	}
	sprintf(tmp, "%c%c", sweek[15], sweek[16]);
	if (!strcmp(tmp, "一"))
	{
		term = 1;
	}
	else
	{
		term = 2;
	}
	if (term < currTerm)		// 开课学期小于当前学期则为已开
	{
		return 1;
	}
	// 能到这的都是本学期的课，取出开课周数，若仍不为汉字取多一位
	if (sweek[24] > 127)
	{
		sprintf(tmp, "%c", sweek[23]);
	}
	else
	{
		sprintf(tmp, "%c%c", sweek[23], sweek[24]);
	}
	week = atoi(tmp);

	sprintf(tmp, "%c%c", stime[2], stime[3]);
	if (!strcmp(tmp, "一"))		// swtich不能以char数组做case，很无奈
	{
		day = 1;
	}
	else if (!strcmp(tmp, "二"))
	{
		day = 2;
	}
	else if (!strcmp(tmp, "三"))
	{
		day = 3;
	}
	else if (!strcmp(tmp, "四"))
	{
		day = 4;
	}
	else if (!strcmp(tmp, "五"))
	{
		day = 5;
	}
	else if (!strcmp(tmp, "六"))
	{
		day = 6;
	}
	else if (!strcmp(tmp, "日"))
	{
		day = 7;
	}
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
	ttsTime = currStart + 604800l * (week - 1) +
		86400l * (day - 1) + 3600l * hr + 60 * min;
	nowTime = time(NULL);
	if (ttsTime - nowTime > 0)
	{
		return 0;
	}
	else {
		return 1;
	}
}

// 若配置文件存在则读取，否则进行首次运行配置程序
void config_init() {
	FILE* inFile;
	char in[30] = "";
	int ini;
	inFile = fopen("cssystem.cfg", "r");
	if (inFile != NULL)
	{
		readCFG();
		return;				// 如果配置文件存在则跳过首次使用设置部分，进入读配置部分
	}

	int flag = 0;
	do
	{
		flag = 0;
		sprintf(dbIP, "");
		system("title 学生选课管理系统 - 首次使用设置系统");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t○●○●○● 欢迎使用学生选课管理系统 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("在开始使用之前，请先对系统进行设置。\n\n\n数据库部分（将在全部输入后检查连接可用性）——\n");
		printf("\n请输入MySQL数据库地址（留空默认为127.0.0.1）：");

		scanf("%[^\n]", dbIP);
		if (!strcmp(dbIP, ""))
		{
			sprintf(dbIP, "127.0.0.1");
		}
		while (getchar() != '\n'); {}

		printf("请输入MySQL数据库端口号（留空默认为3306）：");
		scanf("%[^\n]", in);
		if (!strcmp(in, ""))
		{
			sprintf(in, "3306");
		}
		dbPort = atoi(in);
		while (getchar() != '\n'); {}

		printf("请输入MySQL数据库用户名（留空默认为root）：");
		scanf("%[^\n]", dbUser);
		if (!strcmp(dbUser, ""))
		{
			sprintf(dbUser, "root");
		}
		while (getchar() != '\n'); {}

		printf("请输入MySQL数据库密码：");
		scanf("%[^\n]", dbPassWd);
		if (!strcmp(dbPassWd, ""))
		{
			sprintf(dbPassWd, "123456");
		}
		while (getchar() != '\n'); {}

		printf("请输入MySQL数据库库名称（留空默认为cssystem）：");
		scanf("%[^\n]", dbName);
		if (!strcmp(dbName, ""))
		{
			sprintf(dbName, "cssystem");
		}
		while (getchar() != '\n'); {}
		printf("%s %s %s %s %d", dbIP, dbUser, dbPassWd, dbName, dbPort);
		if (!mysql_real_connect(&mysql, dbIP, dbUser, dbPassWd, dbName, dbPort, NULL, 0))
		{
			printf("\n\n数据库连接失败！请确认配置是否正确，按回车重新配置……\n");
			flag = 1;
		}
		mysql_close(&mysql);
	} while (flag);


	printf("\n\n学期部分——\n");
	printf("\n当前学年（输入一位数后回车即可）：202");
	int ret = scanf("%d", &ini);
	while (ret != 1 || ini > 9 || ini < 0)
	{
		while (getchar() != '\n');
		{
			printf("无效，请重新输入：");
			ret = scanf("%d", &ini);
		}
	}
	currYear = ini;
	printf("第？学期（输入1或2）：");
	ret = scanf("%d", &ini);
	while (ret != 1 || ini > 2 || ini < 1)
	{
		while (getchar() != '\n');
		{
			printf("无效，请重新输入：");
			ret = scanf("%d", &ini);
		}
	}
	currTerm = ini;

	int date[5];
	char tmp[10];
	do {
		flag = 0;
		printf("输入当前学期开学时间（格式yyyy-mm-dd，如输入2020-8-31，必须为周一）：");
		ret = scanf("%d-%d-%d", &date[0], &date[1], &date[2]);
		while (ret != 3)
		{
			while (getchar() != '\n');
			{
				printf("无效，请重新输入：");
				ret = scanf("%d-%d-%d", &date[0], &date[1], &date[2]);
			}
		}
		if (date[0] < 2020 || date[0]>2029 || date[1] > 12
			|| date[1] < 1 || date[2] < 1 || date[2]>31)
		{
			flag = 1;
			printf("不符合日期规范，请重新输入。");
			continue;
		}

		currStart = convert_dateToTT(date[0], date[1], date[2], 0, 0, 0);
		strftime(tmp, sizeof(tmp), "%A", localtime(&currStart));
		if (strcmp(tmp, "Monday"))
		{
			flag = 1;
			printf("该日不是周一，请重新输入。");
		}

	} while (flag);

	do {
		flag = 0;
		printf("输入当前选课开始时间（格式yyyy-mm-dd-hh:mm，如输入2020-8-31-9:00）：");
		ret = scanf("%d-%d-%d-%d:%d"
			, &date[0], &date[1], &date[2], &date[3], &date[4]);
		while (ret != 5)
		{
			while (getchar() != '\n');
			{
				printf("无效，请重新输入：");
				ret = scanf("%d-%d-%d-%d:%d"
					, &date[0], &date[1], &date[2], &date[3], &date[4]);
			}
		}
		if (date[0] < 2020 || date[0]>2029 || date[1] > 12 || date[1] < 1
			|| date[2] < 1 || date[2]>31 || date[3] < 0 || date[3]>23
			|| date[4] < 0 || date[4]>59)
		{
			flag = 1;
			printf("不符合日期规范，请重新输入。");
			continue;
		}

		selecStart = convert_dateToTT(
			date[0], date[1], date[2], date[3], date[4], 0);

	} while (flag);

	do {
		flag = 0;
		printf("输入当前选课结束时间（格式yyyy-mm-dd-hh:mm，如输入2020-8-31-9:00）：");
		ret = scanf("%d-%d-%d-%d:%d"
			, &date[0], &date[1], &date[2], &date[3], &date[4]);
		while (ret != 5)
		{
			while (getchar() != '\n');
			{
				printf("无效，请重新输入：");
				ret = scanf("%d-%d-%d-%d:%d"
					, &date[0], &date[1], &date[2], &date[3], &date[4]);
			}
		}
		if (date[0] < 2020 || date[0]>2029 || date[1] > 12 || date[1] < 1
			|| date[2] < 1 || date[2]>31 || date[3] < 0 || date[3]>23
			|| date[4] < 0 || date[4]>59)
		{
			flag = 1;
			printf("不符合日期规范，请重新输入。");
			continue;
		}

		selecEnd = convert_dateToTT(date[0], date[1], date[2], date[3], date[4], 0);
		if (selecEnd < selecStart) {
			flag = 1;
			printf("选课结束时间不能早于开课时间，请重新输入。");
			continue;
		}
	} while (flag);

	FILE* outFile;
	if ((outFile = fopen("cssystem.cfg", "a+")) == NULL)
	{
		printf("\n打开配置文件失败！请尝试手动删除cfg配置文件后重试！");
		return;
	}
	fprintf(outFile, "# 学生选课管理系统配置文件\n# 手动修改请遵从文件格式（包括行数）\n\n# 数据库地址\n%s\n\n# 端口号\n%d\n\n# 用户名\n%s\n\n# 密码\n%s\n\n# 库名\n%s\n\n# 当前学期信息（学年、学期、开学时间、选课开始时间、结束时间）\n202%d\n%d\n%ld\n%ld\n%ld", dbIP, dbPort, dbUser, dbPassWd, dbName, currYear, currTerm, currStart, selecStart, selecEnd);
	fclose(outFile);
	printf("\n\n首次使用设置完毕，按任意键开始使用系统……");
	system("pause>nul");
}

// 读取配置文件
void readCFG() {
	FILE* inFile;
	char record[50] = { 0 };
	int lineNum = 0;		//当前行数
	if ((inFile = fopen("cssystem.cfg", "r")) == NULL)
	{
		printf("\n打开配置文件失败！请尝试手动删除cfg配置文件并执行首次使用设置！");
		getchar();
		exit(1);
	}
	int tmp[3];				//临时存待转换时间
	while (!feof(inFile)) {
		lineNum++;
		memset(record, 0, sizeof(record));
		fgets(record, sizeof(record) - 1, inFile);
		char* pos;
		pos = strchr(record, '\n');
		if (pos) {
			*pos = '\0';	//把取出的换行符替换为结束
		}

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
	if (mysql_query(&mysql, "select * from `teachers`"))
	{
		printf("\n教师数据表查询失败！请确认数据表是否存在\n");
	}
	else
	{
		mysql_store_result(&mysql);
		int option1;
		system("title 学生选课管理系统 - 教师登录");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t○●○●○● 欢迎登录学生选课管理系统--教师 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\n请选择您要进行的操作:\n");
		printf("  ① - 登录\n");
		printf("  ② - 注册\n");
		printf("  ③ - 返回上层\n\n");
		printf("请输入1，2或3：");
		int ret1 = scanf("%d", &option1);
		while (ret1 != 1 || option1 > 3 || option1 < 1)
		{
			while (getchar() != '\n');
			{
				printf("输入无效！请重新输入：");
				ret1 = scanf("%d", &option1);
			}
		}
		if (option1 == 1)
		{
			do {
				system("cls");			// 清屏，保证重复输入时美观
				system("title 学生选课管理系统 - 教师登录");
				printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
				printf("\t\t\t○●○●○● 欢迎登录学生选课管理系统 ●○●○●○\n");
				printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
				printf("请输入用户名：");
				scanf("%s", teachID);
				printf("请输入密码：");
				scanf("%s", password);
				printf("%d", check_password(0, teachID, password));
			} while (!check_password(1, teachID, password));
			mysql_store_result(&mysql);
			sprintf(query, "select school,name from teachers where teachID='%s'", teachID);
			mysql_query(&mysql, query);
			result = mysql_store_result(&mysql);
			if (result)
			{										// 防止数据为空造成崩溃
				if (mysql_num_rows(result) == 1)	// 若非有且仅有一行数据则登录失败
				{
					nextRow = mysql_fetch_row(result);
					sprintf(nowName, nextRow[1]);
					sprintf(nowSchool, nextRow[0]);
				}
				teacher_mainmenu();
			}
		}
		else if (option1 == 2)
		{
			teacher_reg();
			printf("\n请按任意键返回上一菜单\n");
			system("pause > nul");
			teacher_login();
		}
		else if (option1 == 3)
		{
			main_entrance();
		}
	}
}

void cm_list1()
{
	system("cls");
	char query[200] = "SELECT * FROM `classes` LIMIT 0, 1000";
	select_class(query);//打印相应查询内容
	int option2 = 0;
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请选择您需要的服务:\n\n");
	printf("  ① - 查询课程\n");
	printf("  ② - 返回上一个菜单\n\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请输入1或2:");
	while (scanf("%d", &option2) != 1 || option2 > 2 || option2 < 1)
	{
		printf("无效，请重新输入!\n");
		fflush(stdin);
	}
	switch (option2)
	{
	case 1:
		cm_list2();
		break;
	case 2:
		course_managemenu();
		break;
	default:
		printf("无效，请重新输入!\n");
	}


}
void cm_list2()
{
	char courseName[200] = "niconiconi";
	int column = mysql_num_fields(result);
	int flag = 0;
	do {
		printf("请输入您想要查询的课程名称：");
		scanf("%s", courseName);
		char query[200] = "SELECT * FROM `classes`WHERE 课程名称 = '";
		strcat(query, courseName);
		strcat(query, "'");
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);
		if (mysql_num_rows(result) == 0)
		{
			flag = 1;
			printf("无结果，请重新输入\n");
			mysql_free_result(result);
		}
		else {
			flag = 0;
		}
	} while (flag == 1);

	for (int i = 0; field = mysql_fetch_field(result); i++)
	{
		printf("%10s", field->name);
		printf(" |");
	}
	printf("\n");
	while (nextRow = mysql_fetch_row(result))
	{
		for (int j = 0; j < column; j++)
		{
			printf("%10s", nextRow[j]);
			printf(" |");
		}
		printf("\n");
	}
	printf("按任意键返回上一菜单...\n");
	system("pause>nul");
	cm_list1();
}

void cm_edit() {
	system("cls");
	char query[200] = "SELECT * FROM `classes`WHERE 课程名称 = '";
	int column;
	int stu_num;
	int flag = 0;
	int option2 = 0;
	char courseName[200] = "niconiconi";
	char studentName[200] = "niconiconi";
	strcat(query, nowName);
	strcat(query, "'");
	select_class(query);//打印相应查询内容
	printf("以上是您开设的课程\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("请选择您需要的服务:\n\n");
	printf("  ① - 修改课程内容\n");
	printf("  ② - 结束修改并返回上一菜单\n\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请输入1或2:");
	while (scanf("%d", &option2) != 1 || option2 > 2 || option2 < 1)
	{
		printf("无效，请重新输入!\n");
		fflush(stdin);
	}
	switch (option2)
	{
	case 1:
		do {
			printf("请输入您想要修改的课程的编号：");
			scanf("%s", courseName);
			sprintf(query, "SELECT * FROM `classes`WHERE 课程编号 = '%s'", courseName);
			mysql_query(&mysql, query);
			result = mysql_store_result(&mysql);
			column = mysql_num_fields(result);
			if (mysql_num_rows(result) == 0)
			{
				flag = 1;
				printf("无结果，请重新输入\n");
				mysql_free_result(result);
			}
			else {
				flag = 0;
			}
		} while (flag == 1);
		for (int i = 0; field = mysql_fetch_field(result); i++)
		{
			printf("%10s", field->name);
			printf(" |");
		}
		printf("\n");
		while (nextRow = mysql_fetch_row(result))
		{
			for (int j = 0; j < column; j++)
			{
				printf("%10s", nextRow[j]);
				printf(" |");
			}
			printf("\n");
		}
		sprintf(query, "SELECT `已选人数` FROM `classes`WHERE 课程编号 = '%s'", courseName);
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);
		nextRow = mysql_fetch_row(result);
		stu_num = atoi(nextRow[0]);
		printf("\n---------%d----------\n", stu_num);
		if (stu_num == 0)
		{
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\n未有学生选课，请选择您需要的修改的内容:\n");
			printf("  ① - 教材信息\n");
			printf("  ② - 课程简介\n");
			printf("  ③ - 限制人数\n\n");
			printf("  ④ - 返回上一个菜单\n\n");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("请输入1，2，3或4:");
			while (scanf("%d", &option2) != 1 || option2 > 4 || option2 < 1)
			{
				printf("无效，请重新输入!\n");
				fflush(stdin);
			}
			char content[1000];
			switch (option2)
			{
			case 1:
				printf("教材信息修改\n");
				printf("请输入您的修改内容：");
				scanf("%s", content);
				sprintf(query, "UPDATE `classes` SET `教材信息`='%s' WHERE`课程编号`='%s'"
					, content, courseName);
				mysql_query(&mysql, query);
				printf("修改成功！\n");
				printf("按任意键返回上一菜单...\n");
				system("pause>nul");
				cm_edit();
				break;

			case 2:
				printf("课程简介修改\n");
				printf("请输入您的修改内容：");
				scanf("%s", content);
				sprintf(query, "UPDATE `classes` SET `课程简介`='%s' WHERE`课程编号`='%s'"
					, content, courseName);
				mysql_query(&mysql, query);
				printf("修改成功！\n");
				printf("按任意键返回上一菜单...\n");
				system("pause>nul");
				sm_findcourse();
				cm_edit();
				break;

			case 3:
				printf("限制人数修改\n");
				sprintf(query, "SELECT `限制人数` FROM `classes`WHERE 课程编号 = '%s'"
					, courseName);
				mysql_query(&mysql, query);
				result = mysql_store_result(&mysql);
				nextRow = mysql_fetch_row(result);
				stu_num = atoi(nextRow[0]);
				printf("%d", stu_num);
				switch (stu_num)
			case 80:
				printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
				printf("您目前的限制人数为80人，是否修改成100人？\n");
				printf("  ① - 是\n");
				printf("  ② - 否\n\n");
				printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
				printf("\n请输入1或2:");
				while (scanf("%d", &option2) != 1 || option2 > 2 || option2 < 1)
				{
					printf("无效，请重新输入!\n");
					fflush(stdin);
				}
				switch (option2)
				{
				case 1:
					sprintf(query, "UPDATE `classes` SET `限制人数`='100' WHERE`课程编号`='%s'"
						, courseName);
					mysql_query(&mysql, query);
					printf("修改成功！\n");
					printf("按任意键返回上一菜单...\n");
					system("pause>nul");
					cm_edit();
					break;
				case 2:
					cm_edit();
					break;
				default:
					printf("无效，请重新输入!\n");
				}
				break;
			case 100:
				printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
				printf("您目前的限制人数为100人，是否修改成80人？\n");
				printf("  ① - 是\n");
				printf("  ② - 否\n\n");
				printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
				printf("\n请输入1或2:");
				while (scanf("%d", &option2) != 1 || option2 > 2 || option2 < 1)
				{
					printf("无效，请重新输入!\n");
					fflush(stdin);
				}
				switch (option2)
				{
				case 1:
					sprintf(query, "UPDATE `classes` SET `限制人数`='80' WHERE`课程编号`='%s'"
						, courseName);
					mysql_query(&mysql, query);
					printf("修改成功！\n");
					printf("按任意键返回上一菜单...\n");
					system("pause>nul");
					cm_edit();
					break;
				case 2:
					cm_edit();
					break;
				default:
					printf("无效，请重新输入!\n");
				}
				break;
			case 4:
				course_managemenu();
				break;
			default:
				printf("无效，请重新输入!\n");
			}
		}
		else
		{
			printf("您选择的该课程已有学生选择，只能修改限制人数");
			printf("限制人数修改\n");
			sprintf(query, "SELECT `限制人数` FROM `classes`WHERE 课程编号 = '%s'"
				, courseName);
			mysql_query(&mysql, query);
			result = mysql_store_result(&mysql);
			nextRow = mysql_fetch_row(result);
			stu_num = atoi(nextRow[0]);

			switch (stu_num) {
			case 80:
				printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
				printf("您目前的限制人数为80人，是否修改成100人？\n");
				printf("  ① - 是\n");
				printf("  ② - 否\n\n");
				printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
				printf("\n请输入1或2:");
				while (scanf("%d", &option2) != 1 || option2 > 2 || option2 < 1)
				{
					printf("无效，请重新输入!\n");
					fflush(stdin);
				}
				switch (option2)
				{
				case 1:
					sprintf(query, "UPDATE `classes` SET `限制人数`='100' WHERE`课程编号`='%s'"
						, courseName);
					mysql_query(&mysql, query);
					printf("修改成功！\n");
					printf("按任意键返回上一菜单...\n");
					system("pause>nul");
					cm_edit();
					break;
				case 2:
					cm_edit();
					break;
				default:
					printf("无效，请重新输入!\n");
				}
				break;
			case 100:
				sprintf(query, "SELECT `已选人数` FROM `classes`WHERE 课程编号 = '%s'"
					, courseName);
				mysql_query(&mysql, query);
				result = mysql_store_result(&mysql);
				nextRow = mysql_fetch_row(result);
				if (atoi(nextRow[0]) > 80)
				{
					printf("非常抱歉，您的课程已选人数已经超过80人，无法修改限制人数\n");
					printf("按任意键返回上一菜单...\n");
					system("pause>nul");
					cm_edit();
				}
				else {
					printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
					printf("您目前的限制人数为100人，是否修改成80人？\n");
					printf("  ① - 是\n");
					printf("  ② - 否\n\n");
					printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
					printf("\n请输入1或2:");
					while (scanf("%d", &option2) != 1 || option2 > 2 || option2 < 1)
					{
						printf("无效，请重新输入!\n");
						fflush(stdin);
					}
					switch (option2)
					{
					case 1:
						sprintf(query, "UPDATE `classes` SET `限制人数`='80' WHERE`课程编号`='%s'"
							, courseName);
						mysql_query(&mysql, query);
						printf("修改成功！\n");
						printf("按任意键返回上一菜单...\n");
						system("pause>nul");
						cm_edit();
						break;
					case 2:
						cm_edit();
						break;
					default:
						printf("无效，请重新输入!\n");
					}
				}
				break;
			default:
				printf("无效，请重新输入!\n");

			}
		};
		break;
	case 2:
		course_managemenu();
		break;
	default:
		printf("无效，请重新输入!\n");
	}

}

void cm_delete() {
	system("cls");
	int column;
	int row;
	int flag = 0;
	int option2 = 0;
	char courseName[200] = "niconiconi";
	char studentName[200] = "niconiconi";
	char query[200] = "SELECT * FROM `classes`WHERE 开课教师 = '";
	strcat(query, nowName);
	strcat(query, "'");
	select_class(query);//打印相应内容
	printf("以上是您开设的课程\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("请选择您需要的服务:\n\n");
	printf("  ① - 进行课程的删除\n");
	printf("  ② - 返回上一菜单\n\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请输入1或2:");
	while (scanf("%d", &option2) != 1 || option2 > 2 || option2 < 1)
	{
		printf("无效，请重新输入!\n");
		fflush(stdin);
	}
	switch (option2)
	{
	case 1:
		do {
			printf("请输入您想要删除的课程的编号：");
			scanf("%s", courseName);
			sprintf(query, "SELECT * FROM `classes`WHERE 课程编号 = '%s'", courseName);
			mysql_query(&mysql, query);
			result = mysql_store_result(&mysql);
			column = mysql_num_fields(result);
			if (mysql_num_rows(result) == 0)
			{
				flag = 1;
				printf("无结果，请重新输入\n");
				mysql_free_result(result);
			}
			else {
				flag = 0;
			}
		} while (flag == 1);
		for (int i = 0; field = mysql_fetch_field(result); i++)
		{
			printf("%10s", field->name);
			printf(" |");
		}
		printf("\n");
		while (nextRow = mysql_fetch_row(result))
		{
			for (int j = 0; j < column; j++)
			{
				printf("%10s", nextRow[j]);
				printf(" |");
			}
			printf("\n");
		}

		if (getState_selecting() == 0)
		{
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("确认要删除该课程吗？\n");
			printf("  ① - 是\n");
			printf("  ② - 否\n\n");
			printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
			printf("\n请输入1或2:");
			while (scanf("%d", &option2) != 1 || option2 > 2 || option2 < 1)
			{
				printf("无效，请重新输入!\n");
				fflush(stdin);
			}
			/*printf("===============%s===============", courseName);*/
			switch (option2)
			{
			case 1:
				sprintf(query, "DELETE FROM `classes` WHERE `课程编号` = '%s'"
					, courseName);
				mysql_query(&mysql, query);
				printf("删除成功！\n");
				printf("按任意键返回上一菜单...\n");
				system("pause>nul");
				cm_delete();
				break;
			case 2:
				cm_delete();
				break;
			default:
				printf("无效，请重新输入!\n");
			}
		}
		else
		{
			printf("非常抱歉，该课程选课已开始，您不能对进行删除操作\n");
			printf("按任意键返回上一菜单...\n");
			system("pause>nul");
			cm_delete();
		}

		break;
	case 2:
		select_managemenu();
		break;
	default:
		printf("无效，请重新输入!\n");


	}
}

void pm_edit()
{
	system("cls");
	int num;
	char passwd[20];
	char passwd1[20];
	char email[50];
	system("title 学生选课管理系统 - 教师信息修改");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 信息修改界面--教师 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请选择需要修改的信息：\n");
	printf("  ① - 密码\n");
	printf("  ② - 邮箱\n");
	printf("  ③ - 返回上一个菜单\n\n");
	printf("\n请输入1，2或3：");
	int ret4 = scanf("%d", &num);
	while (ret4 != 1 || num > 3 || num < 1)
	{
		while (getchar() != '\n');
		{
			printf("无效，请重新输入!\n");
			ret4 = scanf("%d", &num);
		}
	}

	switch (num)
	{
	case 1:
	{
		system("cls");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t○●○●○● 信息修改界面--教师--密码 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\n请输入新的密码：");
		scanf("%s", passwd);
		printf("请再次确认新的密码:\n");
		scanf("%s", passwd1);
		while (strcmp(passwd, passwd1) != 0)
		{
			printf("两次输入不一致!请重新确认:");
			scanf("%s", passwd1);
		}
		char query1[200] = "update teachers set passwd=' ";
		strcat(query1, passwd);
		strcat(query1, "' where teachID='");
		strcat(query1, teachID);
		strcat(query1, "'");
		mysql_query(&mysql, query1);
		printf("\n修改成功!\n");
		printf("按任意键返回上一菜单...\n");
		system("pause>nul");
		pm_edit();
		break;
	}
	case 2:
	{
		system("cls");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\t\t\t○●○●○● 信息修改界面--学生--邮箱 ●○●○●○\n");
		printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
		printf("\n请输入新的邮箱：");
		scanf("%s", email);
		while (check_email(email) == 0)
		{
			printf("无效输入！请按照***@***.***格式输入:");
			scanf("%s", email);
		}
		char query2[200] = "update teachers set email=' ";
		strcat(query2, email);
		strcat(query2, "' where teachID='");
		strcat(query2, teachID);
		strcat(query2, "'");
		mysql_query(&mysql, query2);
		printf("\n修改成功!\n");
		printf("按任意键返回上一菜单...\n");
		system("pause>nul");
		pm_edit();
		break;
	}
	case 3:
	{
		teacher_mainmenu();
		break;
	}
	default:
		printf("无效，请重新输入!\n");
	}
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
	if (mysql_num_rows(result) != 0) {
		printf("\n与现有课程冲突！\n");
		return 1;
	}
	return 0;
}
//加课
void cm_add() {
	char classId[20], name[50], credit[10], learnTime[20], property[10]
		, startTime[100], endTime[100], time[100], classroom[20]
		, limit[5], intro[500], book[50];
	char term[20];
	char query[1000];
	float in_f;
	int in, in1;
	int reflag = 0;					//部分语句块变为一就重新执行的标志
	int numClass1, numClass2;		//[1]为必修，[2]为选修课数量
	char in_s[20];
	printf("加课\n");


	printf("请选择开课时间——\n学年部分（输入一位数后回车即可）：202");
	int ret = scanf("%d", &in);
	while (ret != 1 || in > 9 || in < 0)
	{
		while (getchar() != '\n');
		{
			printf("无效，请重新输入：");
			ret = scanf("%d", &in);
		}
	}
	sprintf(startTime, "202%d-202%d学年第", in, in + 1);
	printf("第？学期（输入1或2）：");
	ret = scanf("%d", &in);
	while (ret != 1 || in > 2 || in < 1)
	{
		while (getchar() != '\n');
		{
			printf("无效，请重新输入：");
			ret = scanf("%d", &in);
		}
	}
	if (in == 1)
	{
		strcat(startTime, "一");
	}
	else {
		strcat(startTime, "二");
	}
	strcat(startTime, "学期");

	mysql_store_result(&mysql);				//清除数据防止出错
	sprintf(query, "select 课程编号,课程性质 from classes where 开课教师='%s' and 课程性质='必修' and 开课时间 like '%s"
		, nowName, startTime);
	strcat(query, "%'");
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	numClass1 = mysql_num_rows(result);		// 取所选学期必修课已开数
	sprintf(query, "select 课程编号,课程性质 from classes where 开课教师='%s' and 课程性质='选修' and 开课时间 like '%s"
		, nowName, startTime);
	strcat(query, "%'");
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	numClass2 = mysql_num_rows(result);		// 取所选学期选修课已开数

	strcpy(term, startTime);				//拷贝一份作为开课学期

	printf("您在 %s 已经开了 %d 门必修课、 %d 门选修课，还能再开 %d 门选修课\n"
		, startTime, numClass1, numClass2, 2 - numClass2);

	printf("\n选择课程性质：");
	printf("\n1、必修\n2、选修\n\n请选择本课程性质(1/2）:");
	ret = scanf("%d", &in);
	while (ret != 1 || in > 2 || in < 1 || (in == 2 && numClass2 > 1))
	{
		if (numClass2 > 1) {
			printf("您本学期的选修课开课数量已达上限！\n");
		}
		while (getchar() != '\n');
		{
			printf("无效，请重新输入：");
			ret = scanf("%d", &in);
		}
	}
	if (in == 1)
	{
		sprintf(property, "%s", "必修");
	}
	else
	{
		sprintf(property, "%s", "选修");

	}

	fflush(stdin);

	do
	{
		printf("请输入课程编号（6位数字）：");
		while (scanf("%s", classId) != 1 || !check_classId(classId))
		{
			printf("无效，请重新输入：");
			fflush(stdin);
		}
		//准备验证是否有相同ID的课
		sprintf(query, "select 课程性质 from classes where 课程编号='%s'", classId);
	} while (check_classClash(query));

	do
	{
		printf("请输入课程名称：");
		scanf("%s", name);
		fflush(stdin);
		//准备验证是否有相同名字的课
		sprintf(query, "select 课程性质 from classes where 课程名称='%s'", name);
	} while (check_classClash(query));

	printf("请输入课程学分（1-4，允许一位小数）：");
	ret = scanf("%f", &in_f);
	while (ret != 1 || in_f > 4.0 || in_f < 1.0)
	{
		while (getchar() != '\n');
		{
			printf("无效，请重新输入：");
			ret = scanf("%f", &in_f);
		}
	}
	sprintf(credit, "%.1f", in_f);			// 学分浮点转字符串

	printf("请输入课程学时（允许一位小数）：");
	ret = scanf("%f", &in_f);
	while (ret != 1)
	{
		while (getchar() != '\n');
		{
			printf("无效，请重新输入：");
			ret = scanf("%f", &in_f);
		}
	}
	sprintf(learnTime, "%.1f", in_f);		// 学时浮点转字符串

	do
	{
		reflag = 0;							//防止无限循环
		printf("请输入开课周次（输入1-20间整数）：");
		ret = scanf("%d", &in);
		while (ret != 1 || in > 20 || in < 1)
		{
			while (getchar() != '\n');
			{
				printf("无效，请重新输入：");
				ret = scanf("%d", &in);
			}
		}

		int sw;
		sw = in;							//存好开课周次判断结课大于开课

		sprintf(in_s, "第%d周", in);		//周次整型转字符串
		strcpy(endTime, startTime);			//开课结课时间同一学期直接复制
		strcat(startTime, in_s);


		printf("请输入结课周次（输入1-20间整数）：");
		ret = scanf("%d", &in);
		while (ret != 1 || in > 20 || in < 1 || in < sw)
		{
			while (getchar() != '\n');
			{
				printf("无效，请重新输入：");
				ret = scanf("%d", &in);
			}
		}
		sprintf(in_s, "第%d周", in);		//周次整型转字符串
		strcat(endTime, in_s);

		printf("\n该课程最终开课时间：%s", startTime);
		printf("\n该课程最终结课时间：%s\n\n", endTime);

		printf("\n时间表：\n1、8:00-8:50\n2、9:00-9:50\n3、10:00-10:50\n4、11:00-11:50\n5、13:30-14:20\n6、14:30-15:20\n7、15:30-16:20\n8、16:30-17:20\n9、18:30-19:20\n10、19:30-20:20");
		printf("\n输入格式：[周几(1-7)] [第几(1-10)节]\n若具体上课时间为每周三第五节，则输入应为：3 5");
		printf("\n请输入具体上课时间段：");
		ret = scanf("%d %d", &in, &in1);
		while (ret != 2 || in > 7 || in < 1 || in1 < 1 || in1 > 10)
		{
			while (getchar() != '\n');
			{
				printf("无效，请重新输入：");
				ret = scanf("%d %d", &in, &in1);
			}
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
		printf("\n该课程最终上课时间段为：%s", time);

		sprintf(query, "select 开课时间,结课时间,上课时间段 from classes where 开课教师='%s' and 上课时间段='%s' and 开课时间 like '%s"
			, nowName, time, term);
		strcat(query, "%'");				//准备取出该老师该学期可能有时间冲突的课
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);
		while (nextRow = mysql_fetch_row(result))
		{
			if (check_timeClash(nextRow[0], nextRow[1], nextRow[2], startTime, endTime, time))
			{
				reflag = 1;
				break;
			}
		}
		if (reflag)
		{
			printf("\n\n上课时间有冲突，请重新输入！\n\n");
			continue;
		}

		printf("\n\n上课地点格式：楼号-房间号。1表示教一楼2表示教二楼；房间号为3位数字\n请输入上课地点：");
		ret = scanf("%d-%d", &in, &in1);
		while (ret != 2 || in > 2 || in < 1 || in1 < 100 || in1 > 999)
		{
			while (getchar() != '\n');
			{
				printf("无效，请重新输入：");
				ret = scanf("%d-%d", &in, &in1);
			}
		}
		sprintf(classroom, "%d-%d", in, in1);
		sprintf(query, "select 开课时间,结课时间,上课时间段 from classes where 上课地点='%s'"
			, classroom);					//准备验证是否有相同教室的课
		if (check_classClash(query)) {		//若有，则判断时间是否冲突
			sprintf(query, "select 开课时间,结课时间,上课时间段 from classes where 上课地点='%s'"
				, classroom);
			mysql_query(&mysql, query);
			result = mysql_store_result(&mysql);
			while (nextRow = mysql_fetch_row(result))
			{
				if (check_timeClash(nextRow[0], nextRow[1], nextRow[2], startTime, endTime, time))
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


	printf("\n\n请选择人数上限(80/100）：");
	ret = scanf("%d", &in);
	while (ret != 1 || !(in == 80 || in == 100))
	{
		while (getchar() != '\n');
		{
			printf("无效，请重新输入：");
			ret = scanf("%d", &in);
		}
	}
	if (in == 80)
	{
		sprintf(limit, "80");
	}
	else {
		sprintf(limit, "100");
	}


	printf("请输入课程介绍：");
	scanf("%s", intro);

	printf("请输入课程教材：");
	scanf("%s", book);

	sprintf(query, "INSERT INTO `classes` (`课程编号`, `开课学院`, `课程名称`, `学分`, `学时`, `课程性质`, `开课教师`, `开课时间`, `结课时间`, `上课时间段`, `上课地点`, `课程简介`, `教材信息`) VALUES ('%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s', '%s')"
		, classId, nowSchool, name, credit, learnTime, property, nowName
		, startTime, endTime, time, classroom, intro, book);
	printf(query);

	if (mysql_query(&mysql, query))
	{
		printf("\n数据库读写失败，请重试\n");
	}
	else
	{
		printf("\n加课成功！\n");
	}


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
	char teachID[11], school[50], major[50], name[50], sexual[5], phone[100], passwd[100], email[100];
	system("title 学生选课管理系统 - 教师注册");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 注册 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("请输入教师工号:");
	scanf("%s", teachID);
	char query[100] = "select * from teachers where teachID='";
	strcat(query, teachID);
	strcat(query, "'");
	mysql_query(&mysql, query);
	result = mysql_store_result(&mysql);
	while (mysql_num_rows(result) != 0)
	{
		printf("此教师工号已注册!请更换教师工号:");
		scanf("%s", teachID);
		char query11[100] = "select * from teachers where teachID='";
		strcat(query11, teachID);
		strcat(query11, "'");
		mysql_query(&mysql, query11);
		result = mysql_store_result(&mysql);
	}


	while (check_stuId(stuID) == 0)//检查输入是否符合规范 
	{
		printf("无效输入！请输入10位数字:");
		scanf("%s", stuID);
	}
	char query1[200] = "insert into teachers(teachID) values(  ";
	strcat(query1, "'");
	strcat(query1, teachID);
	strcat(query1, "'");
	strcat(query1, ")");
	mysql_query(&mysql, query1);

	printf("请输入学院:");
	scanf("%s", school);
	char query2[200] = "update teachers set school=' ";
	strcat(query2, school);
	strcat(query2, "' where teachID='");
	strcat(query2, teachID);
	strcat(query2, "'");
	mysql_query(&mysql, query2);

	printf("请输入姓名:");
	scanf("%s", name);
	char query4[200] = "update teachers set name=' ";
	strcat(query4, name);
	strcat(query4, "' where teachID='");
	strcat(query4, teachID);
	strcat(query4, "'");
	mysql_query(&mysql, query4);

	printf("请输入电话:");
	scanf("%s", phone);
	while (check_phone(phone) == 0)
	{
		printf("无效输入！请输入11位电话号:");
		scanf("%s", phone);
	}
	char query6[200] = "update teachers set phone=' ";
	strcat(query6, phone);
	strcat(query6, "' where teachID='");
	strcat(query6, teachID);
	strcat(query6, "'");
	mysql_query(&mysql, query6);

	printf("请输入密码:");
	scanf("%s", passwd);
	char query7[200] = "update teachers set passwd=' ";
	strcat(query7, passwd);
	strcat(query7, "' where teachID='");
	strcat(query7, teachID);
	strcat(query7, "'");
	mysql_query(&mysql, query7);

	printf("请输入邮箱:");
	scanf("%s", email);
	while (check_email(email) == 0)
	{
		printf("无效输入！请按照***@***.***格式输入:");
		scanf("%s", email);
	}
	char query8[200] = "update teachers set email=' ";
	strcat(query8, email);
	strcat(query8, "' where teachID='");
	strcat(query8, teachID);
	strcat(query8, "'");
	mysql_query(&mysql, query8);

	printf("\n注册成功！\n");

}
