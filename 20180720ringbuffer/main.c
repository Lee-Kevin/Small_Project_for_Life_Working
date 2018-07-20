
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include "ringbuffer.h"

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

    // do { \
        // printf("[Trace]:[%s %s - %s|%03d]:: ", __DATE__, __TIME__, __func__, __LINE__); \
        // printf(fmt, ##__VA_ARGS__); \
        // printf("%s", "\r\n"); \
    // } while(0)

#define BUFFER_POOL_SIZE (32*5)
uint8_t buffer_pool[BUFFER_POOL_SIZE];

pthread_t pt_1 = 0;
pthread_t pt_2 = 0;

struct ring_buffer buffer;

static void pthread_func_1 (void);
static void pthread_func_2 (void);
uint8_t thread_buffer_put  =  1;
void main(){
	debug_printf("hello world \n");
	
	ring_buffer_init(&buffer,buffer_pool,BUFFER_POOL_SIZE);
	
	
	int ret = 0;
	
	if (pthread_create(&pt_1, NULL, (void *)pthread_func_1,NULL) != 0) {
		perror ("pthread_1_create");
		exit(0);
	}
	
	if (pthread_create(&pt_2, NULL, (void *)pthread_func_2,NULL) != 0) {
		perror ("pthread_2_create");
		exit(0);
	}
	
	while(1) {
		char chartemp[10];
		scanf("%s",chartemp);
		if (strcmp(chartemp,"stop")==0) {
			thread_buffer_put = 0;
		} else if (strcmp(chartemp,"start")==0) {
			thread_buffer_put = 1;
		} else {
			printf("Please input again");
		}
	}
	
}

static void pthread_func_1 (void)
{
  int i = 0;
  int ret = 0;
  uint8_t data[32];
  static int j = 0;
  while(1) {
	  if(thread_buffer_put) {
		  debug_printf("--------put The %d data is:----- ",32);
		  for( i=0; i<32; i++ ){
			data[i] = 1+i+32*j;
			// debug_printf ("\nThread_1 add one to num:%d\n",gnum);
			printf("%d ",data[i]);
		  }
		   printf("\r\n");
		  j++;
		  ret = ring_buffer_put(&buffer,data,sizeof(data));
		  debug_printf ("\nThread_1 put %d datas to buffer",ret*j);
		  
	  }
	  sleep(1);
  }
}

static void pthread_func_2 (void)
{
  int ret = 0;
  uint8_t data[32];
  while(1) {

	  ret = ring_buffer_get(&buffer,data,sizeof(data));
	  debug_printf("Got The %d data is: ",ret);
	  for(int i=0; i<ret; i++ ){
		printf("%d ",data[i]);
	  }
	  printf("\r\n");
	  sleep(2);
	  
  }
}