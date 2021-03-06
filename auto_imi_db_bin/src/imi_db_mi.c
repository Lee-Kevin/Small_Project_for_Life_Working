/*-------------------------------------------------------------------------*/
/**
   @file    imi_db_mi.c
   @author  Kevin.lee
   @date 	2021-02-25
   @brief   database api for mi ipc 
*/
/*--------------------------------------------------------------------------*/

#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>   
#include <fcntl.h>
#include "iniparser.h"
#include "imi_db_mi.h"
#include "miio_timer.h"

#define log_debug printf
#define log_err	  printf

static dictionary * nvm_ini 	= NULL;
static dictionary * ram_ini 	= NULL;
static dictionary * rom_ini 	= NULL;
static dictionary * mem_ini 	= NULL;

#define PATH_MAX					256
#define NVM_DB_PATH						"./new.ini"
//#define DB_PATH					"/mnt/data/.imi_db.ini"
#define RAM_INI_PATH				"./run.ini"
//#define RUN_INI_PATH				"/var/run/run.ini"
#define SYNC_DATABASE_TIME_INTERVAL	60
#define FACTORY_PARTITION 			"factory"
#define KEY_OFFSET					4
static imi_node_t db_timer;

static db_name_t db_name[] = 
{
	[DB_ROM] = {
		.db			=NULL,
		.path		=NULL,
		.name		="ROM",
		.hash		=0,
		.sync_flag	=0,
	},
	[DB_NVM] = {
		.db			=NULL,
		.path		=NVM_DB_PATH,
		.name		="NVM",
		.hash		=0,
		.sync_flag	=0,
	},
	[DB_RAM] = {
		.db			=NULL,
		.path		=RAM_INI_PATH,
		.name		="RAM",
		.hash		=0,
		.sync_flag	=0,
	},
	[DB_MEM] = {
		.db			=NULL,
		.path		=NULL,
		.name		="MEM",
		.hash		=0,
		.sync_flag	=0,
	},
};
static db_info_keys_t db_keys[] = 
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

static dictionary *imi_db_mi_create_default_db(int index);
static int imi_db_mi_save_db_file();
static int  imi_db_timeout_sync_db() ;
static dictionary * factory_info_load();
static unsigned string_hash(char * key,int len);
static int file_lock(FILE *fd);
static int file_unlock(FILE *fd);

int imi_db_mi_init() 
{
	int ret = 0;
	int db_num = sizeof(db_name)/sizeof(db_name_t);
	for (int i=0; i<db_num; i++) {
		db_name[i].hash = string_hash(db_name[i].name,3);
	}
	for (int i=0; i<db_num; i++) {
		db_name[i].db = imi_db_mi_create_default_db(i);
		if(db_name[i].db == NULL) {
			log_err("[%s %d] Something error to create %s DB",__func__,__LINE__,db_name[i].name);
			return -1;
		}
	}
	db_timer = timer_init();
	create_event_timer(db_timer,SYNC_DATA_TIMEOUT,TIMER_MODE_LOOP,SYNC_DATABASE_TIME_INTERVAL,imi_db_timeout_sync_db);
	return ret;
}

int imi_db_mi_get_value(char* key,void* out_value,int len) 
{
	char *s = NULL;
	int i = 0;
	int db_num = sizeof(db_name)/sizeof(db_name_t);
	for (i=0; i<db_num; i++) {
		if (db_name[i].hash == string_hash(key,3)) {
			s = iniparser_getstring(db_name[i].db, key+KEY_OFFSET, NULL);
			if (s == NULL) {
				return RET_NO_VALUE_ERR;
			}
			if (strlen(s) > len) {
				return RET_LENGTH_ERR;
			}
			memcpy(out_value,s,strlen(s));
			return RET_OK;
		}
	}
	if (i >= db_num) {
		return RET_NO_VALUE_ERR;
	}
}

int imi_db_mi_set_value(char* key,void* in_value,bool flag) 
{
	int i = 0, ret = -1;
	int db_num = sizeof(db_name)/sizeof(db_name_t);
	for (i=0; i<db_num; i++) {
		if (db_name[i].hash == string_hash(key,3)) {
			if (i == DB_ROM) {
				return RET_READONLY_ERR;
			}
			db_name[i].sync_flag = true;
			ret = iniparser_set(db_name[i].db, key+KEY_OFFSET, in_value);
			break;
		}
	}
	if (i >= db_num) {
		return RET_NO_VALUE_ERR;
	}
	log_debug("[%s %d] imi_db_mi_save_db_file flag = %d \n",__func__,__LINE__,flag);
	if (flag == true) {
		ret = imi_db_mi_save_db_file();
		log_debug("[%s %d] imi_db_mi_save_db_file %d \n",__func__,__LINE__,ret);
	}
	return ret;
}

