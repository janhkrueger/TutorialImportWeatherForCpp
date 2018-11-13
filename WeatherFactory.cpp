/*
 * WeatherFactory.cpp
 *
 *  Created on: 16.01.2018
 *  Author: janhkrueger
 *  Copyright 2018 janhkrueger
 */

#include "WeatherFactory.h"
#include <pqxx/pqxx>
#include <string>
#include <stdexcept>
#include <cmath>
#include <iostream>

namespace insulae {

/*
 * Konstruktur.
 */
WeatherFactory::~WeatherFactory() {
  // TODO(janhkrueger) Auto-generated destructor stub
}


Weather WeatherFactory::create(unsigned int karteid, pqxx::connection &conn) {
  // Wetterlocation der Wunschkarte holen
  std::string sql = "SELECT karte_wetter FROM welt.karte_name " \
                    "WHERE karte_id = " + std::to_string(karteid);
  pqxx::work w(conn);
  pqxx::result r = w.exec(sql);
  unsigned int locationID;
  if (r.size() == 1) {
    locationID = r[0]["karte_wetter"].as<unsigned int>();
  } else {
    throw std::invalid_argument("There is no weatherlocation with this ID");
  }
  insulae::Weather wetter(locationID);
  sql = "SELECT * FROM welt.weatherbasics WHERE id = " + std::to_string(
          locationID) + " ORDER BY surveydate DESC LIMIT 1;";
  r = w.exec(sql);
  // Sollte keine Stadt mit dieser ID existieren, Exception werfen
  if (r.size() == 1) {
    wetter.setLocationID(r[0]["id"].as<unsigned int>());
    wetter.setLocationName(r[0]["locationname"].as<std::string>());
    wetter.setTemperature(r[0]["temperature"].as<double>());
    wetter.setHumidity(r[0]["humidity"].as<unsigned int>());
    wetter.setWindSpeed(r[0]["windspeed"].as<double>());
    wetter.setVisibility(r[0]["visibility"].as<unsigned int>());
    r.clear();
    return wetter;
  } else {
    throw std::invalid_argument("There is no weatherlocation with this ID");
  }
}

void WeatherFactory::DisposeObject() {
  delete this;
}
}  // namespace insulae
