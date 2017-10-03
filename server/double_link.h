/*
 * double_link.h
 *
 *  Created on: 2017-9-26
 *      Author: Administrator
 */

#ifndef DOUBLE_LINK_H_
#define DOUBLE_LINK_H_
#include <stdio.h>
#include <malloc.h>
#include<string.h>
#include "cmd.h"

typedef struct node
{
	STU_CMD *cmd;
	int timeout_count;
	struct node * previous;
	struct node * next;
}STU_NODE;

typedef struct dlist
{
    STU_NODE * head;
    pthread_mutex_t *lock;
    pthread_cond_t *not_full_cond;
    pthread_cond_t *not_empty_cond;
} STU_DLIST;

//入队列操作
int dlist_enqueue(STU_DLIST * list,STU_CMD *cmd);

//出队列操作
STU_NODE* dlist_dequeue(STU_DLIST * list);

//链表初始化
void dlist_init(STU_DLIST * list,pthread_mutex_t *lock,pthread_cond_t *not_full_cond,pthread_cond_t *not_empty_cond);
void dlist_treaverse(STU_DLIST * list, void (*func)(void *,void *),STU_CMD *cmd );
void dlist_remove(STU_DLIST * list, STU_NODE *node);
void dlist_add_node_tail(STU_DLIST * list,STU_NODE*node);
void dlist_destroy_node(STU_NODE * node);
void dlist_add_tail(STU_DLIST * list,STU_CMD*cmd);





#endif /* DOUBLE_LINK_H_ */
