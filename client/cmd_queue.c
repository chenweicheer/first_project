/*
 * cmd_queue.c
 *
 *  Created on: 2017-9-12
 *      Author: Administrator
 */

#include "cmd_queue.h"


/*
 * 队列初始化
 * */
void init_queue(STU_QUEUE_CMD* queue,pthread_mutex_t *lock,pthread_cond_t *not_full_cond,pthread_cond_t *not_empty_cond)
{
	queue->front=0;
	queue->rear=0;
	queue->lock=lock;
	queue->not_empty_cond=not_empty_cond;
	queue->not_full_cond=not_full_cond;
}

int is_full(STU_QUEUE_CMD* queue)
{
	return ((queue->rear+1)%QUEUE_SIZE)==(queue->front);
}
int is_empty(STU_QUEUE_CMD* queue)
{
	return  queue->front== queue->rear;
}

int enqueue(STU_QUEUE_CMD* queue,STU_CMD cmd)
{	
	//printf("enqueue  cmd :%x\n",cmd.cmd_value);
	pthread_mutex_lock(queue->lock);
	while(is_full(queue))
	{
		//printf("\t\t\t enqueue is full\n");
		pthread_cond_wait(queue->not_full_cond, queue->lock);  
	}
	queue->cmd_queue[queue->rear]=cmd;
	queue->rear=(queue->rear+1)%QUEUE_SIZE;

	pthread_cond_signal(queue->not_empty_cond);  
	pthread_mutex_unlock(queue->lock);
	return 1;
}

STU_CMD* dequeue(STU_QUEUE_CMD* queue)
{
	pthread_mutex_lock(queue->lock);

	
	 /** 
	    * 若队列空等待指定时间 
	    */  
	struct timeval now;
	struct timespec timepass;  
	gettimeofday(&now, NULL);  
	timepass.tv_sec = now.tv_sec + 10;	
	timepass.tv_nsec = 0;
	while(is_empty(queue))
	{
		//printf("\t\t\t enqueue is empty\n");
		//pthread_cond_timedwait(&g_not_empty_cond, &g_lock1, &timepass);
		pthread_cond_wait(queue->not_empty_cond, queue->lock);
	}
	int res=queue->front;
	queue->front=(queue->front+1)%QUEUE_SIZE;

	pthread_cond_signal(queue->not_full_cond);
	pthread_mutex_unlock(queue->lock);
	return &queue->cmd_queue[res];
}


