#!/bin/bash

set -e

# -std=c99
CFLAGS="-Wall -ansi"
CFLAGS+=" -DUSE_GETLINE"
OPTCFLAGS="${CFLAGS} -O2"
DBGCFLAGS="${CFLAGS} -ggdb3 -DDEBUG"

rm -f *.exe *.dbg

ACTION="-DDO_REFORMAT"
gcc ${OPTCFLAGS}         ${ACTION} json_reader.c json_output.c cJSON.c -o json_formatter.exe
gcc ${DBGCFLAGS}         ${ACTION} json_reader.c json_output.c cJSON.c -o json_formatter.dbg
gcc ${OPTCFLAGS} -static ${ACTION} json_reader.c json_output.c cJSON.c -o json_formatter.static.exe

ACTION="-DDO_FORECAST"
gcc ${OPTCFLAGS}         ${ACTION} json_reader.c json_output.c cJSON.c -o process_forecast.exe
gcc ${DBGCFLAGS}         ${ACTION} json_reader.c json_output.c cJSON.c -o process_forecast.dbg
gcc ${OPTCFLAGS} -static ${ACTION} json_reader.c json_output.c cJSON.c -o process_forecast.static.exe

CURLCFLAGS=`curl-config --cflags`
CURLLIBS=`curl-config --libs`
gcc ${OPTCFLAGS} ${CURLCFLAGS} get_forecast.c getopts.c curl_ops.c cJSON.c csv_output.c ${CURLLIBS} -o forecast2csv.exe
gcc ${DBGCFLAGS} ${CURLCFLAGS} get_forecast.c getopts.c curl_ops.c cJSON.c csv_output.c ${CURLLIBS} -o forecast2csv.dbg

strip *.exe
