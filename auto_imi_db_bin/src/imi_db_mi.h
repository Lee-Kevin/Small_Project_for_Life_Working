
/*-------------------------------------------------------------------------*/
/**
   @file    imi_db_mi.h
   @author  Kevin.lee
   @brief   data structure for mi ipc 
*/
/*--------------------------------------------------------------------------*/

#ifndef _IMI_DB_MI_H_
#define _IMI_DB_MI_H_

#include <stdbool.h>
#define RET_OK				(0)
#define RET_NO_VALUE_ERR	(-1)
#define RET_LENGTH_ERR		(-2)
#define RET_NO_KEY_ERR		(-3)
#define RET_READONLY_ERR	(-4)
#define RET_UNKONW_ERR		(-5)

#define KEY_ROM_VENDOR 						"ROM.vendor"
#define KEY_ROM_MODEL						"ROM.model"
#define KEY_ROM_LANGUAGE					"ROM.language"
#define KEY_ROM_WIFI_MAC					"ROM.mac"
#define KEY_ROM_OOB							"ROM.oob"
#define KEY_ROM_MJAC_I2C_PORT				"ROM.mjac_i2c"
#define KEY_ROM_MJAC_GPIO					"ROM.mjac_gpio"
#define KEY_ROM_MJAC_DID 					"ROM.did"
#define KEY_ROM_MJAC_KEY 					"ROM.key"

#define KEY_NVM_IPC_BIND_STATUS 			"NVM.IPC:bind_status"
#define KEY_NVM_IPC_POWER 					"NVM.IPC:power"
#define KEY_NVM_IPC_LED 					"NVM.IPC:led_status"
#define KEY_NVM_IPC_IMPROVE_PRO 			"NVM.IPC:improve_program"
#define KEY_NVM_IPC_MOTOROM_POSITION 		"NVM.IPC:motor"
#define KEY_NVM_P2P_TOKEN 					"NVM.P2P:token"
#define KEY_NVM_CLOUD_DETECT_SwITCH 		"NVM.CLOUD:detect_swtich"
#define KEY_NVM_CLOUD_BABYCRY_SwITCH 		"NVM.CLOUD:babycry_switch"
#define KEY_NVM_CLOUD_INTERVAL 				"NVM.CLOUD:interval"
#define KEY_NVM_CLOUD_START_TIME 			"NVM.CLOUD:detect_start_time"
#define KEY_NVM_CLOUD_END_TIME 				"NVM.CLOUD:detect_end_time"
#define KEY_NVM_VIDEO_WDR 					"NVM.VIDEO:wdr"
#define KEY_NVM_VIDEO_FLIP 					"NVM.VIDEO:filp"
#define KEY_NVM_VIDEO_WATERMARK 			"NVM.VIDEO:watermark"
#define KEY_NVM_ALGO_SENSITIVITY 			"NVM.ALGO:sensitivity"
#define KEY_NVM_ALGO_TRACK_SWITCH 			"NVM.ALGO:track_switch"
#define KEY_NVM_ALGO_IRCUT_MODE 			"NVM.ALGO:night_mode"
#define KEY_NVM_WIFI_SSID 					"NVM.BIND:wifi_ssid"
#define KEY_NVM_WIFI_PWD 					"NVM.BIND:wifi_pwd"
#define KEY_NVM_TIMEZONE 					"NVM.BIND:timezone"
#define KEY_NVM_SD_STROGE_MODE 				"NVM.SD:storage_mode"

#define KEY_RAM_MOTOROM_RESET 				"RAM.RUN:motor_reset"

#define KEY_MEM_RUNLEVEL 					"MEM.runlevel"

enum db_type {
	DB_ROM = 0,
	DB_NVM,
	DB_RAM,
	DB_MEM,
};

typedef struct 
{
	char *key;
	char *value;
	char *description;
} db_info_keys_t;

typedef struct 
{
	dictionary 	*db;
	char 		*path;
	char 		*name;
	unsigned	hash;
	bool		sync_flag;
} db_name_t;

int imi_db_mi_init();
int imi_db_mi_get_value(char *key,void* out_value,int len);
int imi_db_mi_set_value(char *key,void* in_value,bool flag);
int imi_db_mi_deinit();


#endif

