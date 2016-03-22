#ifndef heard_h
#define heard_h
#include"head1.h"
#include"lianbiao.h"
#include<stdbool.h>


typedef struct _users         //学生用户
{
	char name[17];
	char password[18];
	struct lianbiao user;
}users;
typedef struct _user_message  //学生信息
{
	char name[18];
	char QQ[12];
	char sex[5];
	char phone[13];
	char age[5];
	char address[32];
	struct lianbiao message;
}messages;




/*----------------------进入信息录入界面-----------------------*/
bool lurujiemian(messages* message_head)
{
	messages* message_new = (messages*)malloc(sizeof(messages));
	if(message_new == NULL)
	{
		perror("录入失败");
		exit(0);
	}
	initalize_data(&message_new->message);
	insert_data(&message_head->message, &message_new->message);
	

	/*==========================信息录入============================*/
	while(1)
	{
		printf("\033[2J\033[2;20H班级学生信息管理系统");
		printf("\033[6;10H\033[;31m姓名：\033[0m");
		printf("\033[6;34H\033[;31mQQ：\033[0m");
		printf("\033[9;10H\033[;31m性别：\033[0m");
		printf("\033[9;34H\033[;31m电话：\033[0m");
		printf("\033[12;10H\033[;31m年龄：\033[0m");
		printf("\033[12;34H\033[;31m地址：\033[0m");
		char name[25];
		char QQ[25];
		char sex[25];
		char phone[25];
		char age[25];
		char address[100];
		/*------------------------录入姓名----------------------*/
		while(1)
		{
				bzero(name,25);
				printf("\033[6;10H\033[;31m姓名：\033[0m\033[K");
				printf("\033[6;34H\033[;31mQQ：\033[0m");
		
				printf("\033[6;10H\033[;31m姓名：\033[0m");
				fgets(name,25,stdin);
				if(strlen(name)<=17&&strlen(name)>1)
				{
					bzero(message_new->name, 18);
					strcpy(message_new->name, name);
					break;
				}
		}/*---------------------录入姓名结束---------------------*/
		
		
		/*-------------------------录入QQ------------------------*/
		while(1)
		{
				bzero(QQ,25);
				printf("\033[6;34H\033[;31mQQ：\033[0m\033[K"); fgets(QQ,25,stdin);
				if(strlen(QQ)>7&&strlen(QQ)<=12)
				{
					bzero(message_new->QQ, 13);
					strcpy(message_new->QQ, QQ);
					break;
				}
		}/*-----------------------录入QQ结束---------------------*/



		/*-------------------------录入性别----------------------*/
		while(1)
		{
				bzero(sex,25);
				printf("\033[9;10H\033[;31m性别：\033[0m\033[K");
				printf("\033[9;34H\033[;31m电话：\033[0m");
		
				printf("\033[9;10H\033[;31m性别：\033[0m");
				fgets(sex,25,stdin);
				if(strlen(sex) <= 4&&strlen(sex)>1)
				{
					bzero(message_new->sex,5);
					strcpy(message_new->sex, sex);
					break;
				}
		}/*----------------------录入性别结束---------------------*/



		/*-------------------------录入电话-----------------------*/
		while(1)
		{
				bzero(phone,25);
				printf("\033[9;34H\033[;31m电话：\033[0m\033[K"); fgets(phone,25,stdin);
				if(strlen(phone) == 12)
				{
					bzero(message_new->phone,13);
					strcpy(message_new->phone, phone);
					break;
				}
		}/*----------------------录入电话结束----------------------*/


		/*-------------------------录入年龄------------------------*/
		while(1)
		{
				bzero(age,25);
				printf("\033[12;10H\033[;31m年龄：\033[0m\033[K");
				printf("\033[12;34H\033[;31m地址：\033[0m");
		
				printf("\033[12;10H\033[;31m年龄：\033[0m");
				fgets(age,25,stdin);
				if(strlen(age)<= 4&&strlen(age)>1)
				{
					bzero(message_new->age, 5);
					strcpy(message_new->age, age);
					break;
				}
		}/*----------------------年龄录入结束-----------------------*/


		/*-------------------------录入地址-------------------------*/
		while(1)
		{
				bzero(address,100);
				printf("\033[12;34H\033[;31m地址：\033[0m\033[K"); fgets(address,100,stdin);
				if(strlen(address) <= 31 && strlen(address)>1)
				{
					bzero(message_new->address,32);
					strcpy(message_new->address, address);
					break;
				}
		}/*-----------------------地址录入结束----------------------*/

	/*========================结束全部信息录入============================*/


		printf("\033[2J\033[2;28H你填写信息如下:");
		printf("\033[4;1H姓名-----性别----年龄----电话------------QQ------------地址\n");


		printf("\033[5;1H%s\033[5;10H%s\033[5;19H%s\033[5;26H%s\033[5;42H%s\033[5;56H%s",message_new->name,message_new->sex,message_new->age,message_new->phone,message_new->QQ,message_new->address);


		while(1)
		{	
			printf("\033[7;24H确认录入信息(y/n)");
			char b1;
			char buf[2];
			bzero(buf,2);
			scanf("%c",&b1);
			while(getchar()!='\n');
			if(b1 == 'n')
			{
				printf("\033[2J\033[2;24H录入失败");
				printf("\033[4;20H请输入回车键返回...");
				fgets(buf,10,stdin);
				return false;
			}else if(b1 == 'y')
			{
				printf("\033[2J\033[2;24H录入成功");
				printf("\033[4;20H请输入回车键返回...");
				fgets(buf,10,stdin);
				return true;
			}
		}

	}//退出while(1)

}/*-------------------------信息录入完成-----------------------*/



