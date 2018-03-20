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
