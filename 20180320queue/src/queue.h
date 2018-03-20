/**************************************************************************************************
 Filename:       queue.c
 Author:         Jiankai Li
 Revised:        $Date: 2018-03-20 13:23:33 -0700 
 Revision:       简单的用链表实现循环队列的例子

 Description:    This is a file 
 **************************************************************************************************/

#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdint.h>

struct snode {
	char *data;
	void *next;
};
typedef struct snode Elem;

typedef struct {
    pthread_mutex_t mutex;   // 互斥锁
    pthread_cond_t cond;     // 信号量
    unsigned int size;       // 大小
} queue_core;                // 队列核心


struct sQueue {
	Elem *node;
	queue_core *qc;
	Elem *rear;
	Elem *front;
	Elem *backrear;
	uint32_t count;
};

typedef struct sQueue msgqueue_t;

void QUEUE_INIT (msgqueue_t *queue, uint32_t size);
void QUEUE_PUSH(msgqueue_t *queue,Elem *node);
void QUEUE_POP(msgqueue_t *queue,Elem *node);
uint32_t QUEUE_SIZE(msgqueue_t *queue);
void QUEUE_LOCK(msgqueue_t *queue);
void QUEUE_UNLOCK(msgqueue_t *queue);  


// typedef struct {
    // pthread_mutex_t mutex;   // 互斥锁
    // pthread_cond_t cond;     // 信号量
    // void* front;
    // void* back;
    // void* backqh;
    // unsigned int size;       // 大小
// } queue_core;                // 队列核心

// typedef struct queue_handle {
    // queue_core* qc;
    // void* next;
// } queue_handle;              // 队列操作手柄

// /* queue type */
// struct msg {
    // char *content;
    // queue_handle qh;
// };

// typedef struct msg msgqueue_t;


// 队列初始化 队列的具体数据类型可以以外部定义，前提是数据类型包含queue_handle qh
// #define QUEUE_INIT(qt, q)                                                      \
    // do {                                                                       \
        // queue_core* qc;                                                        \
        // (q) = malloc(sizeof (qt));                                             \
        // if (q) {                                                               \
            // (q)->qh.qc = malloc(sizeof (queue_core));                          \
            // if ((q)->qh.qc) {                                                  \
                // qc = (q)->qh.qc;                                               \
                // pthread_mutex_init(&qc->mutex, NULL);                          \
                // pthread_cond_init(&qc->cond, NULL);                            \
                // qc->front = qc->back = NULL;                                   \
                // qc->backqh = NULL;                                             \
                // qc->size = 0;                                                  \
                // (q)->qh.next = NULL;                                           \
            // } else {                                                           \
                // free(q);                                                       \
                // (q) = NULL;                                                    \
            // }                                                                  \
        // }                                                                      \
    // } while (false)

// #define QUEUE_PUSH(q, e)                                                       \
    // do {                                                                       \
		// msgqueue_t *msg = q;                                                        \
		// msg->content = (char*)malloc(strlen(e->content));											\
        // queue_core* qc;                                                        \
        // queue_handle* backqh;                                                  \
        // qc = (q)->qh.qc;                                                       \
        // pthread_mutex_lock(&qc->mutex);                                        \
        // (e)->qh.qc = qc;                                                       \
        // (e)->qh.next = NULL;                                                   \
        // backqh = qc->backqh;                                                   \
        // if (!qc->front) {                                                      \
            // qc->front = qc->back = (e);                                        \
        // } else {                                                               \
            // backqh->next = (e);                                                \
            // qc->back = (e);                                                    \
        // }                                                                      \
        // backqh = &((e)->qh);                                                   \
        // qc->backqh = backqh;                                                   \
        // qc->size++;                                                            \
        // pthread_mutex_unlock(&qc->mutex);                                      \
        // pthread_cond_signal(&qc->cond);                                        \
    // } while (false)

// #define QUEUE_POP(q, e)                                                        \
    // do {                                                                       \
		// msgqueue_t *msg = q;    											   \
        // queue_core* qc;                                                        \
        // (e) = NULL;                                                            \
        // qc = (q)->qh.qc;                                                       \
        // pthread_mutex_lock(&qc->mutex);                                        \
        // while (QUEUE_SIZE(q) == 0) {                                           \
            // pthread_cond_wait(&qc->cond, &qc->mutex);                          \
        // }                                                                      \
        // if ((q)->qh.qc->front != NULL) {                                       \
            // (e) = (q)->qh.qc->front;                                           \
            // (q)->qh.qc->front = (e)->qh.next;                                  \
            // (q)->qh.qc->size--;                                                \
        // }                                                                      \
		// memset(q->content)\
        // pthread_mutex_unlock(&qc->mutex);                                      \
    // } while (false)

// #define QUEUE_LOCK(q)                                                          \
    // pthread_mutex_lock(&q->qh.qc->mutex);

// #define QUEUE_SIZE(q)                                                          \
    // ((q)->qh.qc->size)

// #define QUEUE_UNLOCK(q)                                                        \
    // pthread_mutex_unlock(&q->qh.qc->mutex);

// #define QUEUE_FREE(q)                                                          \
    // do {                                                                       \
        // queue_core* qc;                                                        \
        // if ((q) && (q)->qh.qc) {                                               \
            // qc = (q)->qh.qc;                                                   \
            // pthread_cond_destroy(&qc->cond);                                   \
            // pthread_mutex_destroy(&qc->mutex);                                 \
            // free(qc);                                                          \
            // free(q);                                                           \
        // }                                                                      \
    // } while (false)

#endif /* QUEUE_H */
