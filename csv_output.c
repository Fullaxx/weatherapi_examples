/*
	create a CSV of forecasted temperatures for a given location
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

#include "cJSON.h"

#define GETTEMP(f) (f ? "temp_f" : "temp_c")

static void print_hourly_temps(cJSON *hourly_array, int is_f)
{
	int i;
	cJSON *obj;
	cJSON *time_obj;
	cJSON *temp_obj;

	if(!cJSON_IsArray(hourly_array)) { return; }
	int n = cJSON_GetArraySize(hourly_array);
	for(i=0; i<n; i++) {
		obj = cJSON_GetArrayItem(hourly_array, i);
		time_obj = cJSON_GetObjectItem(obj, "time_epoch");
		temp_obj = cJSON_GetObjectItem(obj, GETTEMP(is_f));
		if(cJSON_IsNumber(time_obj) && cJSON_IsNumber(temp_obj)) {
			printf("%d,%d\n", time_obj->valueint, temp_obj->valueint);
		}
	}
}

static void print_forecast_temps(cJSON *root, int is_f)
{
	int i;
	cJSON *obj;
	cJSON *hourly_array;

	cJSON *forecast_obj = cJSON_GetObjectItem(root, "forecast");
	cJSON *forecastday_array = cJSON_GetObjectItem(forecast_obj, "forecastday");
	if(!cJSON_IsArray(forecastday_array)) { return; }
	int n = cJSON_GetArraySize(forecastday_array);
	for(i=0; i<n; i++) {
		obj = cJSON_GetArrayItem(forecastday_array, i);
		/*cJSON *date_obj = cJSON_GetObjectItem(obj, "date");
		printf("date: %s\n", date_obj->valuestring);*/
		hourly_array = cJSON_GetObjectItem(obj, "hour");
		print_hourly_temps(hourly_array, is_f);
	}
}

/*static void print_current_temp(cJSON *root)
{
	cJSON *current_obj = cJSON_GetObjectItem(root, "current");
	cJSON *time_obj = cJSON_GetObjectItem(current_obj, "last_updated_epoch");
	cJSON *temp_obj = cJSON_GetObjectItem(current_obj, GETTEMP);
	if(cJSON_IsNumber(time_obj) && cJSON_IsNumber(temp_obj)) {
		printf("%d,%d\n", time_obj->valueint, temp_obj->valueint);
	}
}*/

void print_temp_csv(char *json, int is_f)
{
	cJSON *root = cJSON_Parse(json);
	if(!root) { return; }
	printf("TIME,TEMP\n");
	/*print_current_temp(root);*/
	print_forecast_temps(root, is_f);
	cJSON_Delete(root);
}
