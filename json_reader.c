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

#ifdef USE_GETLINE
#define _GNU_SOURCE
#else
#define _XOPEN_SOURCE 700
#error cant use fgets() without determining a buffer size
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "wa_json.h"

static void chomp(char *buf)
{
	int l;
	if(!buf) { return; }
	l = strlen(buf)-1;
	if(l >= 0) {
		if(buf[l] == '\n') { buf[l] = '\0'; }
		if(buf[l] == '\r') { buf[l] = '\0'; }
	}
}

static void handle_input(char *input)
{
	/*printf("%s\n", input);*/

#ifdef DO_REFORMAT
	print_reformatted_json(input);
#endif

#ifdef DO_FORECAST
	process_forecast(input);
#endif
}

static void process_stdin(void)
{
#ifdef USE_GETLINE
	size_t n;
	ssize_t bytes;
#else
	char *retval;
	char buf[8192];
#endif

	char *line = NULL;
	FILE *input = stdin;

	while(!feof(input)) {
#ifdef USE_GETLINE
		n=0; bytes = getline(&line, &n, input);
		if(bytes < 0) { break; }
#else
		memset(&buf[0], 0, sizeof(buf));
		retval = fgets(buf, sizeof(buf), input);
		if(!retval) { break; }
		line = strdup(buf);
#endif

		chomp(line);
		handle_input(line);
		free(line);
		line = NULL;
	}

	if(line) { free(line); }
}

static void process_file(char *filename)
{
	printf("Processing %s ...\n", filename);
}

/*	If we have nothing to process on the command line,
	Then process json from stdin
*/
int main(int argc, char *argv[])
{
	if(argc == 1) {
		process_stdin();
	} else {
		while(--argc > 0) { process_file(argv[argc]); }
	}

	return 0;
}