static int  imi_db_timeout_sync_db() 
{
	log_debug("[%s %d] imi_db_timeout_sync_db \n",__func__,__LINE__);
	imi_db_mi_save_db_file();
}

int imi_db_mi_deinit(void) 
{
	int db_num = sizeof(db_name)/sizeof(db_name_t);
	if (db_timer != NULL) {
		delete_all_timer(db_timer);
	}
	for (int i=0; i<db_num; i++) {
		if (db_name[i].db != NULL) {
			iniparser_freedict(db_name[i].db);
		}
	}
	
}

static int imi_db_mi_save_db_file() 
{
	int db_num = sizeof(db_name)/sizeof(db_name_t);
	for (int i=0; i<db_num; i++) {
		if (db_name[i].db == NULL || db_name[i].path == NULL) {
			continue;
		}
		if(db_name[i].sync_flag == true) {
			FILE *fd = fopen(db_name[i].path,"rt+");
			if(fd == NULL) {
				log_err("[%s %d]: Can't open the %s\n",__func__,__LINE__,db_name[i].path);
				return -1;
			}
			file_lock(fd);
			iniparser_dump_ini(db_name[i].db,fd);
			log_debug("[%s %d] save %s\n",__func__,__LINE__,db_name[i].path);
			file_unlock(fd);
			fclose(fd);
			//sync();
			db_name[i].sync_flag = false;
		}
	}
	return 0;
}

static dictionary * imi_db_mi_create_default_db(int index) 
{
	dictionary * ini = NULL;
	FILE    *fd ;
	char 	string[1024] = {0};
	int key_num 	 = sizeof(db_keys)/sizeof(db_info_keys_t);
	switch (index) {
		case DB_ROM:
			ini = factory_info_load();
			break;
		case DB_MEM:
			ini = dictionary_new(0);
			for (int i=0; i<key_num; i++) {
				if (db_name[DB_MEM].hash == string_hash(db_keys[i].key,3)) {
					dictionary_set(ini,db_keys[i].key+4,db_keys[i].value);
				}
			}
			break;
		case DB_NVM:
		case DB_RAM:
			if(db_name[index].path) {
				ini = iniparser_load(db_name[index].path);
				if(ini == NULL) {
					log_debug("[%s %d] ready to create_default_db_file %s\n",__func__,__LINE__,db_name[index].name);
					if ((fd=fopen(db_name[index].path, "w")) == NULL) {
						fprintf(stderr, "iniparser: cannot create %s\n",db_name[index].path);
						return NULL;
					}
					file_lock(fd);
					for (int i=0; i<key_num; i++) {
						char section[1024] 	= {0};
						char key[1024]		= {0};
						static unsigned int sect_hash = 0;
						if (db_name[index].hash == string_hash(db_keys[i].key,3)) {
							bzero(string,sizeof(string));
							strncpy(string,db_keys[i].key+4,strlen(db_keys[i].key+4));
							sscanf(string,"%[^:]:%[^:]",section,key);
							if (sect_hash != string_hash(section,strlen(section))) {
								fprintf(fd,"\n");
								fprintf(fd,"\n");
								fprintf(fd,"[%s]\n",section);
								sect_hash = string_hash(section,strlen(section));
							}
							log_debug("[%s %d] section:%s name=%s   key=%s value=%s\n",__func__,__LINE__,section,db_name[index].name,key,db_keys[i].value);
							fprintf(fd,"%s\t=	%s \n",key,db_keys[i].value);
						}
					}
					file_unlock(fd);
					fclose(fd);
			
					ini = iniparser_load(db_name[index].path);
					if (ini == NULL) {
						log_err("[%s %d] can't load db file again \n",__func__,__LINE__);
						return NULL;
					}
				}
			} else {
			}
			break;
		default:
			break;
	}

	return ini;
}


/*
	This is the function deal with factory partition
*/

static int string_find(char *psrc, int len, char *pdst)
{
	int i, j;
	for (i=0; i < len; i++)
	{
		if(psrc[i]!=pdst[0])
			continue;		
		j = 0;
		while(pdst[j]!='\0' && psrc[i+j]!='\0')
		{
			j++;
			if(pdst[j]!=psrc[i+j])
			break;
		}
		if(pdst[j]=='\0')
			return i;
	}
	return -1;
}

