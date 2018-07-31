#ifndef __THREADPOLL_H__
#define __THREADPOLL_H__

#ifdef __cplusplus
extern "C"
{
#endif
#include <pthread.h>

typedef void * THREAD_POLL;

typedef void * (*TASK_NAME)(void *);

/* 任务相关描述 */

typedef struct
{
    TASK_NAME task_name;
    void *task_param;
}threadpoll_task_t;

/* 线程池管理相关 */

typedef struct
{
    int poll_state; //1,enable 0,disable

    /*describe the number of work thread*/
    int min_thread_num;  /* 最小线程数 */
    int max_thread_num;  /* 最大线程数 */
    int live_thread_num; /* 存活的线程数 */
    int busy_thread_num; /* 正在工作的线程数 */
    int wait_exit_thread_num; /* 需要退出的线程数 */

    /*describe the task queue*/
    int max_task_queue_size;  /* 任务队列大小 */
    int task_queue_size;     
    int task_queue_front;     /* 队头 */
    int task_queue_rear;      /* 队尾 */
	threadpoll_task_t * task_queue;   /* 任务相关队列 */
    pthread_cond_t task_queue_not_full; /* 条件变量，任务队列不为满 */
    pthread_cond_t task_queue_not_empty; /* 条件变量，任务队列不为空 */

    pthread_t * work_tids;   //set all work thread detached /* 存放线程的tid */
    pthread_attr_t thr_attr;  //detached

    pthread_t adjust_tid;//set adjust thread joinable

    pthread_mutex_t poll_lock;  /* 锁住整个结构体 */
	pthread_cond_t poll_not_enable;
	pthread_cond_t alive_thread_empty;

}threadpoll_t;


THREAD_POLL threadpoll_create(int min_thread_num, int max_thread_num, int max_task_queue_size);

int threadpoll_add_task(THREAD_POLL poll, TASK_NAME task_name, void *task_param);

int threadpoll_destroy(THREAD_POLL poll);

#ifdef __cplusplus
}
#endif

#endif
