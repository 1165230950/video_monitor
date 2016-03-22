#include "head.h"


int fd;			//socket套接字
sem_t sem_r;
sem_t sem_w;

unsigned char *buffer;
int buf_size;

VideoBuffer framebuf;   

void *send_data(void *arg)
{
	char *ask = (char *)malloc(sizeof(char)*10);	//用来接收回应
	char *size = (char *)malloc(sizeof(char)*10);	//用来发送数据长度

	int data_sum, ret, i;
	ret = send(fd,CMD,10,0);	//发送口令
	if(ret == -1 || ret == 0)
	{	
		printf("send cmd failed!\n");
		close(fd);
		sem_post(&sem_r);
		return NULL;
	}
	
	while(1)
	{
error:
		sem_wait(&sem_w);
		sprintf(size,"%d",buf_size);
		printf("data_size=%s\n",size);	//打印一下数据大小
		memset(ask, 0, 10);
		ret = recv(fd,ask,9,0);		//等待服务器请求发送数据
		if(ret == -1 || ret == 0)
		{
			printf("recv request failed!\n");
			close(fd);
			free(ask);
			free(size);
			sem_post(&sem_r);
			pthread_exit(NULL);
		}
		if(strcmp(ask,START) != 0)
		{
			printf("request error!\n");
			close(fd);
			free(ask);
			free(size);
			sem_post(&sem_r);
			pthread_exit(NULL);
		}
		ret = send(fd,size,strlen(size),0);	//发送即将发送的数据长度
		if(ret == -1 || ret == 0)
		{
			printf("send data length failed!\n");
			close(fd);
			free(ask);
			free(size);
			sem_post(&sem_r);
			pthread_exit(NULL);
		}
		memset(ask, 0, 10);
		ret = recv(fd,ask,10,0);		//服务器响应
		if(ret == -1 || ret == 0)
		{
			printf("recv ask failed!\n");
			close(fd);
			free(ask);
			free(size);
			sem_post(&sem_r);
			pthread_exit(NULL);
		}
		if(strcmp(ask,size) != 0){
			sem_post(&sem_r);
			goto error;
		}							
		/*-------------------开始发送图片数据---------------------*/
		data_sum = 0;
		printf("start send ...\n");
		for(i=0; i<116; i++)		//设置发送的图片数据最大值为116k
		{
			if(buf_size - data_sum < 1024)	//发送最后一串
			{
				ret = send(fd,(char *)((int)(buffer)+data_sum),buf_size-data_sum,0);
				if(ret == -1 || ret == 0)
				{
					printf("send data %d failed!\n",i);
					close(fd);
					free(ask);
					free(size);
					sem_post(&sem_r);
					pthread_exit(NULL);
				}
				data_sum += ret;
				memset(ask, 0, 10);
				ret = recv(fd,ask,10,0);		//服务器响应
				if(ret == -1 || ret == 0)
				{
					printf("recv data ask %d failed!\n",i);
					close(fd);
					free(ask);
					free(size);
					sem_post(&sem_r);
					pthread_exit(NULL);
				}
				if(strcmp(ask,OK) != 0){
					printf("send ok\n");
					sem_post(&sem_r);
					goto error;
				}							
			}else{
				ret = send(fd,(char *)((int)(buffer) + data_sum),1024,0);
				if(ret == -1 || ret == 0)
				{
					printf("send data %d failed!\n",i);
					close(fd);
					free(ask);
					free(size);
					sem_post(&sem_r);
					pthread_exit(NULL);
				}
				data_sum += ret;
				memset(ask, 0, 10);
				ret = recv(fd,ask,10,0);		//服务器响应
				if(ret == -1 || ret == 0)
				{
					printf("recv data ask %d failed!\n",i);
					close(fd);
					free(ask);
					free(size);
					sem_post(&sem_r);
					pthread_exit(NULL);
				}
				if(strcmp(ask,OK) != 0){
					sem_post(&sem_r);
					goto error;
				}							
			}
		}//end for()!
	}//end while()!
}//end send_data()!




