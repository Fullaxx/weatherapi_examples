# WeatherAPI.com Usage
Example code for using the [weatherapi.com](https://www.weatherapi.com/) service. \
API Documentation can be found [here](https://www.weatherapi.com/docs/).

## Requirements for C Code
First we need to make sure we have all the appropriate libraries. \
Please consult this chart for help with installing the required packages. \
If your OS is not listed, please help us fill out the table, or submit a request via github.

| OS     | Commands (as root)                                               |
| ------ | ---------------------------------------------------------------- |
| Alpine | `apk add build-base bash gnutls`                                 |
| Debian | `apt update; apt install -y build-essential libcurl4-gnutls-dev` |
| Ubuntu | `apt update; apt install -y build-essential libcurl4-gnutls-dev` |

## Examples using bash scripting
```
export APIKEY="<YOURAPIKEY>"
export LOCATION="new-york-new-york-united-states-of-america"
./bash_current.sh
./bash_forecast.sh
./bash_search.sh
./bash_astronomy.sh
./bash_timezone.sh
./bash_sports.sh
```

## Searching for locations
```
export APIKEY="<YOURAPIKEY>"
./search_IATA.sh
```

## Processing weather data in C Code
```
./compile.sh

export APIKEY="<YOURAPIKEY>"
export LOCATION="los-angeles-california-united-states-of-america"
./bash_forecast.sh | ./json_formatter.exe | less
./bash_forecast.sh | ./process_forecast.exe
```