/*--------------------------进入删除界面-----------------------*/
bool shanchujiemian(messages* message_head)
{
	char buf[100];
	char buf1[100];
	while(1)
	{
		printf("\033[2J\033[2;20H班级学生信息管理系统 输入要删除的名字：\033[;31m\033[K");
		fgets(buf, 100, stdin); printf("\033[0m");
		if(strlen(buf)<20 && strlen(buf)>1) break;
	}
	struct lianbiao *q;
	int x = 0;
	list_for(q, &message_head->message)
	{
		messages *pos = pos_address(q, messages, message);
		if(strcmp(pos->name, buf)==0)
		{
			printf("\033[5;1H姓名-----性别----年龄----电话------------QQ------------地址\n");


			printf("\033[7;1H%s\033[7;10H%s\033[7;19H%s\033[7;26H%s\033[7;42H%s\033[7;56H%s",pos->name,pos->sex,pos->age,pos->phone,pos->QQ,pos->address);
			while(1)
			{
				printf("\033[13;20H这是你要删除的信息(y/n)\033[K");
				fgets(buf1,100,stdin);
				if(strlen(buf1)==2)
				{
					if(buf1[0]=='y'||buf1[0]=='n') break;
				}
			}
			if(buf1[0] == 'y')
			{
				printf("\033[13;20H删除后将不能恢复\033[K");
				while(1)
				{
					printf("\033[15;22H确认(y)\033[15;30H取消(n)");
					char a;
					scanf("%c",&a);
					if(a == 'n')  return true;
					else if(a == 'y')
					{
						delete_data(&(pos->message));
						printf("\033[2J\033[2;20H删除成功");
						printf("\033[12;18H请输入回车键返回...");
						fgets(buf1,100,stdin);
						return false;
					}
				}
			}
		}
	}//退出for
	printf("\033[2J\033[3;20H已搜索完没有你想要删除的学生信息");
	printf("\033[7;28H请输入回车键返回...");
	fgets(buf,100,stdin);
	return true;
}/*-------------------------退出删除界面-----------------------*/



/*-------------------------进入查询界面-----------------------*/
bool chaxunjiemian(messages* message_head)
{
	
	char buf[100];
	char buf1[100];
	while(1)
	{
		printf("\033[2J\033[2;20H班级学生信息管理系统 输入要查询的名字：\033[;31m\033[K");
		fgets(buf, 100, stdin); printf("\033[0m");
		if(strlen(buf)<20 && strlen(buf)>1) break;
	}
	struct lianbiao *q;
	int x = 0;
	list_for(q, &message_head->message)
	{
		messages *pos = pos_address(q, messages, message);
		if(strcmp(pos->name, buf)==0)
		{
			printf("\033[5;1H姓名-----性别----年龄----电话------------QQ------------地址\n");


			printf("\033[7;1H%s\033[7;10H%s\033[7;19H%s\033[7;26H%s\033[7;42H%s\033[7;56H%s",pos->name,pos->sex,pos->age,pos->phone,pos->QQ,pos->address);
			while(1)
			{
				printf("\033[13;20H这是你要查询的信息(y/n)\033[K");
				fgets(buf1,100,stdin);
				if(strlen(buf1)==2)
				{
					if(buf1[0]=='y'||buf1[0]=='n') break;
				}
			}
			if(buf1[0] == 'y')
			{
				printf("\033[13;20H返回请输入回车键...\033[K");
				fgets(buf1,100,stdin);
				return false;
			}
		}
	}//退出for
	printf("\033[2J\033[3;20H已搜索完没有你想要查询的学生信息");
	printf("\033[7;28H请输入回车键返回...");
	fgets(buf,100,stdin);
	return true;
}/*-------------------------退出查询界面-----------------------*/


