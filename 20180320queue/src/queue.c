/**************************************************************************************************
 Filename:       queue.c
 Author:         Jiankai Li
 Revised:        $Date: 2018-03-20 13:23:33 -0700 
 Revision:       简单的用链表实现循环队列的例子

 Description:    This is a file 
 **************************************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"

void QUEUE_INIT (msgqueue_t *queue, uint32_t size) {
	// queue = (msgqueue_t *)malloc(sizeof(msgqueue_t));
	queue->node = (Elem *)malloc(sizeof(Elem));
	if (queue) {
		queue->qc = (queue_core *)malloc(sizeof(queue_core));
		if (queue->qc) {
			pthread_mutex_init(&(queue->qc->mutex),NULL);
			pthread_cond_init(&(queue->qc->cond),NULL);
			queue->qc->size  = size;
			queue->rear = queue->front = NULL;
			queue->count = 0;
		}
	}	
}

void QUEUE_PUSH(msgqueue_t *queue,Elem *node) {
	msgqueue_t *q = queue;
	queue_core *qc = queue->qc;
	Elem *temp;
	pthread_mutex_lock(&(queue->qc->mutex));  
	if( q->front == NULL) {  // 第一个数据
		q->node->data = (char *)malloc(strlen(node->data));
		memcpy(q->node->data,node->data,strlen(node->data));
		q->front = q->backrear = q->node;
		q->rear  = q->node->next;
		q->count++;
	} else {           // 非第一个数据
		if(q->count < q->qc->size) {
			temp = q->backrear->next = (Elem *)malloc(sizeof(Elem));
			(temp)->data = (char *)malloc(strlen(node->data));
			memcpy(temp->data,node->data,strlen(node->data));
			q->backrear = q->backrear->next;
			q->rear = 	q->backrear->next;
			q->count++;
		} else {       // 队列已满，仍要插入数据
			if (q->front != NULL) { //  头部有数据
				// node = malloc(sizeof(Elem));
				temp = q->front->next;
				memset(q->front->data,0,strlen(q->front->data));
				free(q->front->data);
				free(q->front);
				q->front = temp;
				q->count--;
				temp = q->backrear->next = (Elem *)malloc(sizeof(Elem));
				(temp)->data = (char *)malloc(strlen(node->data));
				memcpy(temp->data,node->data,strlen(node->data));
				q->backrear = q->backrear->next;
				q->rear = 	q->backrear->next;
				q->count++;
			}
			printf("\n[ERR] the queue is large than %d items",q->qc->size);
		}
	}
    pthread_mutex_unlock(&queue->qc->mutex);                                      \
    pthread_cond_signal(&queue->qc->cond);  
}


void QUEUE_POP(msgqueue_t *queue,Elem *node) {
	msgqueue_t *q = queue;
	queue_core *qc = q->qc;
	Elem *temp;
	pthread_mutex_lock(&qc->mutex);
	while ( QUEUE_SIZE(q) == 0) {
		pthread_cond_wait(&qc->cond, &qc->mutex);
	}
	if (q->front != NULL) { //  头部有数据
		// node = malloc(sizeof(Elem));
		node->data = malloc(strlen(q->front->data));
		memcpy(node->data,q->front->data,strlen(q->front->data));
		temp = q->front->next;
		memset(q->front->data,0,strlen(q->front->data));
		free(q->front->data);
		free(q->front);
		q->front = temp;
		q->count--;
	}
	pthread_mutex_unlock(&queue->qc->mutex);  
}

uint32_t QUEUE_SIZE(msgqueue_t *queue) {
	return (queue->count);
}

void QUEUE_LOCK(msgqueue_t *queue) {
	pthread_mutex_lock(&queue->qc->mutex);
}

void QUEUE_UNLOCK(msgqueue_t *queue) {
	pthread_mutex_unlock(&queue->qc->mutex);
}
