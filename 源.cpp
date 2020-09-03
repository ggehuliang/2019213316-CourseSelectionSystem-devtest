#include <stdio.h>
#include <Windows.h>
#include <stdlib.h>
#include <winsock.h>
#include <mysql.h> 
#include <string.h>
#pragma comment (lib, "libmysql.lib")

int check_stuId(char* str);
int showinfo();
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

int check_classId(char* str);
void select_class(char* query);
void teacher_login();   // 教师登录模块
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
int  getState_selecting();  // 获取选课状态

MYSQL mysql;     //创造一个MYSQL句柄
MYSQL_RES* result;
MYSQL_RES* result1;
MYSQL_RES* result2;
MYSQL_RES* result3;
MYSQL_RES* result4;
MYSQL_RES* result5;
MYSQL_RES* result6;

MYSQL_FIELD* field;
MYSQL_ROW nextRow;
MYSQL_ROW Row;
MYSQL_ROW Row1;
MYSQL_ROW Row2;
MYSQL_ROW Row3;
MYSQL_ROW Row4;
//void teacher_login();
char stuID[11];

int main()
{
	mysql_init(&mysql);
	sql();

	switch (showinfo())
	{
	case 1:
		system("cls");
		student_login();
		break;
	case 2:
		system("cls");
		break;
	case 3:
		exit(0);
	}
	return 0;
}

int showinfo()
{
	int option;
	system("title 学生选课管理系统");
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

	return option;
}

void student_login()
{
	int option1;
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 欢迎登录学生选课管理系统--学生 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请选择您要进行的操作:\n");
	printf("  ① - 登录\n");
	printf("  ② - 注册\n");
	printf("  ③ - 退出程序\n\n");
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

		char query[100] = "select * from students where stuID='";
		strcat(query, stuID);
		strcat(query, "'");
		mysql_store_result(&mysql);
		mysql_query(&mysql, query);
		result = mysql_store_result(&mysql);
		if (result)
		{
			nextRow = mysql_fetch_row(result);
		}
		while (mysql_num_rows(result) == 0 || check_stuId(stuID) == 0)
		{
			if (check_stuId(stuID) == 0)
				printf("无效输入！请输入10位数字\n");
			else if (mysql_num_rows(result) == 0)
				printf("此学号未注册，请重新输入！\n");
			scanf("%s", stuID);
			char query1[100] = "select * from students where stuID='";
			strcat(query1, stuID);
			strcat(query1, "'");
			mysql_store_result(&mysql);
			mysql_query(&mysql, query1);
			result = mysql_store_result(&mysql);
			if (result)
			{
				nextRow = mysql_fetch_row(result);
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
		exit(0);
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
	char yon[1];
	int YesOrNo = 1;
	MYSQL_RES* res;  //查询结果集
	MYSQL_FIELD* field;  //包含字段信息的结构指针
	MYSQL_ROW nextRow;  //存放查询sql语句字符串数组
	int ret;  //执行sql语句后返回是否成功查询
	int i, j;

	mysql_init(&mysql);//分配对象 p4302
	if (!(mysql_real_connect(&mysql, "localhost", "root", "123456", "courseselectionsystem", 3306, NULL, 0))) {
		printf("Failed to access to the database...Error: %s\n", mysql_error(&mysql));
	}
	if (!mysql_set_character_set(&mysql, "gbk"))
	{
		printf("New client character set: %s\n",
			mysql_character_set_name(&mysql));
	}
}

int check_password(int who, char* ID, char* password) // 第一个参数学生为0，教师为1；登录失败返回0，成功返回1
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
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 功能界面--学生 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n登陆成功！\n");
	printf("\n请选择你需要的操作：\n");
	printf("  ① - 学生选课\n");
	printf("  ② - 查询课程 \n");
	printf("  ③ - 查询选课结果\n");
	printf("  ④ - 删除选课结果\n");
	printf("  ⑤ - 个人信息管理\n");
	printf("  ⑥ - 查看课程详细信息\n\n");
	printf("  ⑦ - 退出程序\n");
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
		exit(0);
}

void student_reg()
{
	system("cls");
	char stuID[11], school[50], major[50], name[50], sexual[5], phone[100], passwd[100], email[100];
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
	MYSQL_RES* result8;
	system("cls");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 学生选课 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	char classID[50];
	printf("\n课程信息如下：\n");
	char query[50] = "select * from classes";
	select_class(query);
	printf("\n请输入您想选的课程编号：");
	scanf("%s", classID);
	//判断是否有这门课
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
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 删除选课结果 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n您的已选课程如下:\n");
	student_query_result();
	printf("\n请输入课程编号以删除课程:");
	scanf("%s", classID);
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
	system("cls");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 查看课程详细信息--学生 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");


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

void teacher_mainmenu()
{
	system("cls");
	int option2 = 0;
	system("title 教师主菜单");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\t\t\t○●○●○● 教师主菜单 ●○●○●○\n");
	printf("━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━\n");
	printf("\n请选择您需要的服务:\n");
	printf("  ① - 选课管理\n");
	printf("  ② - 课程管理\n");
	printf("  ③ - 个人信息管理\n\n");
	printf("  ④ - 返回上一个菜单\n\n");
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
		showinfo();
		break;
	}

}

void select_managemenu() {
	system("cls");
	int option2 = 0;
	system("title 选课菜单");
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
	char courseName[200] = "niconiconi";
	char studentName[200] = "niconiconi";
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
	system("title 课程管理菜单");
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


