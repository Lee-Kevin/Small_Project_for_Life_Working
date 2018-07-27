
#include <signal.h>  
#include <stdio.h>  
#include <stdlib.h>  
#include <error.h>  
#include <string.h>  
#include <time.h>
#include <pthread.h>
#include "sem.h"

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



void sig_int_handler(int signum)  
{  
    printf("Recive SIGINT signal\n");
	// exit(0);
}  
  
void sig_term_handle(int signum)  
{  
    printf("Recive SIGTERM signal\n");  
    exit(0);  
}  
  
pthread_t pt_1 = 0;
pthread_t pt_2 = 0;

static void pthread_func_1 (void *ptr);
static void pthread_func_2 (void *ptr);

sem_node_t sem_node_test;

void main() {
	
		struct sigaction action, old_action;
		
		/* 设置SIGINT */  
		action.sa_handler = sig_int_handler;  
		sigemptyset(&action.sa_mask);  
		/* 安装handler的时候, 设置在handler 
		 * 执行期间, 屏蔽掉SIGTERM信号 */  
		// sigaddset(&action.sa_mask, SIGTERM);  
		action.sa_flags = 0;  
		sigaction(SIGINT, NULL, &old_action);  
		if (old_action.sa_handler != SIG_IGN) {  
			sigaction(SIGINT, &action, NULL);  
		}  
		
		/* 设置SIGTERM */  
		action.sa_handler = sig_term_handle;  
		sigemptyset(&action.sa_mask);  
		action.sa_flags = 0;  
		sigaction(SIGTERM, NULL, &old_action);  
		if (old_action.sa_handler != SIG_IGN) {  
			sigaction(SIGTERM, &action, NULL);  
		}  
	
        debug_printf("hello world");
		sem_node_init(&sem_node_test,0,0);
		sem_node_post(&sem_node_test);
		sem_node_post(&sem_node_test);
		if (pthread_create(&pt_1,NULL,(void *)pthread_func_1,NULL) != 0) {
			perror("pthread 1 create");
			exit(0);
		}
		
		if (pthread_create(&pt_2,NULL,(void *)pthread_func_2,NULL) != 0) {
			perror("pthread 1 create");
			exit(0);
		}
		
		while(1) {
			char buftemp[20];
			scanf("%s",buftemp);
			if (strcmp(buftemp,"kill") == 0) {
				kill(getpid(),SIGTERM);
			}
			
		}
}

static void pthread_func_1 (void *ptr) {
	int done = 0;
	static int index = 0;
	do {
		sem_node_wait(&sem_node_test);
		debug_printf("I am pthread_func_1 %d",index++);
		
	} while(!done);
}

static void pthread_func_2 (void *ptr) {
	int done = 0;
	static int index = 0;
	do {
		
		sem_node_post(&sem_node_test);
		sleep(1);
		debug_printf("I am pthread_func_2 %d",index++);
	} while(!done);
}

// int main(int argc, char **argv)  
// {  
    // struct sigaction action, old_action;  
  
    // /* 设置SIGINT */  
    // action.sa_handler = sig_handler;  
    // sigemptyset(&action.sa_mask);  
    // /* 安装handler的时候, 设置在handler 
     // * 执行期间, 屏蔽掉SIGTERM信号 */  
    // // sigaddset(&action.sa_mask, SIGTERM);  
    // action.sa_flags = 0;  
	// debug_printf("sigaction 1 --\n");  
    // sigaction(SIGINT, NULL, &old_action);  
    // if (old_action.sa_handler != SIG_IGN) {  
	
		// debug_printf("sigaction again --\n");  
        // sigaction(SIGINT, &action, NULL);  
    // }  
	
    // /* 设置SIGTERM */  
    // action.sa_handler = handle_term;  
    // sigemptyset(&action.sa_mask);  
    // action.sa_flags = 0;  
  
    // sigaction(SIGTERM, NULL, &old_action);  
    // if (old_action.sa_handler != SIG_IGN) {  
        // sigaction(SIGTERM, &action, NULL);  
    // }  
  
    // printf("--> send SIGINT -->\n");  
    // kill(getpid(), SIGINT);  
	
    // while (1) {  
        // sleep(1);  
		// kill(getpid(), SIGTERM);
    // }  
  
    // return 0;  
// }  
