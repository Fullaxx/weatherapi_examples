#!/bin/bash

if [ -z "${APIKEY}" ]; then
  if [ -r myapikey.sh ]; then
    source myapikey.sh
  fi
fi

if [ -z "${APIKEY}" ]; then
  echo "APIKEY must be set in order to use this script!"
  exit 1
fi

if [ -z "${LOCATION}" ]; then
  echo "LOCATION must be set in order to use this script!"
  exit 2
fi

DATE=`date "+%Y-%m-%d"`
curl -s "http://api.weatherapi.com/v1/astronomy.json?key=${APIKEY}&q=${LOCATION}&dt=${DATE}"
echo
