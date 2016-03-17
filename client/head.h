#ifndef head_h
#define	head_h


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>            
#include <errno.h>
#include <malloc.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>

#define CAMERA_DEVICE "/dev/video3"
#define VIDEO_WIDTH 640
#define VIDEO_HEIGHT 480
#define VIDEO_FORMAT V4L2_PIX_FMT_YUYV
#define CMD "laohuhdj"
#define START "start"
#define OK	"ok"

typedef struct VideoBuffer {
	int							dev;
	struct v4l2_capability		cap;
	struct v4l2_fmtdesc			fmtdesc; 
	struct v4l2_format			fmt;
    struct v4l2_requestbuffers	reqbuf;
    struct v4l2_buffer			buf;
    void						*start;  
    size_t						length; 
} VideoBuffer; 

int get_driver_massage(struct VideoBuffer *p);

int show_any_support_format(struct VideoBuffer *p);

int show_now_format(struct VideoBuffer *p);

int set_video_format(struct VideoBuffer *p);

int get_video_format(struct VideoBuffer *p);

int request_buffer(struct VideoBuffer *p);

int get_v4l2_buffer_info(struct VideoBuffer *p);

unsigned char *mapp_buffer(struct VideoBuffer *p);
 
#endif
