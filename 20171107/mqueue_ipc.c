/*
 该文件展示了消息队列 system V IPC 的API的使用过程
 Here is the describtion of the difference between System V IPC and Posix IPC
*/

#include <stdio.h>  
#include <mqueue.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#define MQ_FILE "/DeviceStatus"
#define BUF_LEN 128

void *thr_fun1(void *ptr) {
	mqd_t mymqd;
	int cnt;
	int msg_prio = 1;
	struct mq_attr attr;
	char buf[BUF_LEN];
	mymqd = *((mqd_t*)ptr);
	printf("\n This is Pthread thr_fun1--\n");
	printf("thr_fun2 The mqd is %d\n",(int)(mymqd));
	while(1) {
		 msg_prio = 1;
		cnt = mq_receive(mymqd,buf,BUF_LEN,&msg_prio);
		if (0 < cnt) {
			printf("\nThr_fun1 recived %s, the cnt is %d, \tThe prio is %d\n",buf,cnt,msg_prio);
		} else {
			//sleep(1);
			printf("thr_fun1 while The mqd is %d\n",(int)(mymqd));
		}
	}
}

void *thr_fun2(void *ptr) {
	mqd_t mymqd;
	int cnt;
	int msg_prio = 2;
	struct mq_attr attr;
	char buf[BUF_LEN];
	mymqd = *((mqd_t*)ptr);
	printf("\n This is Pthread thr_fun2--\n");
	printf("thr_fun2 The mqd is %d\n",(int)(mymqd));
	while(1) {
		msg_prio = 2;
		cnt = mq_receive(mymqd,buf,BUF_LEN,&msg_prio);
		if (0 < cnt) {
			printf("\nThr_fun2 recived %s, the cnt is %d, \tThe prio is %d\n",buf,cnt,msg_prio);
		} else {
			//sleep(1);
			printf("thr_fun2 while The mqd is %d\n",(int)(mymqd));
		}
	}
}

int main()
{
    mqd_t mqd;
	pthread_t td1;
    char buf[BUF_LEN];
    int  por = 0;
    int ret = 0;
	int err1;
	
    struct mq_attr attr;
    attr.mq_flags = 0;
    attr.mq_maxmsg = 3;
    attr.mq_msgsize = 50;
    attr.mq_curmsgs= 0;
    mqd = mq_open(MQ_FILE, O_CREAT|O_RDWR,0666,&attr);
    if (-1 == mqd)
    {
        printf("mq_open error.\n");
        return -1;
    }
	printf("The mqd is %d\n",(int)(mqd));
	
	if (0 != pthread_create(&td1,NULL,thr_fun1,&mqd)) {
		fprintf(stderr,"Can't create the thread");
		exit(1);
	}
	
	if (0 != pthread_create(&td1,NULL,thr_fun2,&mqd)) {
		fprintf(stderr,"Can't create the thread");
		exit(1);
	}
	
	//sleep(1);
    do{
		memset(buf,'\0',BUF_LEN);
        buf[BUF_LEN-1]='\0';
        printf("MQ_MSG : ");
        scanf("%s", buf);
        if(buf[BUF_LEN-1]!= '\0')
        {
            continue;
        }
        printf("strlen:%d\nMQ_POR : ",strlen(buf));
        scanf("%d", &por);  // 设置优先级
        ret== mq_send(mqd, buf, strlen(buf)+1, por);
        if (ret != 0)
        {
            perror("mq_send error.\n");
        }
       
    }while(strcmp(buf, "quit"));
    mq_close(mqd); // 关闭后 调用进程不可以使用该描述符
    mq_unlink(MQ_FILE);  // 系统中删除名为MQ_FILE的队列
	printf("\n Going to exit the APP");
    return 0;
}
  
// struct myMsg  
// {  
    // long msgType;              // 不同的msgType代表不同消息类型
    // char msgText[MAXBUF+1]; 
	// char msg2[20];
// };  
  

