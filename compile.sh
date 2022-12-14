#!/bin/bash

set -e

# -std=c99
CFLAGS="-Wall -ansi"
CFLAGS+=" -DUSE_GETLINE"
OPTCFLAGS="${CFLAGS} -O2"
DBGCFLAGS="${CFLAGS} -ggdb3 -DDEBUG"

rm -f *.exe *.dbg

ACTION="-DDO_REFORMAT"
gcc ${OPTCFLAGS}         ${ACTION} json_reader.c wa_json.c cJSON.c -o json_formatter.exe
gcc ${DBGCFLAGS}         ${ACTION} json_reader.c wa_json.c cJSON.c -o json_formatter.dbg
gcc ${OPTCFLAGS} -static ${ACTION} json_reader.c wa_json.c cJSON.c -o json_formatter.static.exe

ACTION="-DDO_FORECAST"
gcc ${OPTCFLAGS}         ${ACTION} json_reader.c wa_json.c cJSON.c -o process_forecast.exe
gcc ${DBGCFLAGS}         ${ACTION} json_reader.c wa_json.c cJSON.c -o process_forecast.dbg
gcc ${OPTCFLAGS} -static ${ACTION} json_reader.c wa_json.c cJSON.c -o process_forecast.static.exe

CURLCFLAGS=`curl-config --cflags`
CURLLIBS=`curl-config --libs`
gcc ${OPTCFLAGS} ${CURLCFLAGS} get_forecast.c getopts.c curl_ops.c ${CURLLIBS} -o get_forecast.exe
gcc ${DBGCFLAGS} ${CURLCFLAGS} get_forecast.c getopts.c curl_ops.c ${CURLLIBS} -o get_forecast.dbg

strip *.exe
