/*
 * cmd_queue.h
 *
 *  Created on: 2017-9-12
 *      Author: Administrator
 */

#ifndef CMD_QUEUE_H_
#define CMD_QUEUE_H_
#include "public.h"
#include "cmd.h"
#include<pthread.h>

#define QUEUE_SIZE 	20

typedef struct
{
	STU_CMD cmd_queue[QUEUE_SIZE];
	int front;
	int rear;
	pthread_mutex_t *lock;
	pthread_cond_t *not_full_cond;
	pthread_cond_t *not_empty_cond;
}STU_QUEUE_CMD;

void init_queue(STU_QUEUE_CMD* queue,pthread_mutex_t *lock,pthread_cond_t *not_full_cond,pthread_cond_t *not_empty_cond);

int is_full(STU_QUEUE_CMD* queue);

int is_empty(STU_QUEUE_CMD* queue);

int enqueue(STU_QUEUE_CMD* queue,STU_CMD cmd);

STU_CMD* dequeue(STU_QUEUE_CMD* queue);



#endif /* CMD_QUEUE_H_ */
