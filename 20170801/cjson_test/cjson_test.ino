
#include "cjson.h"

char *str = "{\"name\":\"Mike\",\"age\":24,\"data\":[48,49,50,51,52]}";

uint32_t count = 0;
bool flag = false;

void setup()
{
	SerialUSB.begin(115200);
	pinMode(13, OUTPUT);
}

void loop()
{
	cJSON *pJson = cJSON_Parse(str);
	
	if(NULL == pJson)
	{
		SerialUSB.println("parse fail");
	}
	else
	{
		int RootArrayNum = cJSON_GetArraySize(pJson);
		SerialUSB.print("RootArrayNum: ");
		SerialUSB.println(RootArrayNum);
		
		cJSON *pName = cJSON_GetObjectItem(pJson, "name");
		if(pName != NULL)
		{
			SerialUSB.print("Name: ");
			SerialUSB.println(pName->valuestring);
		}
		
		cJSON *pAge = cJSON_GetObjectItem(pJson, "age");
		if(pAge != NULL)
		{
			SerialUSB.print("Age: ");
			SerialUSB.println(pAge->valueint);
		}
		
		cJSON *pData = cJSON_GetObjectItem(pJson, "data");
		if(pData != NULL)
		{
			uint8_t num = cJSON_GetArraySize(pData);
			SerialUSB.println("Data: ");
			for(uint8_t i = 0; i < num; i ++)
			{
				cJSON *pValue = cJSON_GetArrayItem(pData, i);
				SerialUSB.println(pValue -> valueint);
			}
		}
	}
	cJSON_Delete(pJson);
	
	// SerialUSB.println(count ++);
	flag = !flag;
	digitalWrite(13, flag);
	delay(1000);
}