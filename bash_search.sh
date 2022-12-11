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

curl -s "http://api.weatherapi.com/v1/search.json?key=${APIKEY}&q=${LOCATION}"
echo
