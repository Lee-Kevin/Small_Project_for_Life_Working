
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "list.h"

list_t devTable;

void main(){
	
	// devInfoTable_t devInfo;
	
	devInfoTable_t *temp;
	
	devInfoTable_t *devInfo = (devInfoTable_t *)malloc(sizeof(devInfoTable_t));
	
	list_t *devTable = (list_t *)malloc(sizeof(list_t)); 
	list_init(devTable);
	
	printf("\n Hello world \n");
	memset(devInfo,0,sizeof(devInfoTable_t));
	
	sprintf(devInfo->devId,"%s","111122");
	sprintf(devInfo->shortAddr,"%s","0001");
	list_append(devTable,devInfo);
	
	
	
	sprintf(devInfo->devId,"%s","111133");
	sprintf(devInfo->shortAddr,"%s","0002");
	list_append(devTable,devInfo);
	
	sprintf(devInfo->devId,"%s","111144");
	sprintf(devInfo->shortAddr,"%s","0003");
	list_append(devTable,devInfo);
	
	
	
    for (int i = 0; i < list_len(devTable); i++){//list_len获取链表的长度
        list_get(devTable, i, (void **)&temp); //取得位置为i的结点的数据
		printf("\nIndex is %d\n", i);
        printf("dev Id %s\n", temp->devId);
        printf("shortAddr %s\n", temp->shortAddr);
    }
	
	memset(devInfo,0,sizeof(devInfoTable_t));
	sprintf(devInfo->devId,"%s","111144");
	sprintf(devInfo->shortAddr,"%s","0000");
			
	int i = list_index_by_devID(devTable,devInfo);
	printf("\n the index is %d",i);
	if( list_pop(devTable,i) ) {
		printf("\n pop the message successfully");
	} else {
		printf("\n can't pop the message");		
	}
	
	if (list_pop_tail(devTable)) {
		printf("\n pop the tail successfully");	
	} else {
		printf("\n cann't pop the tail ");	
	}
	
	while(1) {
		char chartemp[10];
		printf("> Please input the data number:\n>");
	    scanf("%s",chartemp);
		
		if(strcmp("add",chartemp) == 0) {
			sprintf(devInfo->devId,"%s","111166");
			sprintf(devInfo->shortAddr,"%s","0000");
			list_append(devTable,devInfo);
		} else if (strcmp("pop",chartemp) == 0) {
			memset(devInfo,0,sizeof(devInfoTable_t));
			sprintf(devInfo->devId,"%s","111144");
			sprintf(devInfo->shortAddr,"%s","0000");
			
			int i = list_index_by_devID(devTable,devInfo);
			printf("\n the index is %d",i);
			if( list_pop(devTable,i) ) {
				printf("\n pop the message successfully");
			} else {
				printf("\n can't pop the message");
			}
			
		} else if (strcmp("getindex1",chartemp) == 0){
			devInfoTable_t devInfo1;
			memset(&devInfo1,0,sizeof(devInfoTable_t));
			sprintf(devInfo1.devId,"%s","111177");
			sprintf(devInfo1.shortAddr,"%s","0000");

			int i = list_index_by_devID(devTable,&devInfo1);
			printf("\n The Index is %d",i);
		} else if (strcmp("getindex2",chartemp) == 0){
			devInfoTable_t devInfo1;
			memset(&devInfo1,0,sizeof(devInfoTable_t));
			sprintf(devInfo1.devId,"%s","111133");
			sprintf(devInfo1.shortAddr,"%s","0000");
			
			int i = list_index_by_devID(devTable,&devInfo1);
			printf("\n The Index is %d",i);
		} else if (strcmp("poptail",chartemp) == 0){
			
			if (list_pop_tail(devTable)) {
				printf("\n pop the tail successfully");	
			} else {
				printf("\n cann't pop the tail ");	
			}
			
		} else if (strcmp("print",chartemp) == 0){
			for (int i = 0; i < list_len(devTable); i++){//list_len获取链表的长度
				list_get(devTable, i, (void **)&temp); //取得位置为i的结点的数据
				printf("\nIndex is %d\n", i);
				printf("dev Id %s\n", temp->devId);
				printf("shortAddr %s\n", temp->shortAddr);
			}
		} else {
			printf("\n Please input again! \n");
		}
	}
	free(devInfo);
}
