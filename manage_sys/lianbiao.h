#ifndef lianbiao_h
#define lianbiao_h
#include<stdio.h>
#include<stdbool.h>
#include<stdlib.h>

struct lianbiao    //创建链表节点
{
	struct lianbiao *last;
	struct lianbiao *next;
};

void initalize_data(struct lianbiao *head)     //初始化链表
{
	head->last = head->next = head;
}

void insert_data(struct lianbiao *head, struct lianbiao *_new)   //插入数据
{
	_new->last = head;
	_new->next = head->next;
	head->next->last = _new;
	head->next = _new;
}

void delete_data(struct lianbiao *list)      //删除数据
{
	list->next->last = list->last;
	list->last->next = list->next;
}

#define pos_address(lianbiao, type, member) \
((type *)((char *)(lianbiao)-(unsigned long)(&((type *)0)->member)))

#define list_for(pos, head) \
for (pos = (head)->next; pos != (head); \
pos = pos->next)    //用来循环列表


#endif
