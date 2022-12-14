/*
	A command-line utility to process JSON weather data
	Copyright (C) 2022 Brett Kuskie <fullaxx@gmail.com>

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

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
	cJSON *current_obj = cJSON_GetObjectItem(root, "current");
	cJSON *temp_f_obj = cJSON_GetObjectItem(current_obj, "temp_f");
	if(cJSON_IsNumber(temp_f_obj)) { printf("temp_f: %d\n", temp_f_obj->valueint); }
}

static void process_forecast_forecast(cJSON *root)
{
/*
	cJSON *forecast_obj = cJSON_GetObjectItem(root, "forecast");
*/
}

void process_forecast(char *json)
{
	cJSON *root = cJSON_Parse(json);
	if(!root) { return; }
	process_forecast_location(root);
	process_forecast_current(root);
	process_forecast_forecast(root);
/*
	char *fjson = cJSON_Print(root);
	printf("%s\n", fjson);
*/
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