static int string_key_value_get(char *psrc, int len, char *pdst,char * out, int out_len)
{
	int i = 0;
	int index = string_find(psrc,len,pdst);
	if (-1 == index || NULL == out) {
		return -1;
	}
	index =strlen(pdst)+1+index;
	for (i=0; i<out_len; i++) {
		out[i] = psrc[index+i];
		if(psrc[index+i] == '\0') {
			break;
		}
	}
	if (i==out_len) {
		log_err("[%s %d] out_len is too short",__func__,__LINE__);
		return -2;
	}
	return i;
}

static char *nvram_find_mtd(char *name, int *part_size)
{
	char dev[PATH_MAX] = {0x00};
	char *path = NULL;
	int i=0;
	struct stat s;
	
	FILE *fp = fopen("/proc/mtd", "r");
	if (!fp) {
		return NULL;
	}

	while( fgets(dev, sizeof(dev), fp) ) {
		if (strcasestr(dev, name) &&
			sscanf(dev, "mtd%d: %08x", &i, part_size) ) {
			snprintf(dev, sizeof(dev), "/dev/mtdblock%d", i);
			if ((stat(dev, &s) == 0) && (s.st_mode & S_IFBLK) ) {
				path = (char *)malloc(strlen(dev)+1);
				strncpy(path, dev, strlen(dev)+1);
				break;
			}
		}
	}
	fclose(fp);
	return path;
}

static dictionary * factory_info_load() 
{
	FILE *fd = NULL;
	int size = 0;
	char tmp_out[1024] = {0};
	dictionary *ini = NULL;
	int key_num = sizeof(db_keys)/sizeof(db_info_keys_t);
	//char *factory = nvram_find_mtd(FACTORY_PARTITION, &size);
	char *factory = "./factory.txt";
	size = 64*1024;
	if (factory == NULL) {
		return ini;
	}
	char *buf = (char *)malloc(size);
	if(buf == NULL) {
		free(factory);
		return ini;
	}
	fd = fopen(factory,"r+");
	if(fd == NULL) {
	//	free(factory);
		free(buf);
		return ini;
	}
	ini = dictionary_new(0);
	fgets(buf,size,fd);
	for (int i=0; i<key_num; i++) {
		if (db_name[DB_ROM].hash == string_hash(db_keys[i].key,3)) {
			bzero(tmp_out,sizeof(tmp_out));
			if (string_key_value_get(buf,size,db_keys[i].key+4,tmp_out,sizeof(tmp_out)) > 0 ) {
				log_debug("[%s %d] key=%s \t value=%s\n",__func__,__LINE__,db_keys[i].key+4,tmp_out);
				dictionary_set(ini,db_keys[i].key+4,tmp_out);

			} else {
				log_err("[%s %d]: can't find %s from factory \n",__func__,__LINE__,db_keys[i].key+4);
			}
		}
	}

//	free(factory);
	free(buf);
	return ini;
}

/**
  @brief    Compute the hash key for a string.
  @param    key     Character string to use for key.
  @return   1 unsigned int on at least 32 bits.

  This hash function has been taken from an Article in Dr Dobbs Journal.
  This is normally a collision-free function, distributing keys evenly.
  The key is stored anyway in the struct so that collision can be avoided
  by comparing the key itself in last resort.
 */
/*--------------------------------------------------------------------------*/
static unsigned string_hash(char * key,int len)
{
    unsigned    hash ;
    size_t      i ;

    if (!key)
        return 0 ;
    for (hash=0, i=0 ; i<len ; i++) {
        hash += (unsigned)key[i] ;
        hash += (hash<<10);
        hash ^= (hash>>6) ;
    }
    hash += (hash <<3);
    hash ^= (hash >>11);
    hash += (hash <<15);
    return hash ;
}

static int file_lock(FILE *fd) 
{
	if ( fd != NULL ) { 
		if(flock(fileno(fd), LOCK_EX) == 0) {
			log_debug("[%s %d] locked successful\n",__func__,__LINE__);
		} else {
			log_debug("[%s %d] locked failed\n",__func__,__LINE__);
			return -1;
		}
	}   
	return 0; 
}

static int file_unlock(FILE *fd) 
{
	if ( fd != NULL ) { 
		if(flock(fileno(fd), LOCK_UN) == 0) {
			log_debug("[%s %d] unlock successful\n",__func__,__LINE__);
		} else {
			log_debug("[%s %d] unlock failed\n",__func__,__LINE__);
			return -1;
		}
	}   
	return 0;
}


