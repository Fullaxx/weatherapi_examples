# weatherapi_examples
Example code for using the [weatherapi.com](https://www.weatherapi.com/) service

## Bash Examples
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

## Reformatting the JSON output
```
./compile.sh

export APIKEY="<YOURAPIKEY>"
export LOCATION="los-angeles-california-united-states-of-america"
./bash_forecast.sh | ./json_formatter.exe | less
```
