# Small Project for life and working
**There is all my project for life and working, for a better experience.**

## 20161109
This project is based on Linkit Smart 7688 Duo, Then run a python script from Arduino side and read the result. Acccording to the result light up different color LED Strip. 
> This project will checkout if somebody's K3 Cloud account is online, if so, the LED Stripe will turn into red.

## 20161114

This is a python script that can turn a picture to a ascii text.

## 20161121

This is a project that can indicate if the door is open.

## 20161122

This is a FFT demo based on Arduino FHT library.

## 20170213

This is a project based on python to deal with json file, and change some special values.

## 20170626 - Arduino Music Player Game

This is a demo that use two button and a speaker to play a song, In fact this is a game that not so interesting.

## 20170801 - C Deal with Json

This is the demo shows how to deal with the string to json format.

## 20170808- Using C to Operate SQLite Interface Demo
This is the demo shows how to use C API to create a table for sql and update, insert, delete data from a database, in the demo I use the Sqlite3 as the database.

Execuate:
Take sqldelete.c for example:

```shell
gcc -o delete sqldelete.c -l sqlite3
```
Then you'll find there is a execuatable file named delete, and run the following code.
```shell
./delete
```

## 20170815 TCP/IP Server/Client

This is a demo that shows how to use tcpdump to see the TCP protocol structure, use 3 termials to run tcpdump server and client.

```shell
sudo tcpdump -vvv -X -i lo tcp port 777
```

Run the following code to run the server.

```shell
./server 127.0.0.1
```

And run the code to run the client.

```shell
./client 127.0.0.1
```

## 20170822 pthread app demo and mutex app 

This is a demo that shows how to use pthread to create many threads to run, and use mutex to protect some shared variables.
Run the following code to have a test:

```shell
 gcc -o app main.c -lpthread
```
Then run the app

```shell
./app 
```
You can have a try and see the result.

## 20170831 A way to get the MAC Address

This demo shows a way to get the machine mac address.

## 20171107 Two ways for using MSG Queue

This is a demo shows two ways for mqueue, System mqueue can be used for send messages to different threads while the Posix mqueue can
be used for when the message need priority info. 

## 20180208 A C list demo

This is a demo shows how to use c to achive list function, you can change the data structure in the list.h file,
In this demo, you can pop node or append node to the list. GDB is a good tool to see the debug process.

Just run 
```
make
```
## 20180320queue Circle Queue demo based on list

This is a circle queue demo based on list, you can put the string to the queue, and read it in another thread, this queue is thread safe. when you put the items larger than the size you define, the queue will cover the data at the front.

Here is the example:

```
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"

msgqueue_t msgs; // message queue

void addmsg(void) {
	Elem m3;
	static unsigned int i;
	m3.data = (char *) malloc(30);
	sprintf(m3.data,"The number is %d",i++);
	QUEUE_PUSH(&msgs, &m3);
	// free(m3.content);
}

int main(void) {
    
    Elem m1 ;
	printf("DBG[1]\n");
    QUEUE_INIT(&msgs, 10);
	printf("DBG[2]\n");
    m1.data = "abc";
	printf("DBG[3]\n");
    QUEUE_PUSH(&msgs, &m1);
    m1.data = "123";
	printf("DBG[4]\n");
    QUEUE_PUSH(&msgs, &m1);
    m1.data = "456";
    QUEUE_PUSH(&msgs, &m1);
	
	for(int i=0; i<15; i++) {
		addmsg();
	}
	
	
    printf("msgs size: %d\n", QUEUE_SIZE(&msgs)); // msgs size: 1

	while(QUEUE_SIZE(&msgs)) {
		Elem m2;
		QUEUE_POP(&msgs, &m2);
		printf("m2 content: %s\n", m2.data); // m2 content: abc
		free(m2.data);
	}
	while(1);
    // QUEUE_FREE(msgs);
}

```

