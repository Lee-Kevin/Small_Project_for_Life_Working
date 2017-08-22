#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <unistd.h>
/*全局变量*/
int gnum = 0;
/*互斥量 */
pthread_mutex_t mutex;
/*声明线程运行服务程序*/
static void pthread_func_1 (void);
static void pthread_func_2 (void);
int main (void)
{
 /*线程的标识符*/
  pthread_t pt_1 = 0;
  pthread_t pt_2 = 0;
  int ret = 0;
  /*互斥初始化*/
  pthread_mutex_init (&mutex, NULL);
  /*分别创建线程1、2*/
  ret = pthread_create( &pt_1,                  //线程标识符指针
                                     NULL,                  //默认属性
                                     (void *)pthread_func_1,//运行函数
                                     NULL);                  //无参数
  if (ret != 0)
  {
     perror ("pthread_1_create");
  }
  ret = pthread_create( &pt_2,                  //线程标识符指针
                                     NULL,                   //默认属性
                                     (void *)pthread_func_2, //运行函数
                                     NULL);                  //无参数
  if (ret != 0)
  {
     perror ("pthread_2_create");
  }
  /*等待线程1、2的结束*/
  printf ("I am going to wait the thread\n");
  // pthread_join (pt_1, NULL);
  // printf ("Finished thread 1 \n");
  // pthread_join (pt_2, NULL);
  while(1);
  printf ("main programme exit!\n");
  
  return 0;
}
/*线程1的服务程序*/
static void pthread_func_1 (void)
{
  int i = 0;
  for( i=0; i<3; i++ ){
    printf ("This is pthread_1!/n");
//    pthread_mutex_lock(&mutex); /*获取互斥锁*/
        /*注意，这里以防线程的抢占，以造成一个线程在另一个线程sleep时多次访问互斥资源，所以sleep要在得到互斥锁后调用*/
    sleep(1);
    /*临界资源*/
    gnum++;
    printf ("\nThread_1 add one to num:%d\n",gnum);
//    pthread_mutex_unlock(&mutex); /*释放互斥锁*/
  }
  pthread_exit ( NULL );
}
/*线程2的服务程序*/
static void pthread_func_2 (void)
{
  int i = 0;
  for( i=0; i<6; i++ )  {
    printf ("This is pthread_2!/n");
 //   pthread_mutex_lock(&mutex); /*获取互斥锁*/
        /*注意，这里以防线程的抢占，以造成一个线程在另一个线程sleep时多次访问互斥资源，所以sleep要在得到互斥锁后调用*/
    sleep(1);
    /*临界资源*/
    gnum++;
    printf ("Thread_2 add one to num:%d\n",gnum);
 //   pthread_mutex_unlock(&mutex); /*释放互斥锁*/
  }
  pthread_exit ( NULL );
} 
