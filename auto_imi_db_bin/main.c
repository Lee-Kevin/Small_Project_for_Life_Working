/*-------------------------------------------------------------------------*/
/**
   @file    main.c
   @author  Kevin.lee
   @brief   test for imi_db_mi
*/
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "src/iniparser.h"
#include "src/imi_db_mi.h"

void main() {
	int ret = 0;
	char test_out[32] = {0};
	printf("hello world\n");
	imi_db_mi_init("./test.ini");
	for (int i=0; i<PROP_MAX_NUM; i++) {
		bzero(test_out,sizeof(test_out));
		ret = imi_db_mi_get_value(i,test_out,sizeof(test_out));
		printf("--------------------------------------------\n");
		printf("id:%d              ret=%d\n",i,ret);
		if(test_out[0] == '\0') {
			printf("testout=null \n");
		} else {
			printf("testout=%s \n",test_out);
		}
		printf("\n");
	}
	int i = 1;
	imi_db_mi_set_value(PROP_RW_IPC_BIND_STATUS,&i, sizeof(int),false);
	bzero(test_out,sizeof(test_out));
	imi_db_mi_get_value(PROP_R_VENDOR,test_out,sizeof(test_out));
	printf("testout=%s \n",test_out);
	i=3;
	imi_db_mi_set_value(PROP_RUN_MOTOR_RESET,&i, sizeof(int),false);
//	ret = imi_db_mi_get_value(PROP_RUN_MOTOR_RESET,test_out,sizeof(test_out));
	printf("--------------------------------------------\n");
	printf("id:%d              ret=%d\n",PROP_RUN_MOTOR_RESET,ret);
		if(test_out[0] == '\0') {
			printf("testout=null \n");
		} else {
			printf("testout=%s \n",test_out);
	}
	imi_db_mi_set_value(PROP_RAM_RUNLEVEL,"ota",sizeof(test_out),false);
	bzero(test_out,sizeof(test_out));
	imi_db_mi_get_value(PROP_RAM_RUNLEVEL,test_out,sizeof(test_out));
	printf("testout=%s \n",test_out);
	while(1) {
		sleep(1);
	}
}