/*-------------------------进入修改界面-----------------------*/
bool xiugaijiemian(messages* message_head)
{
	char buf[100];
	char buf1[100];
	while(1)
	{
		printf("\033[2J\033[2;20H班级学生信息管理系统 输入要修改的名字：\033[;31m\033[K");
		fgets(buf, 100, stdin); printf("\033[0m");
		if(strlen(buf)<20 && strlen(buf)>1) break;
	}
	struct lianbiao *q;
	int x = 0;
	list_for(q, &message_head->message)
	{
		messages *pos = pos_address(q, messages, message);
		if(strcmp(pos->name, buf)==0)
		{
			printf("\033[5;1H姓名-----性别----年龄----电话------------QQ------------地址\n");


			printf("\033[7;1H%s\033[7;10H%s\033[7;19H%s\033[7;26H%s\033[7;42H%s\033[7;56H%s",pos->name,pos->sex,pos->age,pos->phone,pos->QQ,pos->address);
			while(1)
			{
				printf("\033[13;20H这是你想要修改的信息(y/n)\033[K");
				fgets(buf1,100,stdin);
				if(strlen(buf1)==2)
				{
					if(buf1[0]=='y'||buf1[0]=='n') break;
				}
			}
			if(buf1[0] == 'y')
			{
				while(1)
				{
					printf("\033[13;20H你需要修改全部信息(y/n)\033[K");
					fgets(buf1,100,stdin);
					if(strlen(buf1)==2)
					{
						if(buf1[0] == 'y')
						{
							printf("\033[13;20H此功能暂不支持\033[K");
							printf("\033[15;19H请输入回车键返回...\033[K");
							fgets(buf1,100,stdin);
							continue;
						}
						else if(buf1[0] == 'n')
						{
							while(1)
							{
								printf("\033[13;1H\033[K");
								printf("\033[13;35H退出(exit)");
								printf("\033[13;1H请输入你想要修改的名称:");
								fgets(buf1,100,stdin);
								buf1[strlen(buf1)-1] = '\0';
								if(strcmp(buf1,"姓名")==0)
								{
									while(1)
									{
										printf("\033[15;1H姓名:\033[K");
									
										fgets(buf1,100,stdin);
										
										if(strlen(buf1)>2 && strlen(buf1)<18)
										{
											strcpy(pos->name,buf1);
											printf("\033[13;1H\033[K");
											printf("\033[13;25H修改成功");
											printf("\033[15;1H\033[K");
											printf("\033[15;21H请输入回车键返回");
											fgets(buf1,100,stdin);
											printf("\033[15;1H\033[K");
											break;
										}
									}
								}else if(strcmp(buf1,"性别")==0)
								{
									while(1)
									{
										printf("\033[15;1H性别:\033[K");
										fgets(buf1,100,stdin);
										if(strlen(buf1)>2 && strlen(buf1)<18)
										{
											strcpy(pos->sex,buf1);
											printf("\033[13;1H\033[K");
											printf("\033[13;25H修改成功");
											printf("\033[15;1H\033[K");
											printf("\033[15;21H请输入回车键返回");
											fgets(buf1,100,stdin);
											printf("\033[15;1H\033[K");
											break;
										}
									}
									
								}else if(strcmp(buf1,"年龄")==0)
								{
									while(1)
									{
										printf("\033[15;1H年龄:\033[K");
										fgets(buf1,100,stdin);
										if(strlen(buf1)>2 && strlen(buf1)<18)
										{
											strcpy(pos->age,buf1);
											printf("\033[13;1H\033[K");
											printf("\033[13;25H修改成功");
											printf("\033[15;1H\033[K");
											printf("\033[15;21H请输入回车键返回");
											fgets(buf1,100,stdin);
											printf("\033[15;1H\033[K");
											break;
										}
									}
									
								}else if(strcmp(buf1,"电话")==0)
								{
									while(1)
									{
										printf("\033[15;1H电话:\033[K");
										fgets(buf1,100,stdin);
										if(strlen(buf1)>2 && strlen(buf1)<18)
										{
											strcpy(pos->phone,buf1);
											printf("\033[13;1H\033[K");
											printf("\033[13;25H修改成功");
											printf("\033[15;1H\033[K");
											printf("\033[15;21H请输入回车键返回");
											fgets(buf1,100,stdin);
											printf("\033[15;1H\033[K");
											break;
										}
									}
									
								}else if(strcmp(buf1,"QQ")==0)
								{
									while(1)
									{
										printf("\033[15;1HQQ:\033[K");
										fgets(buf1,100,stdin);
										if(strlen(buf1)>2 && strlen(buf1)<18)
										{
											strcpy(pos->QQ,buf1);
											printf("\033[13;1H\033[K");
											printf("\033[13;25H修改成功");
											printf("\033[15;1H\033[K");
											printf("\033[15;21H请输入回车键返回");
											fgets(buf1,100,stdin);
											printf("\033[15;1H\033[K");
											break;
										}
									}
									
								}else if(strcmp(buf1,"地址")==0)
								{
									while(1)
									{
										printf("\033[15;1H地址:\033[K");
										fgets(buf1,100,stdin);
										if(strlen(buf1)>2 && strlen(buf1)<18)
										{
											strcpy(pos->address,buf1);
											printf("\033[13;1H\033[K");
											printf("\033[13;25H修改成功");
											printf("\033[15;1H\033[K");
											printf("\033[15;21H请输入回车键返回");
											fgets(buf1,100,stdin);
											printf("\033[15;1H\033[K");
											break;
										}
									}
									
								}else if(strcmp(buf1,"exit")==0)
								{
									return true;
								}
							}//退出while(1)
						}//退出else if
					}//退出if
				}//退出while(1)

			}//退出if
		}
	}//退出for
	printf("\033[2J\033[3;20H已搜索完没有你想要修改的学生信息");
	printf("\033[7;28H请输入回车键返回...");
	fgets(buf,100,stdin);
	return true;
}/*------------------------退出修改界面------------------------*/



