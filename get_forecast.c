/*
	get_forecast is a command-line utility to retrieve JSON weather data
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

/* strdup() */
#define _XOPEN_SOURCE (700)	

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "getopts.h"
#include "curl_ops.h"

static void parse_args(int argc, char **argv);
void print_temp_csv(char *json, int is_f);

char *g_apikey = NULL;
char *g_location = NULL;
int g_fahrenheit = 0;

static int get_forecast_page(void)
{
	int retval = 0;
	char url[2048];
	curlresp_t resp;

	memset(&resp, 0, sizeof(curlresp_t));
	snprintf(&url[0], sizeof(url), "http://api.weatherapi.com/v1/forecast.json?key=%s&q=%s&days=3&aqi=yes&alerts=yes", g_apikey, g_location);

	int z = ws_curl_get(url, &resp);
	if(z) {
		fprintf(stderr, "ws_curl_get() failed!\n");
		retval = 3;
	} else {
		if(resp.http_code != 200) {
			fprintf(stderr, "HTTP Error %ld: %s\n", resp.http_code, resp.page);
			retval = 4;
		} else {
			/*printf("%s\n", resp.page); */
			print_temp_csv(resp.page, g_fahrenheit);
		}
	}

	if(resp.page) { free(resp.page); }
	return retval;
}

int main(int argc, char *argv[])
{
	int retval;

	parse_args(argc, argv);
	retval = get_forecast_page();

	if(g_apikey) { free(g_apikey); }
	if(g_location) { free(g_location); }
	return retval;
}

struct options opts[] = 
{
	{ 1, "apikey",		"APIKEY to use",				"K",  1 },
	{ 2, "location",	"LOCATION to use",				"L",  1 },
	{ 3, "fahrenheit",	"Display Temps in Fahrenheit",	"f",  0 },
	{ 0, NULL,			NULL,							NULL, 0 }
};

static void parse_args(int argc, char **argv)
{
	int c;
	char *args;

	while ((c = getopts(argc, argv, opts, &args)) != 0) {
		switch(c) {
			case -2:
				/* Special Case: Recognize options that we didn't set above. */
				fprintf(stderr, "Unknown Getopts Option: %s\n", args);
				break;
			case -1:
				/* Special Case: getopts() can't allocate memory. */
				fprintf(stderr, "Unable to allocate memory for getopts().\n");
				exit(EXIT_FAILURE);
				break;
			case 1:
				g_apikey = strdup(args);
				break;
			case 2:
				g_location = strdup(args);
				break;
			case 3:
				g_fahrenheit = 1;
				break;
			default:
				fprintf(stderr, "Unexpected getopts Error! (%d)\n", c);
				break;
		}

		/* This free() is required since getopts() automagically allocates space for "args" everytime it's called. */
		free(args);
	}

	if(!g_apikey) {
		char *apikey = getenv("APIKEY");
		if(apikey) { g_apikey = strdup(apikey); }
		else { fprintf(stderr, "APIKEY not found! Add APIKEY to environment or use -K\n"); exit(EXIT_FAILURE); }
	}

	if(!g_location) {
		char *location = getenv("LOCATION");
		if(location) { g_location = strdup(location); }
		else { fprintf(stderr, "LOCATION not found! Add LOCATION to environment or use -L\n"); exit(EXIT_FAILURE); }
	}
}
