#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include "cJSON.h"

static void process_forecast_location(cJSON *root)
{
	cJSON *location_obj = cJSON_GetObjectItem(root, "location");
	cJSON *tz_id_obj = cJSON_GetObjectItem(location_obj, "tz_id");
	cJSON *localtime_epoch_obj = cJSON_GetObjectItem(location_obj, "localtime_epoch");
	cJSON *localtime_obj = cJSON_GetObjectItem(location_obj, "localtime");

	if(cJSON_IsString(tz_id_obj)) { printf("tz_id: %s\n", tz_id_obj->valuestring); }
	if(cJSON_IsNumber(localtime_epoch_obj)) { printf("localtime_epoch: %d\n", localtime_epoch_obj->valueint); }
	if(cJSON_IsString(localtime_obj)) { printf("localtime: %s\n", localtime_obj->valuestring); }
}

static void process_forecast_current(cJSON *root)
{
	//cJSON *current_obj = cJSON_GetObjectItem(root, "current");
}

static void process_forecast_forecast(cJSON *root)
{
	//cJSON *forecast_obj = cJSON_GetObjectItem(root, "forecast");
}

void process_forecast(char *json)
{
	cJSON *root = cJSON_Parse(json);
	if(!root) { return; }
	process_forecast_location(root);
	process_forecast_current(root);
	process_forecast_forecast(root);
	//char *fjson = cJSON_Print(root);
	//printf("%s\n", fjson);
	cJSON_Delete(root);
}

void print_reformatted_json(char *json)
{
	cJSON *root = cJSON_Parse(json);
	if(!root) { return; }
	char *fjson = cJSON_Print(root);
	printf("%s\n", fjson);
	cJSON_Delete(root);
}
