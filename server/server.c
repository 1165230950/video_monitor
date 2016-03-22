#include <stdio.h>

#include <stdlib.h>
//open
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <string.h>
//write read
#include <unistd.h>

//socket
//#include <sys/types.h>
#include <sys/socket.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>
//#include <pthread.h>
#include <semaphore.h>
#include "threadpool.h"
#include "head.h"

#include <signal.h>
#include <time.h>

int stop;

int fd;

pthread_mutex_t mutex[11];
pthread_cond_t	update;

FILE *fp;
unsigned char	*(framebuf[11]);		//给每个线程分配一个缓冲
int				thread_fd[12];			//用来存储线程自己的套接子
int				length;
char *(file[100]);					//用来存储图片文件名

void usage(const char *pro)
{
	fprintf(stderr, "Usage: %s <IP> <PORT>\n", pro);
	exit(1);
}

void func(int arg)
{
	time_t sed = time(0);
	char date[100]="ffmpeg -f image2 -i %d.jpg ";
	strftime(date+strlen(date),sizeof(date),"%Y-%m-%d.avi",localtime(&sed));
	system(date);
	system("rm *.jpg");
	exit(0);
}


void *client_thread(void *arg)
{
	
	/*-------------找出空的framebuf记下标记，并分配内存------------*/
	
	int i,sign;
	for(i=0; i<10; i++)
	{
		if(framebuf[i] == NULL){
			sign = i;
		}
	}
	framebuf[sign] = malloc(118784);
	thread_fd[sign]= fd;

	/*-------------------------开始工作--------------------------*/

	/*-------------------接收浏览器发来的请求--------------------*/
	printf("client start work ......\n");
	char buffer[1000];
	bzero(buffer, 1000);
	recv(fd, buffer, 999, 0);	
	/*----------------------解析浏览器请求-------------------*/

	if(strstr(buffer, "GET /?action=snapshot") != NULL){
		
		pthread_mutex_lock(&mutex[0]);
		pthread_cond_wait(&update, &mutex[0]);

		/*---------------------发送静态图片-------------------*/
		bzero(buffer, 1000);
		sprintf(buffer, "HTTP/1.0 200 OK\r\n" \
						STD_HEADER \
						"Content-type: image/jpeg\r\n" \
						"\r\n");
		if( write(thread_fd[sign], buffer, strlen(buffer)) < 0 ) {
			printf("write buffer faile!\n");
			close(thread_fd[sign]);
			return NULL;
		}

		if(write(thread_fd[sign], framebuf[sign], length) < 0){
			printf("write framebuf failed!\n");
			close(thread_fd[sign]);
			return NULL;
		}
	}else if(strstr(buffer, "GET /?action=stream") != NULL){
		/*---------------------发送动态图片-------------------*/
			
		memset(buffer, 0, 100);
		sprintf(buffer, "HTTP/1.0 200 OK\r\n" \
						STD_HEADER \
						"Content-Type: multipart/x-mixed-replace;boundary="\
						 BOUNDARY "\r\n" \
						 "\r\n" \
						 "--" BOUNDARY "\r\n");
	
	
		if( write(thread_fd[sign], buffer, strlen(buffer)) < 0 ) {
			printf("write faile!\n");
			return NULL;
		}
	
		while(1)
		{
			/*-------------测试代码--将接收到的图片写入文件--------------*/
			pthread_mutex_lock(&mutex[0]);
			pthread_cond_wait(&update, &mutex[0]);
	
	/*
			j++;
			file[j] = malloc(15);
			memset(file[j],0,15);
			sprintf(file[j],"%d.jpg",j);
		
			fp = fopen(file[j],"w+");
			free(file[j]);
			file[j] = NULL;
		
			
			fwrite(framebuf[sign],1,length,fp);
			fclose(fp);
			printf("222222222222222222222222222222222222222222\n");
	*/		
			
	
			//开始循环发送图片
			sprintf(buffer, "Content-Type: image/jpeg\r\n" \
							"Content-Length: %d\r\n" \
							"\r\n", length);
			if(write(thread_fd[sign], buffer, strlen(buffer)) <= 0 ) break;
	
			if(write(thread_fd[sign], framebuf[sign], length) < 0 ) break;
	
			sprintf(buffer, "\r\n--" BOUNDARY "\r\n");
	
			if(write(thread_fd[sign], buffer, strlen(buffer)) < 0 ) break;
			pthread_mutex_unlock(&mutex[0]);
		}//end while(1)!
	}else{
		write(thread_fd[sign], "request error\r\n", 20);
	}
	pthread_mutex_unlock(&mutex[0]);
	shutdown(thread_fd[sign], 0);
	free(framebuf[sign]);
	framebuf[sign] = NULL;
	printf("client thread exit!\n");
}


