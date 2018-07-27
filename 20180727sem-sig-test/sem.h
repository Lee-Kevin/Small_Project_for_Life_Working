#ifndef __SEM_H__
#define __SEM_H__

#include <semaphore.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct
{
	sem_t *sem;
	char sem_name[32];
}sem_node_t;

int sem_node_init(sem_node_t *sem_node, int pshared, unsigned int value);

int sem_node_destroy(sem_node_t *sem_node);

int sem_node_wait(sem_node_t *sem_node);

int sem_node_post(sem_node_t *sem_node);
    
#ifdef __cplusplus
}
#endif

#endif

