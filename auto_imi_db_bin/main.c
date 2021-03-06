/*-------------------------------------------------------------------------*/
/**
   @file    main.c
   @author  Kevin.lee
   @brief   test for imi_db_mi
*/
/*--------------------------------------------------------------------------*/
#include <stdio.h>
#include "iniparser.h"
#include "imi_db_mi.h"
db_info_keys_t db_keys[] = 
{
	{KEY_ROM_VENDOR,				NULL,					"vendor information"},
	{KEY_ROM_MODEL,					NULL,					NULL},
	{KEY_ROM_LANGUAGE,				NULL,		 			NULL},
	{KEY_ROM_WIFI_MAC,				NULL,					NULL},
	{KEY_ROM_OOB,					NULL,					NULL},
	{KEY_ROM_MJAC_I2C_PORT,			NULL,					NULL},
	{KEY_ROM_MJAC_GPIO,				NULL,					NULL},
	{KEY_ROM_MJAC_DID,				NULL,					NULL},
	{KEY_ROM_MJAC_KEY,				NULL,					"key value for mijia"},

	{KEY_NVM_IPC_BIND_STATUS,		"failed",				NULL},
	{KEY_NVM_IPC_POWER,				"1",					NULL},
	{KEY_NVM_IPC_LED,				"1",					NULL},
	{KEY_NVM_IPC_IMPROVE_PRO,		"0",					NULL},
	{KEY_NVM_IPC_MOTOROM_POSITION,	"[50,50]",				NULL},
	{KEY_NVM_P2P_TOKEN,				"",						NULL},
	{KEY_NVM_CLOUD_DETECT_SwITCH,	"0", 					NULL},
	{KEY_NVM_CLOUD_BABYCRY_SwITCH,	"0",					NULL},
	{KEY_NVM_CLOUD_INTERVAL,		"5",					NULL},
	{KEY_NVM_CLOUD_START_TIME,		"",						NULL},
	{KEY_NVM_CLOUD_END_TIME,		"",						NULL},
	{KEY_NVM_VIDEO_WDR,				"0",					NULL},
	{KEY_NVM_VIDEO_FLIP,			"0",					NULL},
	{KEY_NVM_VIDEO_WATERMARK,		"1",					NULL},
	{KEY_NVM_ALGO_SENSITIVITY,		"",						NULL},
	{KEY_NVM_ALGO_TRACK_SWITCH,		"0",					NULL},
	{KEY_NVM_ALGO_IRCUT_MODE,		"0",					NULL},
	{KEY_NVM_WIFI_SSID,				"",						NULL},
	{KEY_NVM_WIFI_PWD,				"",						NULL},
	{KEY_NVM_TIMEZONE,				"",						NULL},
	{KEY_NVM_SD_STROGE_MODE,		"0",					NULL},
	
	{KEY_RAM_MOTOROM_RESET,			"0",					NULL},
	
	{KEY_MEM_RUNLEVEL,				"runlevel",				NULL},
};


void main() {
	int ret = 0;
	int key_num = 32;
	char test_out[32] = {0};
	printf("hello world\n");
	imi_db_mi_init();
	for (int i=0; i<key_num; i++) {
		bzero(test_out,sizeof(test_out));
		printf("********************************************\n");
		ret = imi_db_mi_get_value(db_keys[i].key,test_out,sizeof(test_out));
		printf("--------------------------------------------\n");
		printf("id:%d              ret=%d\n",i,ret);
		if(test_out[0] == '\0') {
			printf("testout=null \n");
		} else {
			printf("key= %s \t value = %s \n",db_keys[i].key,test_out);
		}
		printf("\n");
	}
	int i = 1;
	imi_db_mi_set_value(KEY_NVM_IPC_BIND_STATUS,"2",true);
	bzero(test_out,sizeof(test_out));
	imi_db_mi_get_value(KEY_NVM_IPC_BIND_STATUS,test_out,sizeof(test_out));
	printf("key = %s testout=%s \n",KEY_NVM_IPC_BIND_STATUS,test_out);
	i=3;
	imi_db_mi_set_value(KEY_RAM_MOTOROM_RESET,"4",true);
	ret = imi_db_mi_get_value(KEY_RAM_MOTOROM_RESET,test_out,sizeof(test_out));
	printf("--------------------------------------------\n");
	printf("id:%d              ret=%d\n",KEY_RAM_MOTOROM_RESET,ret);
		if(test_out[0] == '\0') {
			printf("testout=null \n");
		} else {
			printf("testout=%s \n",test_out);
	}
	imi_db_mi_set_value(KEY_MEM_RUNLEVEL,"normal",true);
	bzero(test_out,sizeof(test_out));
	imi_db_mi_get_value(KEY_MEM_RUNLEVEL,test_out,sizeof(test_out));
	printf("testout=%s \n",test_out);
	
	while(1) {
		sleep(1);
	}
}