void *cam_thread(void *arg, int *fd1)
{

	static int j = 0;
	unsigned char *recv_buf = malloc(118784);		//用来接收图片数据
	thread_fd[11] = fd;

	/*--------------------开始工作----------------------*/

	int ret, data_sum ,recv_data, i;
	char *size = malloc(11);		//用来接收图片的大小
	while(1)
	{
error:
		ret = send(thread_fd[11],START,5,0);	//请求客户端开始发送数据
		if(ret == 0 || ret == -1)
		{
			close(thread_fd[11]);
			free(size);
			free(recv_buf);
			stop = 1;
			pthread_exit(NULL);
		}
		memset(size,0,11);
		ret = recv(thread_fd[11], size, 10,0);	//接收client要发来图片数据的大小
		if(ret == -1 || ret == 0)
		{
			close(thread_fd[11]);
			free(size);
			free(recv_buf);
			stop = 1;
			pthread_exit(NULL);
		}
		data_sum = atoi(size);
//		printf("图片数据大小为：%d\n",data_sum);
		if(data_sum <= 0)
		{
			ret = send(thread_fd[11],ERROR,10,0);
			if(ret == 0 || ret == -1)
			{
				close(thread_fd[11]);
				free(size);
				free(recv_buf);
				stop = 1;
				pthread_exit(NULL);
			}
			goto error;
		}else{
			ret = send(thread_fd[11],size,10,0);		//发送回应
			if(ret == -1 || ret == 0)
			{
				close(thread_fd[11]);
				free(size);
				free(recv_buf);
				stop = 1;
				pthread_exit(NULL);
			}
		}
		/*-----------------------获取线程锁----------------------*/
		for(i=0; i<10; i++)
		{
			pthread_mutex_lock(&mutex[i]);
		}
		/*----------------------开始接收数据---------------------*/
		recv_data = 0;
		memset(recv_buf,0,118784);
//		printf("开始接收数据\n");
		for(i=0; i<120; i++)
		{
			ret = recv(thread_fd[11], (unsigned char *)((int)recv_buf+recv_data), 1024,0);		//接收数据
			if(ret == 0 || ret == -1)
			{
				printf("recv data %d failed\n",i);
				close(thread_fd[11]);
				free(size);
				free(recv_buf);
				stop = 1;
				pthread_exit(NULL);
			}
			recv_data += ret;
			if(recv_data >= data_sum)		//判断数据是否接收完成
			{
				ret = send(thread_fd[11],END,10,0);	//通知客户端数据接收完成
				if(ret == 0 || ret == -1)
				{
					printf("send end failed!\n");
					close(thread_fd[11]);
					free(size);
					free(recv_buf);
					stop = 1;
					pthread_exit(NULL);
				}else{
					//将数据拷贝到全局缓冲
					for(i=0; i<10; i++)
					{
						if(framebuf[i] != NULL)
						{
							memset(framebuf[i],0,118784);
							memcpy(framebuf[i], recv_buf, recv_data);
						}
					}
					length = recv_data;
	


			j++;
			file[j] = malloc(15);
			memset(file[j],0,15);
			sprintf(file[j],"%d.jpg",j);
		
			fp = fopen(file[j],"w+");
			free(file[j]);
			file[j] = NULL;
		
			
			fwrite(recv_buf,1,length,fp);
			fclose(fp);
	


					//通知所有client函数并释放互斥锁
					pthread_cond_broadcast(&update);
					for(i=0; i<10; i++)
					{
						pthread_mutex_unlock(&mutex[i]);
					}
//					printf("接收完成\n\n");
					goto error;
				}
			}else{
				ret = send(thread_fd[11],OK,10,0);	//发送回应
				if(ret == -1 || ret == 0)
				{
					close(thread_fd[11]);
					free(size);
					free(recv_buf);
					stop = 1;
					pthread_exit(NULL);
				}
			}
		}//end for(i=0; i<120; i++)

	}//end while(1)
}