/*-----------------------------进入主函数----------------------------*/
int main(int argc, char *argv[])
{
	/*-----------------------打开视频设备-------------------------*/

    framebuf.dev = open(CAMERA_DEVICE, O_RDWR);
    if (framebuf.dev < 0) {
        printf("Open %s failed\n", CAMERA_DEVICE);
        return -1;
    }

	/*----------------------获取驱动信息--------------------------*/

	int ret;
	ret = get_driver_massage(&framebuf);

	/*-------------------显示所有支持的格式-----------------------*/

	ret = show_any_support_format(&framebuf);

	/*---------------------查看当前格式---------------------------*/

	ret = show_now_format(&framebuf);

	/*---------------------设置视频格式---------------------------*/
	
	ret = set_video_format(&framebuf);

	/*---------------------获取视频格式---------------------------*/

	ret = get_video_format(&framebuf);

	/*---------------------请求分配内存---------------------------*/

	ret = request_buffer(&framebuf);

	/*-------------------获取缓冲帧的信息-------------------------*/
	
	ret = get_v4l2_buffer_info(&framebuf);
	
	/*------------------映射地址到用户空间------------------------*/
	buffer = mapp_buffer(&framebuf);


	/*----------------------申请信号量----------------------------*/

	ret = sem_init(&sem_r, 1, 1);
	if(ret != 0){
		printf("sem_r applay failed!\n");
		return 0;
	}
	ret = sem_init(&sem_w, 1, 0);
	if(ret != 0){
		printf("sem_w applay failed!\n");
		return 0;
	}

	/*--------------------把缓冲帧放入缓冲队列--------------------*/
	
	ret = ioctl(framebuf.dev , VIDIOC_QBUF, &framebuf.buf);//把缓冲帧放入缓冲队列
	if (ret < 0) {
		printf("VIDIOC_QBUF (%d) failed (%d)\n", 0, ret);
		return -1;
	}
	printf("Frame buffer %d: address=0x%x, length=%d\n", 0, (unsigned int)framebuf.start, framebuf.length);

	/*--------------------------开始录制--------------------------*/
	printf("----------- get the info of V4L2 buffer -------------\n");
	
    enum v4l2_buf_type type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    ret = ioctl(framebuf.dev, VIDIOC_STREAMON, &type); //开始视频采集
    if (ret < 0) {
        printf("VIDIOC_STREAMON failed (%d)\n", ret);
        return -1;
    }
	
/*--------------------------创建socket连接服务器-------------------------*/
	// 1，创建TCP套接字
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if(fd == -1)
	{
		printf("socket failed!\n");
		return -1;
	}

	// 2，指定服务器的IP + PORT
	struct sockaddr_in srvaddr;
	socklen_t len = sizeof(srvaddr);
	bzero(&srvaddr, len);

	srvaddr.sin_family = AF_INET;
	inet_pton(AF_INET, argv[1], &srvaddr.sin_addr);
	srvaddr.sin_port = htons(atoi(argv[2]));

	if(connect(fd, (struct sockaddr *)&srvaddr, len) == -1)
	{
		printf("connect failed\n");
		return -1;
	}		
	
	
	
/*----------------------------创建线程-------------------------------*/

	pthread_t std1;

	if(pthread_create(&std1,NULL,send_data,NULL) != 0)		//创建线程发送图片数据到服务器
	{
		printf("std1 create failed!\n");
		return 0;
	}


	while(1)
	{
		//获取资源

		sem_wait(&sem_r);
	/*------------------------从队列取出数据-------------------------*/	
		ret = ioctl(framebuf.dev, VIDIOC_DQBUF, &framebuf.buf); //从队列取出数据
		if(ret < 0){
			printf("VIDIOC_DQBUF failed (%d)\n", ret);
			return 0;
		}
		
		memset(buffer,0,framebuf.length);
		memcpy(buffer, framebuf.start, framebuf.buf.bytesused);

		buf_size = framebuf.buf.bytesused;
		//释放资源
		sem_post(&sem_w);
		
		/*-------------------把缓冲帧重新插入缓冲队列--------------------*/
	
		ret = ioctl(framebuf.dev, VIDIOC_QBUF, &framebuf.buf);//把缓冲帧重新插入缓冲队列
		if(ret < 0){
			printf("VIDIOC_QBUF failed (%d)\n", ret);
			pthread_exit(NULL);
		}
	}//end while!


	


/*-----------------------------函数退出-------------------------------*/

	pthread_join(std1,NULL);	//回收线程
	sem_destroy(&sem_r);
	sem_destroy(&sem_w);
	
	munmap(framebuf.start,framebuf.length);
	//关闭设备
    close(framebuf.dev);
    printf("Camera test Done.\n");
    return 0;
}
