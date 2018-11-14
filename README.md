# Import Weather in Insulae

## What you need
* A postgresql database
* the schemas "foundation" and "world"
* g++, make, curl, libcurl4-openssl-dev libpq-dev libpqxx-dev
* an OpenWeathermap.org api key

## Dependencies
* Tencent/rapidjson
* jpbarrette/curlpp
* gabime/spdlog

## Creating relevant subfolders
```
mkdir bin
mkdir log
mkdir coverage
```


## Creating the relevant tables
ReadWeatherSources needs two tables. One in which is the definition about the places to watch for the weather and the other is for holding the current weather. The table definitions can be found in: db/createTables.sql
Have in mind that are to schemas, foundation and world, are a requirement.

Use your favored psql tool to create both tables. On the bash it can be something like this:

```
psql -U {user_name} -d {database_name} -f db/createTables.sql -h {host_name}
```

If everything is right there should now both tables in your database.

## Getting an OpenWeatherMap api key
To read data from the openweathermap.org api, we need to create an account and request an api key. Please refer to https://home.openweathermap.org/users/sign_up for the details.

## Adjust the config file
In the file "insulae.cfg" the central config is stored

```
dbname=[INSERT DATABASE NAME]
user=[INSERT DATABASE USER]
password=[INSERT DATABASE PASSWORD]
hostaddr=[INSERT THE HOST IP ADDRESS]
port=5432
logdir=log/
batchlogfile=batch.log
weatherjobname=ReadWeatherSource
appid=[INSERT YOUR OpenWeathermap API KEY]
```

Insert your values and your're set.

## Install OS dependencies
In order to run, we need some software components. On modern debian systems you can install them via apt. For other destribution please refer to the designated repostories.

```
apt-get install g++ make curl libcurl4-openssl-dev libpq-dev libpqxx-dev
```

## Intialise the submodules
We need the submodules before we can compile or even run the programm.

```
git submodue update --init
```
Please refer to the separate README files if and how to build the submodules.


## Compile the programm
Now we can compile everything. Simply run:

```
make
```

You should see the following output

```
export GLIBCXX_FORCE_NEW=true                                                                                                                                                                                                                                                                                                                                                               
g++ -std=c++17 -flto -pthread -D_FORTIFY_SOURCE=2 -pie -fPIE -ftrapv -Wl,-z,relro,-z,now -fpic -fvisibility=hidden -fstack-check -fstack-protector-all --param ssp-buffer-size=1 -fmessage-length=0 -O3 src/Batch.cpp src/Weather.cpp src/WeatherFactory.cpp src/ReadWeatherSource.cpp -lpqxx -lpq -Llib -lcurlpp -lcurl -Isrc -I/usr/include/postgresql -I frameworks/rapidjson/include/ -I
 frameworks/spdlog/include/ -I/usr/include -Iinclude -includelibpq-fe.h -o bin/ReadWeatherSource                                                                                                                                                                                                                                                                                            
export GLIBCXX_FORCE_NEW=false                                                                                                                                                                                                                                                                                                                                                              
```

The output is in the subfolder bin, named "ReadWeatherSource"

## Add a weathersource to watch
Now we are, theoretically, able to read current weather data and import it for further useage.
But we need a definition what places we want to want. So we insert a new place in the table foundation.weathersource

```
INSERT INTO weathersource (id, locationname, active) VALUES ('3413829', 'Reykjavik', '1');
```

## Running the program
Everythig is up, we can now run the programm for the first time.

```
bin/ReadWeatherSource
```

That's it. You now should have an entry in the table world.weather with the current data.




# Important warning
The error handling in this sample is reduced because my regular used routines from https://git.janhkrueger.de/insulae/insulae-private are still not open for the public. So no fancy error handling. Nonetheless, there should be no nard errors or atomar explosions.