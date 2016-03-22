#include"heard.h"
int user_fd;
int message_fd;
users *user_head;
messages *message_head;

/*------------------------处理函数------------------------*/
void fun1()
{
	user_fd = open("./.user", O_WRONLY|O_TRUNC);
	if(user_fd == -1)
	{
		perror("数据保存失败");
	}
	struct lianbiao *q2;
	struct lianbiao *q4 = &user_head->user;
	list_for(q2, q4)
	{
		users *pos = pos_address(q2, users, user);
		int ret_w = write(user_fd, pos, sizeof(users));
	}
	close(user_fd);

	message_fd = open("./.message", O_WRONLY|O_TRUNC);
	if(message_fd == -1);
	{
		perror("数据保存失败");
	}
	struct lianbiao *q5;
	struct lianbiao *q6 = &message_head->message;
	list_for(q5, q6)
	{
		messages *pos1 = pos_address(q5, messages, message);
		int ret_w1 = write(message_fd, pos1, sizeof(messages));
	}
	close(message_fd);

	printf("\033[2J\033[3;25H欢迎再次登录\n");
}



/*-----------------------------主函数入口------------------------------*/
int main(void)
{
	/*------------------------创建链表节点-------------------------*/
	user_head = (users*)malloc(sizeof(users));    //创建用户链表节点
	if(user_head == NULL) 
	{
		perror("user creat failse");
		return 0;
	}
	initalize_data(&user_head->user);             //初始化节点

	message_head = (messages*)malloc(sizeof(messages));  //创建信息链表节点
	if(message_head == NULL)
	{
		perror("message creat failse");
		return 0;
	}
	initalize_data(&message_head->message);             //初始化节点
	/*-----------------------成功创建链表节点----------------------*/



	/*----------------------判断与创建用户文件---------------------*/
	int ret_user = access("./.user",F_OK);            //判断用户文件是否存在

	if(ret_user == -1)                               //创建用户文件
	{
		user_fd = open("./.user",O_RDWR|O_CREAT,0666);
		if(user_fd == -1)
		{
			perror("user_file creat failse");
			return 0;
		}
		close(user_fd);
	}/*----------------------成功创建用户文件----------------------*/



    /*------------------打开用户文件将数据写入链表-----------------*/
	else
	{
		user_fd = open("./.user", O_RDONLY);
		while(1)
		{
			users *user_new = (users*)malloc(sizeof(users));
			if(user_new == NULL)
			{
				printf("初始化失败\n");
				return 0;
			}
			int a = read(user_fd, user_new, sizeof(users));
			static int i=0;
			if(a == 0) 
			{
				free(user_new);
				user_new = NULL;
				break;
			}
			initalize_data(&user_new->user);
			insert_data(&user_head->user, &user_new->user);
		}
		close(user_fd);
	}/*-------------------成功将用户文件数据写进链表-------------------*/
	

	/*----------------------判断与创建用户信息文件--------------------*/
	int ret_message = access("./.message", F_OK);   //判断信息文件是否存在
	
	if(ret_message == -1)                          //创建信息文件
	{
		message_fd = open("./.message", O_RDWR|O_CREAT, 0666);
		if(message_fd == -1)
		{
			perror("message_file creat failse");
			return 0;
		}
		close(message_fd);
	}/*------------------------成功创建用户信息文件--------------------*/

	

    /*------------------打开用户信息文件将数据写入链表-----------------*/
	else
	{
		message_fd = open("./.message", O_RDONLY);
		while(1)
		{
			messages *message_new = (messages*)malloc(sizeof(messages));
			if(message_new == NULL)
			{
				printf("初始化失败\n");
				return 0;
			}
			int a = read(message_fd, message_new, sizeof(messages));
			static int i=0;
			if(a == 0) 
			{
				free(message_new);
				message_new = NULL;
				break;
			}
			initalize_data(&message_new->message);
			insert_data(&message_head->message, &message_new->message);
		}
		close(message_fd);

	/*	
	struct lianbiao *q7;
	struct lianbiao *q8 = &message_head->message;
	list_for(q7, q8)
	{
		messages *pos1 = pos_address(q7, messages, message);
		printf("%s",pos1->name);
		printf("%s",pos1->QQ);
		printf("%s",pos1->sex);
		printf("%s",pos1->phone);
		printf("%s",pos1->age);
		printf("%s",pos1->address);
	}
		return 0;
	*/
	
	}/*-----------------成功将用户信息文件数据写进链表------------------*/



int z =0;
while(1)
{

	/*-----------------------出错处理------------------------*/
	if(z != 0 & z != 1)
	{
		printf("\033[2J\033[3;25H网络出错\n");
		printf("\033[5;20H请输入任意键返回.....");
		system("stty raw -echo");
		int c =getchar();
		system("stty cooked echo");
	}
	/*-------------------------------------------------------*/


//进入主界面
	z = zhujiemian();
	if(z == 3) break;    

	//进入登录界面
	if(z == 0) 
	{
		while(denglujiemian(user_head, message_head))   //登录成功则无返回值，否则返回真
		{
			char a;
			while(1)
			{
				printf("\033[4;24H重新登录(y/n)");
				scanf("%c", &a);
				while(getchar()!='\n');
				if(a == 'y'||a == 'n') break;
			}
			if(a == 'n') break;
		}
		continue;
	}


	//进入注册界面
	if(z == 1) 
	{
		/*----------创建一个新的用户结构体并初始化-------------*/
		users *user_new = (users*)malloc(sizeof(users));
		if(user_new == NULL) continue;
	//	initalize_data(&user_new->user);
	//	insert_data(&user_head->user, &user_new->user);
		/*-----------------------------------------------------*/

		//返回真则回主界面，返回假则回到注册界面
		while(zhucejiemian(user_head, user_new, message_head))
		{
			free(user_new);
			user_new = NULL;
			break;
		}
		continue;
	}
}//while(1)
	atexit(*fun1);
	exit;
}















/*
	char buf[100];
	char buf1[100];
	int ret = access("./use", F_OK);
	printf("%d\n", ret);
	if(ret == -1)
	{
		users* use_head = (users*)malloc(sizeof(users));
		if(use_head == NULL)
		{
			perror("error");
			return 0;
		}
		printf("\033[2J\033[1;1Huse name:\n");
		printf("\033[2;1Huse password:\n");
		printf("\033[2J\033[1;1Huse name:"); 
		fgets(buf,100,stdin);
		printf("\033[2;1Huse password:"); fgets(buf1, 100, stdin);
		strncpy(use_head->name, buf, 10);
		strncpy(use_head->password, buf1, 16);
		int fd = open("./use", O_RDWR|O_CREAT, 0777);
		if(fd == -1)
		{
			perror("fd error");
			return 0;
		}
		write(fd, use_head, sizeof(users));
		close(fd);
	}
	else
	{
		users* lao = (users*)malloc(sizeof(users));
		int fd = open("./use",O_RDWR);
		read(fd, lao, sizeof(users));
		close(fd);
		printf("%s",lao->name);
		printf("%s",lao->password);
	}
	return 0;
}
*/