// struct msqid_ds msgbuf;      // 获取消息属性的结构体  每个消息队列都有一个msqod_ds结构与其关联
  
  
// void *thr_fun1(void *arg)  
// {  
    // //printf("here is thread 1!\n");  
    // struct myMsg msg;  
    // //const int qid=*((int *)arg);  
    // int qid, tmpqid, err;  
    // tmpqid = *((int *)arg);  
    // qid = tmpqid;  
	
	// printf("This is thr_fun1 \n");  
	// printf("thr_fun1 The pid is %d \n",tmpqid);  
	
	
    // while (1)  
    // {
		
        // // qid = tmpqid;     
        // /* msgrcv竟然会在出错返回时改掉 qid 的值，加const修饰都会被改为 1  
         // * 太扯淡了，所以必须使用qidtmp保存第一次传进来的 qid值， 
         // * 以便后面消息的接收 
         // */  
		 
		// /*
		// 从消息队列中取消息
		// ssize_t msgrcv(int msqid, void *ptr, size_t nbytes, long type, int flag); 
		
		// ptr: 指向一个长整型数(返回的消息类型存放在其中), 跟随其后的是存放实际消息数据的缓冲区. 
		// nbytes: 数据缓冲区的长度. 若返回的消息大于nbytes, 且在flag中设置了MSG_NOERROR, 则该消息被截短. 
		// type: 
		// type == 0: 返回队列中的第一个消息. 
		// type > 0: 返回队列中消息类型为type的第一个消息. 
		// type < 0: 返回队列中消息类型值小于或等于type绝对值的消息, 如果这种消息有若干个, 则取类型值最小的消息. 
		
		// */
        // if (-1 == (err = msgrcv(qid, &msg, sizeof(struct myMsg), 1, IPC_NOWAIT)) )  
        // {  

            // continue;  
            // //pthread_exit((void *)-1);  

        // }  
        // printf("thread1:received msg \"%s\" %s  MsgType = %d\n", msg.msgText, msg.msg2, msg.msgType);  
          

        // msg.msgType = 2; 
        // sprintf(msg.msgText,"%s","Message from thr_fun1"); 
        // if ( -1 == (err = msgsnd(qid, &msg, sizeof(struct myMsg), 0)) ) 
        // { 
            // perror("thr1: send msg error!\n"); 
        // } 

        // //sleep(2);  
          
    // }  
      
    // //fprintf(stdout, "thread1 receive: %s\n", msg.msgText);  
    // //  
  
  
      
  
  
    // //pthread_exit((void *)2);  
// }  
  
  
// void *thr_fun2(void *arg)  
// {  
    // //printf("here is thread 2!\n");  
    // struct myMsg msg;  
    // int qid, qidtmp;  
    // int err;  
    // qidtmp = *((int *)arg);  
	// printf("This is thr_fun2 \n");  
	// printf("thr_fun2 The pid is %d \n",qidtmp); 
	// qid = qidtmp;
    // while (1)  
    // {  
        
		// /* Recive any message */
        // if (-1 == (err = msgrcv(qid, &msg, sizeof(struct myMsg),2, IPC_NOWAIT)) )  
        // {  
            // continue;  
            // //pthread_exit((void *)-1);  
        // }  
          
          
        // else  
        // {  
            // printf("thread2:received msg \"%s\" MsgType = %d\n", msg.msgText, msg.msgType);  
        // }  
        // /*  
        // msg.msgType = 1; 
 
 
        // sleep(2); 
        // if ( -1 == (err = msgsnd(qid, &msg, sizeof(struct myMsg), 0)) ) 
        // { 
            // perror("thr2: send msg error!\n"); 
 
 
        // } 
        // */  
        // //sleep(2);  
          
    // }  
      
  
  
      
      
    // //fprintf(stdout, "thread1 receive: %s\n", msg.msgText);  
    // //msg.msgType = 2;  
  
  
    // //msgsnd(qid, &msg, sizeof(struct myMsg), 0);  
  
  
    // //pthread_exit((void *)2);  
  
  
// }  
  
// void *thr_fun3(void *arg)  
// {  
    // struct myMsg msg;  
    // int qid, qidtmp;  
    // int err;  
    // qidtmp = *((int *)arg);  
	// printf("This is thr_fun3 \n");
	// printf("thr_fun3 The pid is %d \n",qidtmp); 
	// qid = qidtmp;
    // while (1)  
    // {  
		
        
        // //printf("thread3111111: qid = %d  ", qid);  
        // if (-1 == (err = msgrcv(qid, &msg, sizeof(struct myMsg), 3, IPC_NOWAIT)) )  
        // { 
            // continue;  
            // //pthread_exit((void *)-1);  
        // }  
        // else  
        // {  
			// qid = qidtmp;  
            // printf("thread3222222: qid = %d  ");  
            // err = msgctl(qid, IPC_RMID, 0);    // 删除消息队列  
            // if ( 0 == err )  
            // {  
                // printf("msg queue removed successfully!\n");  
                // printf("thread3:received msg \"%s\" MsgType = %d\n", msg.msgText, msg.msgType);  
                // printf("Now going to shutdown the system!\n");  
                // pthread_exit((void *)0);  
            // }  
            // else  
            // {  
                // perror("thread3: delete msg queue error!\n");  
                // exit (-1);  
            // }  
              
        // }  
        // /*  
        // msg.msgType = 1; 
 
 
        // sleep(2); 
        // if ( -1 == (err = msgsnd(qid, &msg, sizeof(struct myMsg), 0)) ) 
        // { 
            // perror("thr2: send msg error!\n"); 
 
 
        // } 
        // */  
        // //sleep(2);  
          
      
    // }  
