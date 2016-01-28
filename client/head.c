#include "head.h"


/*---------------------------获取驱动信息-------------------------------*/
int get_driver_massage(struct VideoBuffer *p)
{
	int ret;
	printf("---------- the info of driver --------------\n");
    ret = ioctl(p->dev, VIDIOC_QUERYCAP, &p->cap); 
    if (ret < 0) {
        printf("VIDIOC_QUERYCAP failed (%d)\n", ret);
        return ret;
    }
    // Print capability infomations
    printf("Capability Informations:\n");
    printf(" driver: %s\n", p->cap.driver);
    printf(" card: %s\n", p->cap.card);
    printf(" bus_info: %s\n", p->cap.bus_info);
    printf(" version: %08X\n", p->cap.version);
    printf(" capabilities: %08X\n", p->cap.capabilities);
	return 0;
}

/*--------------------------显示所有支持的格式--------------------------*/
int show_any_support_format(struct VideoBuffer *p)
{
	printf("\n----------------get support format-------------\n");
	p->fmtdesc.index=0; 
	p->fmtdesc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	printf("Support format:\n");
	while(ioctl(p->dev, VIDIOC_ENUM_FMT, &p->fmtdesc) != -1)
	{
		printf("\t%d.%s\n",p->fmtdesc.index+1,p->fmtdesc.description);
		p->fmtdesc.index++;
	}
	return 0;
}

/*-----------------------------查看当前格式-----------------------------*/
int show_now_format(struct VideoBuffer *p)
{
	printf("\n----------------get current format-------------\n");
	p->fmt.type=V4L2_BUF_TYPE_VIDEO_CAPTURE; 
	ioctl(p->dev, VIDIOC_G_FMT, &p->fmt);
	printf("Current data format information:\n");
	printf("width:%d, height:%d\n",p->fmt.fmt.pix.width,p->fmt.fmt.pix.height);
	return 0;
}
	
/*-----------------------------设置视频格式-----------------------------*/
int set_video_format(struct VideoBuffer *p)
{
	printf("\n----------------set current format-------------\n");
    //struct v4l2_format fmt;
    memset(&p->fmt, 0, sizeof(p->fmt));
    p->fmt.type					= V4L2_BUF_TYPE_VIDEO_CAPTURE;
    p->fmt.fmt.pix.width			= VIDEO_WIDTH; //640
    p->fmt.fmt.pix.height			= VIDEO_HEIGHT; //480
    p->fmt.fmt.pix.pixelformat	= V4L2_PIX_FMT_JPEG;//YUYV格式
    p->fmt.fmt.pix.field			= V4L2_FIELD_INTERLACED;
    int ret = ioctl(p->dev, VIDIOC_S_FMT, &p->fmt);
    if (ret < 0) {
        printf("VIDIOC_S_FMT failed (%d)\n", ret);
        return ret;
    }
	return 0;
}

/*------------------------------获取视频格式-----------------------------*/
int get_video_format(struct VideoBuffer *p)
{
	int ret;
	printf("\n----------------get setted format-------------\n");
    ret = ioctl(p->dev, VIDIOC_G_FMT, &p->fmt);
    if (ret < 0) {
        printf("VIDIOC_G_FMT failed (%d)\n", ret);
        return ret;
    }
    // Print Stream Format
    printf("Stream Format Informations:\n");
    printf(" type: %d\n", p->fmt.type);
    printf(" width: %d\n", p->fmt.fmt.pix.width);
    printf(" height: %d\n", p->fmt.fmt.pix.height);
    char fmtstr[8];
    memset(fmtstr, 0, 8);
    memcpy(fmtstr, &p->fmt.fmt.pix.pixelformat, 4);
    printf(" pixelformat: %s\n", fmtstr);
    printf(" field: %d\n", p->fmt.fmt.pix.field);
    printf(" bytesperline: %d\n", p->fmt.fmt.pix.bytesperline);
    printf(" sizeimage: %d\n", p->fmt.fmt.pix.sizeimage);
    printf(" colorspace: %d\n", p->fmt.fmt.pix.colorspace);
	return 0;
}

/*-------------------------------请求分配内存----------------------------*/
int request_buffer(struct VideoBuffer *p)
{
	int ret;
	printf("----------------request buffer-------------\n");
    p->reqbuf.count = 1; //申请一个拥有1个缓冲帧的缓冲区

    p->reqbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
    p->reqbuf.memory = V4L2_MEMORY_MMAP;
    ret = ioctl(p->dev , VIDIOC_REQBUFS, &p->reqbuf);
    if(ret < 0) {
        printf("VIDIOC_REQBUFS failed (%d)\n", ret);
        return ret;
	}else{
		return 0;
	}
}

/*-------------------------------获取缓冲帧的信息------------------------*/
	int get_v4l2_buffer_info(struct VideoBuffer *p)
	{
		int ret;
		printf("----------- get the info of V4L2 buffer -------------\n");
		//VideoBuffer*  buffers = calloc( framebuf.reqbuf.count, sizeof(*buffers) );
		p->buf.index = 0;
		p->buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		p->buf.memory = V4L2_MEMORY_MMAP;
		ret = ioctl(p->dev , VIDIOC_QUERYBUF, &p->buf);
		if(ret < 0) {
			printf("VIDIOC_QUERYBUF (%d) failed (%d)\n", 0, ret);
			return ret;
		}
		return 0;
	}
	
/*-----------------------------映射地址到用户空间------------------------*/
	unsigned char *mapp_buffer(struct VideoBuffer *p)
	{
		p->length = p->buf.length; //缓冲帧的长度
		p->start = calloc(1,p->buf.length);
		if(p->start == NULL){
			printf("framebuf.start calloc failed!\n");
			return NULL;
		}
		p->start =  mmap(0, p->buf.length, PROT_READ|PROT_WRITE, MAP_SHARED, p->dev, p->buf.m.offset);

        if (p->start == MAP_FAILED) {
            printf("mmap (%d) failed: %s\n", 0, strerror(errno));
            return NULL;
        }
		unsigned char *sp = calloc(1, p->buf.length);
		if(sp == NULL)
		{
			printf("buffer calloc failed!\n");
			return NULL;
		}
		return sp;
	}

