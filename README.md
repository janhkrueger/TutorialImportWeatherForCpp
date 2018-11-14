Import Weather in Insulae

## What you need
* A postgresql database
* the schemas "foundation" and "world"
* g++, make, curl, libcurl4-openssl-dev libpq-dev libpqxx-dev
* an OpenWeathermap.org api key




## Dependencies
* Tencent/rapidjson
* jpbarrette/curlpp
* gabime/spdlog


## Creating the relevant tables
ReadWeatherSources needs two tables. One in which is the definition about the places to watch for the weather and the other is for holding the current weather. The table definitions can be found in: db/createTables.sql
Have in mind that are to schemas, foundation and world, are a requirement.

Use your favored psql tool to create both tables. On the bash it can be something like this:

```
psql -U {user_name} -d {database_name} -f db/createTables.sql -h {host_name}
```

If everything is right there should now both tables in your database.


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