int main(int argc, char const *argv[])
{
	signal(SIGPIPE, SIG_IGN);	//用来屏蔽浏览器断开时内核发来的SIGPIPE信号
								//此信号会结束进程
	signal(SIGINT, func);

	int i;
	/*---------------------检测是否输入IP和PORT--------------------*/
	if(argc != 2)
	{
		usage(argv[0]);
	}

	/*-----------------------创建线程池并初始化--------------------*/
	
	struct threadpool *pool		= threadpool_init(11,15);

	/*------------------------初始化全局线程锁---------------------*/	
	for(i=0; i<10; i++)
	{
		pthread_mutex_init(&mutex[i], NULL);
	}
	pthread_cond_init(&update, NULL);

	/*-----------------------初始化全局数据缓存--------------------*/
	for(i=0; i<10; i++)
	{
		framebuf[i] = NULL;
	}

	/*--------------------------开启服务器-------------------------*/

	int fd1;
	// 1，创建TCP套接字
	fd1 = socket(AF_INET, SOCK_STREAM, 0);
	if(fd1 == -1)
	{
		printf("socket failed!\n");
		return 0;
	}
	int on = 1;
	if(setsockopt(fd1, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0){
		perror("setsockopt failed");
		return 0;
	}

	// 2，绑定 IP+PORT
	struct sockaddr_in srvaddr;
	socklen_t len = sizeof(srvaddr);
	bzero(&srvaddr, len);

	srvaddr.sin_family = AF_INET;
	srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	srvaddr.sin_port = htons(atoi(argv[1]));

	if(bind(fd1, (struct sockaddr *)&srvaddr, len) == -1)
	{
		printf("bind failed!\n");
		return 0;
	}

	//3 将套接子设置为监听状态，并且设置同时接受连接最大值数量
	if(listen(fd1, 7) == -1)
	{
		printf("listen failed!\n");
		return 0;
	}

	// 4，坐等对方的连接请求
	struct sockaddr_in cliaddr;
	len = sizeof(cliaddr);
	bzero(&cliaddr, len);

	stop = 0;
	pthread_t std;
	while(!stop)
	{
		fd = accept(fd1, (struct sockaddr *)&cliaddr, &len);
		if(fd == -1)
		{
			printf("accept failed!\n");
			return 0;
		}
		char peer_addr[16];
		printf("new connection[%s:%hu]\n",
				inet_ntop(AF_INET, &cliaddr.sin_addr, peer_addr,len),
				ntohs(cliaddr.sin_port));
		
	/*--------------------------开始工作------------------------------*/
		static int sign = 0;
		int ret;
		if(sign == 0)		//先让摄像头开始工作
		{
			sign = 1;
			char *buf = malloc(10);
			bzero(buf,10);
			ret = recv(fd,buf,10,0);
			if(ret == -1 || ret == 0)
			{
				printf("recv cmd failed!\n");
				close (fd);
			}else{
				if(strcmp(buf,CMD) != 0)
				{
					printf("cmd error!\n");
					close(fd);
				}else{
					printf("cmd crrect\n");
					pthread_create(&std, NULL, (void *)cam_thread, NULL);
				}
			}
		}else{
			threadpool_add_job(pool, client_thread, NULL);
		}
	}
	printf("while(!stop) exit ..............\n");
/*-----------------------销毁线程池退出进程-------------------------*/

	pthread_join(std, NULL);	//等待cam_thread退出
	printf("exit...................................\n");

	if(threadpool_destroy(pool) == -1)
	{
		printf("pool destroy failed: pool has exit!\n");
	}else{
		printf("pool destroy success!\n");
	}

	return 0;

}
