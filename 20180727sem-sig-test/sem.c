

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "sem.h"
// #include "random.h"
#define __BIG_DEBUG__

#ifdef __BIG_DEBUG__
#define debug_printf(fmt, ...)  \
    do { \
        printf("[Trace]: "); \
        printf(fmt, ##__VA_ARGS__); \
        printf("%s", "\r\n"); \
    } while(0)
#else
#define debug_printf(fmt, ...)
#endif

#if 0
sem_t * sem_open(const char *name, int oflag, mode_t mode, unsigned int value);
int sem_close(sem_t *sem);
int sem_unlink(const char *name);

int sem_init(sem_t *sem, int pshared, unsigned int value);
int sem_wait(sem_t *sem);
int sem_post(sem_t *sem);
int sem_destroy(sem_t *sem);
#endif

/*
* 参数名：sem_node
		  pshared 0   只在线程见共享
				非0 进程间共享
		  value       信号量初始值
  return 0  success
*/

int sem_node_init(sem_node_t *sem_node, int pshared, unsigned int value)
{
	int ret = -1;
	sem_node->sem = NULL;
	srand((int)time(0));
	memset(sem_node->sem_name, 0, sizeof(sem_node->sem_name));
	
    sprintf(sem_node->sem_name, "%u", random());
    // sprintf(sem_node->sem_name, "%u", random_uint());
	debug_printf("The sem_node->name is %s",sem_node->sem_name);
	
	#ifdef __APPLE__
	sem_node->sem = sem_open(sem_node->sem_name, O_CREAT|O_EXCL, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH, value);
	if (sem_node->sem != NULL)
	{
		ret = 0;
	}
	#else
	sem_node->sem = (sem_t *)malloc(sizeof(sem_t));
	ret = sem_init(sem_node->sem, pshared, value);
	#endif
	return ret;
}

int sem_node_destroy(sem_node_t *sem_node)
{
	int ret = -1;
	#ifdef __APPLE__
	if (sem_close(sem_node->sem) == 0 && sem_unlink(sem_node->sem_name) == 0)
	{
		ret = 0;
	}
	#else
	ret = sem_destroy(sem_node->sem);
	if (sem_node->sem != NULL)
	{
		free(sem_node->sem);
		sem_node->sem = NULL;
	}
	#endif
	return ret;
}

int sem_node_wait(sem_node_t *sem_node)
{
	return sem_wait(sem_node->sem);
}

int sem_node_post(sem_node_t *sem_node)
{
	return sem_post(sem_node->sem);
}

