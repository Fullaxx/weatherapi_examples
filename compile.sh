#!/bin/bash

set -e

CFLAGS="-Wall -ansi -std=c99"
CFLAGS+=" -DUSE_GETLINE"
OPTCFLAGS="${CFLAGS} -O2"
DBGCFLAGS="${CFLAGS} -ggdb3 -DDEBUG"

rm -f *.exe *.dbg

gcc ${OPTCFLAGS}         json_formatter.c wa_json.c cJSON.c -o json_formatter.exe
gcc ${DBGCFLAGS}         json_formatter.c wa_json.c cJSON.c -o json_formatter.dbg
gcc ${OPTCFLAGS} -static json_formatter.c wa_json.c cJSON.c -o json_formatter.static.exe

strip *.exe
