#!/bin/bash

set -e

CFLAGS="-Wall -ansi -std=c99"
CFLAGS+=" -DUSE_GETLINE"
OPTCFLAGS="${CFLAGS} -O2"
DBGCFLAGS="${CFLAGS} -ggdb3 -DDEBUG"

rm -f *.exe *.dbg

ACTION="-DDO_REFORMAT"
gcc ${OPTCFLAGS}         ${ACTION} json_main.c wa_json.c cJSON.c -o json_formatter.exe
gcc ${DBGCFLAGS}         ${ACTION} json_main.c wa_json.c cJSON.c -o json_formatter.dbg
gcc ${OPTCFLAGS} -static ${ACTION} json_main.c wa_json.c cJSON.c -o json_formatter.static.exe

ACTION="-DDO_FORECAST"
gcc ${OPTCFLAGS}         ${ACTION} json_main.c wa_json.c cJSON.c -o process_forecast.exe
gcc ${DBGCFLAGS}         ${ACTION} json_main.c wa_json.c cJSON.c -o process_forecast.dbg
gcc ${OPTCFLAGS} -static ${ACTION} json_main.c wa_json.c cJSON.c -o process_forecast.static.exe

strip *.exe