/*------------------------进入显示界面------------------------*/
bool xianshiquanbuxinxi(messages* message_head)
{
	struct lianbiao *q;
	char buf[100];
		printf("\033[2J\033[2;20H班级学生信息管理系统");
			printf("\033[5;1H姓名-----性别----年龄----电话------------QQ------------地址\n");
	list_for(q, &message_head->message)
	{
		messages *pos = pos_address(q, messages, message);

		printf("\033[1B%s\033[1A\033[10C%s\033[1A\033[18C%s\033[1A\033[25C%s\033[1A\033[41C%s\033[1A\033[55C%s",pos->name,pos->sex,pos->age,pos->phone,pos->QQ,pos->address);
	}
	printf("\033[1B\033[26C请输入回车键返回...");
	fgets(buf,100,stdin);
	return true;
}/*------------------------退出显示界面------------------------*/

/*----------------------------用户匹配-------------------------*/
bool yonghupipei(users* user_head, char *user, char *password, int yn)
{
	struct lianbiao *q = (&user_head->user)->next;
	while(q != &user_head->user)
	{
		users *pos = pos_address(q, users, user);
		if(yn == 0)
		{
			if(strcmp(pos->name, user)==0&&
			strcmp(pos->password, password)==0)  return false;
		}
		else if(yn == 1)
		{
			if(strcmp(pos->name, user)==0) return false;
		}
		printf("%s",pos->name);
		q = q->next;
	}
	return true;
}/*---------------------------退出用户匹配---------------------*/


/*----------------------------进入系统界面---------------------*/
bool xitongjiemian(messages* message_head)
{
	while(1)
	{
		printf("\033[2J\033[2;20H班级管理系统");
		printf("\033[5;21H1.录入");
		printf("\033[7;21H2.删除");
		printf("\033[9;21H3.查询（只以名称查询）");
		printf("\033[11;21H4.修改");
		printf("\033[13;21H5.查看全部信息");
		printf("\033[15;21H6.退出");
		int i;
		scanf("%d",&i);
		while(getchar()!='\n');
		if(i == 1)
		{
			lurujiemian(message_head);
		}else if(i == 2)
		{
			shanchujiemian(message_head);
		}else if(i == 3)
		{
			chaxunjiemian(message_head);
		}else if(i == 4)
		{
			xiugaijiemian(message_head);
		}else if(i == 5)
		{
			xianshiquanbuxinxi(message_head);
		}else if(i == 6) return 0;
	}//退出while(1)
}/*----------------------------退出系统界面---------------------*/






