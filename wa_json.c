#include <stdio.h>
//#include <stdlib.h>
//#include <string.h>

#include "cJSON.h"

void print_reformatted_json(char *json)
{
	cJSON *root = cJSON_Parse(json);
	char *fjson = cJSON_Print(root);
	printf("%s\n", fjson);
	cJSON_Delete(root);
}
