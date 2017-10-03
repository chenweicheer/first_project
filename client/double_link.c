
#include "public.h"
#include "cmd.h"
#include "double_link.h"


static STU_NODE * make_node(STU_CMD * cmd)
{
	//printf(" make_node\n");
	STU_NODE * node =(STU_NODE *) malloc(sizeof(STU_NODE));
	if(NULL==node)
	{
		return NULL;
	}
	node->cmd=cmd;
	node->next=NULL;
	node->previous=NULL;
	node->timeout_count=0;
	return node;
}


void dlist_destroy_node(STU_NODE * node)
{
	if(node==NULL)
	{
		return ;
	}
	if(NULL!=node->cmd)
	{
		free(node->cmd);
	}
	free(node);
	printf("mem clear success\n\n");
}


void dlist_init(STU_DLIST * list,pthread_mutex_t *lock,pthread_cond_t *not_full_cond,pthread_cond_t *not_empty_cond)
{
    list->head = NULL;
    list->lock=lock;
	list->size=0;
    list->not_empty_cond=not_empty_cond;
    list->not_full_cond=not_full_cond;
}

int dlist_enqueue(STU_DLIST * list,STU_CMD *cmd)
{
	//printf("cmd dlist_enquue\n");
	/*//创建节点
	STU_NODE *node =make_node(cmd);
	if(NULL==node)
	{
		return -1;
	}
	
	//如果链表为空
	pthread_mutex_lock(list->lock);
	if(NULL==list->head)
	{
		list->head=node;
	}
	else
	{
		node->previous=list->head;
		list->head->next=node;
		list->head=node;
	}
	pthread_mutex_unlock(list->lock);
	return 0;*/
	dlist_add_tail(list,cmd);
}


STU_NODE * dlist_dequeue(STU_DLIST * list)
{
	
	STU_NODE * node = list->head;
	if(NULL==node)
	{
		//printf("dlist_dequeue head is null\n");
		return NULL;
	}
	
	//printf("cmd dlist_denquue\n");
	//只有一个节点
	pthread_mutex_lock(list->lock);
	if(NULL==node->previous)
	{

		list->head=NULL;
		//printf("head is null\n");
	}
	else
	{	//printf("head is not null\n");
		list->head=node->previous;
		node->previous=NULL;
		list->head->next=NULL;
	}
	list->size=list->size-1;
	pthread_mutex_unlock(list->lock);
	return node;
}



int d_insert(STU_DLIST * list, STU_CMD *cmd)
{
	STU_NODE * current = list->head;//当前指向头节点
	STU_NODE * node;//新节点指针

    while ( current->next != NULL) //找到插入位置
        current = current->next;//在不是尾节点结束时，当前指针的值要大于新值，所以插入在当前节点之前


    node = make_node(cmd);

    if ( current->next == NULL )//在链表的尾端插入和中间插入不同
    {
        current->next = node;
        node->previous = current;
    }
    else
    {
        node->previous = current->previous; //将新节点的前驱指向当前节点的前驱
        node->next = current;//将新节点的后继指向当前节点，所以这时一个前插方式

        current->previous->next = node;//将当前节点的前驱的后继指向新节点
        current->previous = node;//将当前节点的前驱指向新节点
    }

    return 1;
}


//只是指针的断开并没有释放内存
void dlist_remove(STU_DLIST * list, STU_NODE *node)
{	pthread_mutex_lock(list->lock);
	if(NULL==node->previous&&NULL==node->next)
	{
		list->head=NULL;
	}
	else if(NULL==node->previous&&NULL!=node->next)
	{
		node->next->previous=NULL;
		node->next=NULL;
	}
	else if(NULL!=node->previous&&NULL==node->next)
	{
		list->head=node->previous;
		node->previous->next=NULL;
		node->previous=NULL;
		
	}
	else
	{
		node->previous->next=node->next;
		node->next->previous=node->previous;
		node->next=node->previous=NULL;
	}
	list->size=list->size-1;
	pthread_mutex_unlock(list->lock);
	
}

int d_modify(STU_DLIST* list, const int key, const int data)//修改元素，因为是有序链表，所以要先删除后插入
{
}

STU_NODE* d_find(STU_DLIST * list, const int key)
{

}

//将节点添加链表尾部
void dlist_add_node_tail(STU_DLIST * list,STU_NODE*node)
{
	pthread_mutex_lock(list->lock);
	//printf("dlist_add_node_tail\n");
	STU_NODE *p = list->head;
	if(NULL==list->head)
	{
		//printf("dlist_add_node_tail  head is null\n");
		list->head=node;
	}
	else
	{
		//找到最后一个位置
		while(NULL!=p->previous)
		{
			p=p->previous;
		}
		p->previous=node;
		node->next=p;
		//printf("dlist_add_node_tail  head is not null\n");
	}
	pthread_mutex_unlock(list->lock);
}



void dlist_add_tail(STU_DLIST * list,STU_CMD*cmd)
{
	//printf("dlist_add_tail\n");
	STU_NODE *node =  make_node(cmd);
	if(NULL==node)
	{
		//printf("dlist_add_tail node is null\n");
		return;
	}
	dlist_add_node_tail(list,node);
}



void dlist_treaverse(STU_DLIST * list, void (*func)(void * ,void *),STU_CMD *cmd )
{
	//printf("dlist_traverse\n");
    STU_NODE * node = list->head;
	STU_NODE *tmp =NULL;
	while(NULL!=node)
	{
		//printf("dlist_treaverse not null\n");
		tmp =node->previous;
		func(node,cmd);
		node=tmp;
	}
}

void d_destroy(STU_DLIST * list)
{
   // treaverse_list(list->head, destroy_node); //遍历，销毁链表
    //list->head = NULL;
}