/*---------------------------进入主界面---------------------------*/
int zhujiemian()
{
	while(1)
	{
		int i=0;
		printf("\033[2J\033[3;20H班级学生管理系统");
		printf("\033[4;24H1.登入");
		printf("\033[6;24H""2.注册");
		printf("\033[8;14H""请输入选项[ ](1-登入 2-注册 3-退出)");
		printf("\033[8;25H"); scanf("%d",&i);
		while(getchar() != '\n');
		if(i==1) return 0;   //登录
		if(i==2) return 1;   //注册
		if(i==3) return 3;
	}
}
/*---------------------------跳出主界面-----------------------*/

/*---------------------------进入登录界面---------------------------*/
int denglujiemian(users* user_head, messages* message_head)
{
	char buf[20];
	char buf1[20];
	printf("\033[2J""\033[2;20H""班级学生管理系统");
	printf("\033[6;18H""用户：[                    ]");
	printf("\033[8;18H""密码：[                    ]");
	printf("\033[10;14H""输入密码用回车登入");
	printf("\033[6;18H""用户：["); fgets(buf, 20, stdin);
	printf("\033[8;25H\033[8m"); fgets(buf1, 20, stdin);
	printf("\033[8;30H\033[0m\n");
	while(yonghupipei(user_head, buf, buf1, 0))
	{
		printf("\033[2J\033[2;24H登录失败");
		return true;
	}
	xitongjiemian(message_head);    //跳转到系统界面
	return false;
}
/*---------------------------跳出登录界面-----------------------*/


/*---------------------------进入注册界面---------------------------*/
int zhucejiemian(users* user_head, users* user_new, messages* message_head)
{
	while(1)
	{
		char buf[17];
		char buf1[20];
		bzero(buf, 15);
		bzero(buf1, 20);
		printf("\033[2J""\033[2;20H""班级学生管理系统");
		printf("\033[6;18H""用户：[                ]注:用户名长度为1-10字节");
		printf("\033[8;18H""密码：[                ]注:密码长度为6-16");
		printf("\033[10;14H""输入密码用回车登入");
		printf("\033[6;18H""用户：["); fgets(buf, 17, stdin);
		printf("\033[8;25H"); fgets(buf1, 20, stdin);
		//printf("\033[8;30H\033[0m\n");
		int len1 = strlen(buf);
		int len2 = strlen(buf1);


		/*----------------判断用户名以及密码长度---------------*/
		if(len1 > 16||len1 < 1 || len2 > 18||len2 < 7)
		{
			printf("\033[2J\033[3;24H用户名或密码长度不对");
			while(1)
			{
				printf("\033[5;28H重新注册(y/n)");
				char i;
				scanf("%c",&i);
				while(getchar() != '\n');
				if(i != 'y' & i !='n') continue;
				else if(i == 'n') return true;
				else if(i == 'y') break;
			}
			continue;
		}//跳出判断


		/*---------判断用户是否已经存在-----------*/
		while(yonghupipei(user_head, buf, NULL, 1))
		{
			initalize_data(&user_new->user);
			insert_data(&user_head->user, &user_new->user);
			strcpy(user_new->name,buf);
			strcpy(user_new->password,buf1);
			printf("\033[2J\033[2;24H注册成功");
		//	printf("\033[6;20H输入任意键返回主界面...");
		//	system("stty raw -echo");
		//	int c = getchar();
		//	system("stty cooked echo");
			while(1)
			{
				printf("\033[5;21H直接登录(y/n)");
				char a;
				scanf("%c",&a);
				while(getchar()!='\n');
				if(a == 'y')
				{
					xitongjiemian(message_head);   //跳转到系统界面
					return false;
				}
				else if(a == 'n') return false;
			}
		}
		printf("\033[2J\033[3;24H注册失败");
		printf("\033[5;21H该用户已被注册");
		while(1)
		{
			printf("\033[8;20H重新注册(y/n)");
			char i;
			scanf("%c",&i);
			while(getchar() != '\n');
			if(i != 'y' & i !='n') continue;
			else if(i == 'n') return true;
			else if(i == 'y') break;
		}
	}//退出while(1)

}/*----------------------------跳出注册界面---------------------------*/














#endif