// }  
  
  
  
  
  
  
  
  
// int main(int argc, char *argv[])  
// {  
    // pthread_t td1, td2, td3;  
    // int err1, err2, err3, err;  
    // int qid;  
    // key_t key;  
  
  
    // char buf[MAXBUF+1];  
    // int tmpMsgType;  
      
    // struct myMsg msg;  
  
  
    // fflush(stdin);  
    // fflush(stdout);  
  
  
    // if ( -1 == (key = ftok("/home/ubuntu/share/posixMessage", 11)) )   // 创建key  
    // {  
         // perror("Create key error!\n");  
         // exit (-1);  
    // }  
    // /* 创建消息队列，若存在直接报错 */  
    // if ( -1 == (qid = msgget(key, IPC_CREAT | 0777)) )  // | IPC_EXCL     创建消息队列 或打开一个已经存在的队列
    // {  
        // perror("message queue already exitsted!\n");  
        // exit (-1);  
    // }  
    // printf("create . open queue qid is %d!\n", qid);  
      
	  
    // err1 = pthread_create(&td1, NULL, thr_fun1, &qid);  
    // if ( 0 != err1 )  
    // {  
        // fprintf(stderr, "sync thread create error!\n");  
        // exit(1);  
    // }  
  
  
    // err2 = pthread_create(&td2, NULL, thr_fun2, &qid);  
    // if ( 0 != err2 )  
    // {  
        // fprintf(stderr, "fdatasync thread create error!\n");  
        // exit(1);  
    // }  
  
  
    // err3 = pthread_create(&td3, NULL, thr_fun3, &qid);  
    // if ( 0 != err3 )  
    // {  
        // fprintf(stderr, "sync thread create error!\n");  
        // exit(1);  
    // }  
      
    // while (1)  
    // {  
        // printf("Input the string: ");  
        // scanf("%s", buf);  
        // printf("buf = %s\n",buf);  
        // fflush(stdin);  
        // fflush(stdout);  
  
  
        // strncpy(msg.msgText, buf, MAXBUF);  
  
        // printf("Input the msg2 string: ");  
        // scanf("%s", buf);  
        // printf("msg2 = %s\n",buf);  
        // fflush(stdin);  
        // fflush(stdout);  
  
  
        // strncpy(msg.msg2, buf, 20);    
  
        // printf("Input the msgType:");  
        // scanf("%d", &tmpMsgType);  
        // printf("msgType = %d\n", tmpMsgType);  
        // fflush(stdin);  
        // fflush(stdout);  
        // msg.msgType = tmpMsgType;  
		
		// /* 调用msgsnd 将数据放到消息队列中 */
          
        // if ( -1 == (msgsnd(qid, &msg, sizeof(struct myMsg), 0)) )  
        // {  
            // perror("msg closed! quit the system!\n");  
            // break;  
            // //exit (-1);  
        // }  
        // printf("Write msg success!\n");  
  
  

        // printf("\n---------------------------\n"); 
		
		// /*
		// 函数原型
		 // int msgctl(int msqid, int cmd, struct msgqid_ds *buf);
		 
		 // cmd参数说明对msqid 执行的命令
		 // IPC_STAT 取此队列的msqid_ds结构，并存入buf指定的结构中。
		 // IPC_SET  按由buf指向结构中的值，设置相关的msg_perm.uid，msg_perm.gid,msg_perm.mode和msg_per.uid.
		 // IPC_RMID 从系统中删除该消息队列，以及所有数据
		 
		// */
        // err = msgctl(qid, IPC_STAT, &msgbuf);            // 消息队列的垃圾桶函数
        // if ( err != -1 ) 
        // { 
            // printf("current number of bytes on queue: %ld\n", msgbuf.msg_cbytes); 
            // printf("number of msg in queue is: %ld\n", msgbuf.msg_qnum); 
            // printf("Max number of bytes on queue is: %ld\n", msgbuf.msg_qbytes); 
        // } 
        // else 
        // { 
            // printf("msg queue msgctl error!\n"); 
        // } 
        // printf("---------------------------\n");  

        // sleep(2);  
    // }  
      
  
  
    // /* 
    // err1 = pthread_join(td1, NULL); 
    // if ( 0 != err1 ) 
    // { 
        // printf("join thread1 failed!\n"); 
        // exit(1); 
    // } 
 
 
    // err2 = pthread_join(td2, NULL); 
    // if ( 0 != err2 ) 
    // { 
        // printf("join thread2 failed!\n"); 
        // exit(1); 
    // } 
    // */  
      
    // err3 = pthread_join(td3, NULL);  
    // if ( 0 != err3 )  
    // {  
        // printf("join thread3 failed!\n");  
        // exit(1);  
    // }  
  
  
      
      
    // return 0;  
// }  