/*
	wa_get is a command-line utility to retrieve JSON weather data  
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
#include <curl/curl.h>

#include "curl_ops.h"

/* https://stackoverflow.com/questions/290996/http-status-code-with-libcurl */

static size_t save_response(void *ptr, size_t size, size_t nmemb, void *user_data)
{
	long incdatasize, newpagesize;
	curlresp_t *r = (curlresp_t *)user_data;

	/* realloc page location */
	incdatasize = size*nmemb;
	newpagesize = r->bytecount + incdatasize;
	r->page = realloc(r->page, newpagesize+1);	/* +1 = make room for a NULL byte */

	/* save curl data buffer */
	memcpy(r->page+(r->bytecount), ptr, incdatasize);
	r->page[newpagesize] = 0;	/* tack on NULL byte */

	/* save new page size */
	r->bytecount = newpagesize;

#ifdef CURL_PRINT_DATA_INCOMING
	printf("%s\n", r->page);
#endif

	return incdatasize;
}

int ws_curl_get(char *url, curlresp_t *r)
{
	CURL *ch;
	CURLcode res;
	int retval = 0;

	ch = curl_easy_init();
	if(!ch) {
		fprintf(stderr, "curl_easy_init() failed!\n");
		return 1;
	}

	curl_easy_setopt(ch, CURLOPT_URL, url);
	/* curl_easy_setopt(ch, CURLOPT_VERBOSE, 0L); default is 0 */
	curl_easy_setopt(ch, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(ch, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 0L);
	curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, save_response);
	curl_easy_setopt(ch, CURLOPT_WRITEDATA, r);
	/* CURLcode curl_easy_setopt(ch, CURLOPT_TIMEOUT, long timeout); */

	res = curl_easy_perform(ch);
	if(res == CURLE_OK) {
		curl_easy_getinfo(ch, CURLINFO_RESPONSE_CODE, &r->http_code);
	} else {
		retval = 2;
		fprintf(stderr, "curl_easy_perform() failed! %s\n", curl_easy_strerror(res));
		if(r->page) { free(r->page); r->page = NULL; }
	}

#ifdef CURL_PRINT_DATA_OUTPUT
	if(r->page) { printf("%s\n\n", r->page); }
#endif

	curl_easy_cleanup(ch);
	return retval;
}

#if 0
/* We do this so that libcurl does not output to stdout */
static size_t dump_data(void *ptr, size_t size, size_t nmemb, void *user_data)
{
	//user_t *opt = user_data;
	long bytes = size*nmemb;

	return bytes;
}
#endif

int ws_curl_post(char *url, curlresp_t *r, curlpost_t *p)
{
	CURL *ch;
	CURLcode res;
	int retval = 0;

	ch = curl_easy_init();
	if(!ch) {
		fprintf(stderr, "curl_easy_init() failed!\n");
		return 1;
	}

	curl_easy_setopt(ch, CURLOPT_URL, url);
	curl_easy_setopt(ch, CURLOPT_CUSTOMREQUEST, "POST");
	curl_easy_setopt(ch, CURLOPT_NOSIGNAL, 1L);
	curl_easy_setopt(ch, CURLOPT_NOPROGRESS, 1L);
	curl_easy_setopt(ch, CURLOPT_FOLLOWLOCATION, 0L);
	curl_easy_setopt(ch, CURLOPT_POSTFIELDSIZE, p->size);
	curl_easy_setopt(ch, CURLOPT_POSTFIELDS, p->data);
	curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, save_response);
	curl_easy_setopt(ch, CURLOPT_WRITEDATA, r);
/*
	curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, dump_data);
	curl_easy_setopt(ch, CURLOPT_WRITEDATA, opt);
	if(opt->headerlist) curl_easy_setopt(ch, CURLOPT_HTTPHEADER, opt->headerlist);
*/
	res = curl_easy_perform(ch);
	if(res == CURLE_OK) {
		curl_easy_getinfo(ch, CURLINFO_RESPONSE_CODE, &r->http_code);
	} else {
		retval = 2;
		fprintf(stderr, "curl_easy_perform() failed! %s\n", curl_easy_strerror(res));
		if(r->page) { free(r->page); r->page = NULL; }
	}

#ifdef CURL_PRINT_DATA_OUTPUT
	if(r->page) { printf("%s\n\n", r->page); }
#endif

	curl_easy_cleanup(ch);
	return retval;
}
