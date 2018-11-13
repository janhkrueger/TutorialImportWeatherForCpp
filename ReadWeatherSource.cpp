//  Copyright janhkrueger 2018.
#include <unistd.h>
#include <stdlib.h>     /* atoi */
#include <pqxx/pqxx>
#include <cstdio>
#include <string>
#include <stdexcept>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include "Batch.h"
#include "Weather.h"
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/filereadstream.h"
#include "rapidjson/pointer.h"
#include "spdlog/spdlog.h"
#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>
#include <curlpp/Exception.hpp>
#include <curlpp/Infos.hpp>


inline const rapidjson::Value* find_ptr(const rapidjson::Value& v,
                                        const char* k) {
  if (!v.IsObject())
    return NULL;
  if (const char* dot = strchr(k, '.')) {
    auto p = find_ptr(v, std::string(k, dot - k).c_str());
    return p && p->IsObject() ? find_ptr(*p, dot + 1) : NULL;
  }
  auto p = v.FindMember(k);
  return p == v.MemberEnd() ? NULL : &p->value;
}

inline const char* get_string(const rapidjson::Value& v, const char* k,
                              std::string histeintrag) {
  std::string str = k;
  auto p = find_ptr(v, k);
  if ( p && p->IsString() ) {
    return p->GetString();
  } else {
    spdlog::get(histeintrag)->info("  Could not read element: " + str);
    return 0;
  }
}

inline int get_int(const rapidjson::Value& v, const char* k,
                   std::string histeintrag) {
  std::string str = k;
  auto p = find_ptr(v, k);
  if ( p && p->IsInt() ) {
    return p->GetInt();
  } else {
    spdlog::get(histeintrag)->info("  Could not read element: " + str);
    return 0;
  }
}

inline double get_double(const rapidjson::Value& v, const char* k,
                         std::string histeintrag) {
  std::string str = k;
  auto p = find_ptr(v, k);
  if ( (p != 0) && (p->IsDouble() || p->IsInt()) ) {
    return p->GetDouble();
  } else {
    spdlog::get(histeintrag)->info("  Could not read element: " + str);
    return 0.00;
  }
}


std::string get_SubDoc(rapidjson::Document& dTown, std::string objectName,
                       std::string histeintrag) {
  try {
    if ( dTown[ objectName.c_str() ].IsObject() ) {
      rapidjson::StringBuffer sb;
      rapidjson::Writer<rapidjson::StringBuffer> writer(sb);
      dTown[objectName.c_str()].Accept(writer);
      return sb.GetString();
    } else {
      spdlog::get(histeintrag)->info("Could not read objectValue: "
                                     + objectName);
      return "";
    }
  } catch (std::exception& e) {
    std::cerr << "Exception catched : " << e.what() << std::endl;
    return "";
  }
}

// Getting a nested float Value, return 0.0 if not found
double readNestedValue(rapidjson::Document& dTown, std::string objectName,
                       std::string elementName, std::string histeintrag) {
  // get the subobject of wind
  if ( dTown[ objectName.c_str() ].IsObject() ) {
    rapidjson::Document document;
    document.Parse(get_SubDoc(dTown, objectName.c_str(),
                              histeintrag).c_str());
    if (document.HasParseError()) {
      return 0.0;
    }
    return get_double(document, elementName.c_str(), histeintrag);
  } else {
    spdlog::get(histeintrag)->info("Could not read value: " + objectName +
                                   " -> " + elementName);
    return 0.0;
  }
}

std::string readURLContent(std::string url) {
  std::string documentString;
  try {
    curlpp::Cleanup cleaner;
    curlpp::Easy request;
    request.setOpt(new curlpp::options::Url(url));
    std::ostringstream os;
    os << request;
    documentString = os.str();
  } catch ( curlpp::LogicError & e ) {
    std::cout << e.what() << std::endl;
  } catch ( curlpp::RuntimeError & e ) {
    std::cout << e.what() << std::endl;
  }
  return documentString;
}




// Getting the location name
std::string readTownName(rapidjson::Document& dTown, std::string histeintrag) {
  std::string townName = get_string(dTown, "name", histeintrag);
  townName.erase(
    remove(townName.begin(), townName.end(), '\"'),
    townName.end());
  return townName;
}

// Getting the visibility
int readVisibility(rapidjson::Document& dTown, std::string histeintrag) {
  return get_int(dTown, "visibility", histeintrag);
}

// Getting the location ID
int readLocationID(rapidjson::Document& dTown, std::string histeintrag) {
  return get_int(dTown, "id", histeintrag);
}

// work an entire town
void workTown(std::string town, pqxx::connection &c, std::string histeintrag) {
  spdlog::get(histeintrag)->info("Working on town: " + town);
  rapidjson::Document d;
  // Hier das Dokument parsen
  d.Parse(readURLContent(town).c_str());
  insulae::Weather w(readLocationID(d, histeintrag));
  w.setLocationName(readTownName(d, histeintrag));
  w.setTemperature(readNestedValue(d, "main", "temp", histeintrag));
  w.setHumidity(readNestedValue(d, "main", "humidity", histeintrag));
  w.setWindSpeed(readNestedValue(d, "wind", "speed", histeintrag));
  w.setWindDeg(readNestedValue(d, "wind", "deg", histeintrag));
  w.setVisibility(readVisibility(d, histeintrag));
  pqxx::work T(c, std::string("TRWriteWeatherData"));
  c.prepare(
    "setWeatherInsert",
    "INSERT INTO welt.weatherbasics (id, locationname, temperature, " \
    "humidity, windspeed, winddeg, visibility) " \
    "VALUES ($1, $2, $3, $4, $5, $6, $7)");
  pqxx::result Res = T.prepared("setWeatherInsert")(w.getLocationID())(
                       w.getLocationName())(
                       w.getTemperature())(w.getHumidity())
                     (w.getWindSpeed())(w.getWindDeg())(
                       w.getVisibility()).exec();
  Res.clear();
  T.commit();
  spdlog::get(histeintrag)->info("Finished importing town: " + town);
}

// Adding all ids from towns to a vector
std::vector<unsigned int> addWeatherIDs(pqxx::result r) {
  std::vector<unsigned int> _weatheridlist;
  for ( auto row : r ) {
    unsigned int id = row["id"].as<unsigned int>();
    _weatheridlist.push_back(id);
  }
  r.clear();
  return _weatheridlist;
}




int main() {
  // TODO(janhkrueger): Read config file from console
  const std::string konfigDatei = "/home/insulae/insulae-conf/insulae.cfg";
  insulae::Batch b(konfigDatei);
  // Variablen vorbereiten
  auto logger = spdlog::rotating_logger_mt(b.getConfigValue("weatherjobname"),
                b.getConfigValue("logdir")+b.getConfigValue("batchlogfile"),
                1048576 * 5, 3);
  logger->flush_on(spdlog::level::info);
  logger->info(b.getConfigValue("weatherjobname") + " started");
  b.setSkriptName(b.getConfigValue("weatherjobname"));
  std::string wTown;
  try {
    // connect to the database
    pqxx::connection C("dbname=" + b.getConfigValue("dbname") + " user=" +
                       b.getConfigValue("user") + " password=" +
                       b.getConfigValue("password") + " host=" +
                       b.getConfigValue("hostaddr") + " port=" +
                       b.getConfigValue("port") + " application_name=" +
                       b.getConfigValue("weatherjobname") );
    if (!C.is_open()) {
      logger->error("Konnte Datenbankverbindung nicht aufbauen.");
      return 11;
    }
    // hier alle IDs aus der DB lesen und in tlist anfügen
    std::string sql = "SELECT id " \
                      "FROM sonstiges.weathersource " \
                      "WHERE active IS TRUE";
    std::vector<unsigned int> _weatherList;
    // Alle Weathersources ermitteln
    pqxx::work txn(C);
    pqxx::result r = txn.exec(sql);
    _weatherList = addWeatherIDs(r);
    txn.commit();
    for (auto & element : _weatherList) {
      wTown = "http://api.openweathermap.org/data/2.5/weather?" \
              "appid=adf4646744e3b8d12bf7f52c69e2ac44&units=metric&id=" \
              +std::to_string(element);
      try {
        workTown(wTown, C, b.getConfigValue("weatherjobname"));
      } catch (const std::exception &e) {
        logger->error("{}", e.what());
      }
    }
  } catch (const pqxx::pqxx_exception &e) {
    std::cerr << "*** Caught pqxx_exception:\n";
    std::cerr << e.base().what() << "\n";
    const pqxx::sql_error *s = dynamic_cast<const
                               pqxx::sql_error*>(&e.base());
    if (s) std::cerr << "Query was: " << s->query() << "\n";
  } catch (const std::exception &e) {
    logger->error("{}", e.what());
    return 16;
  }
  logger->info("Finished");
  // of course :-)
  return 0;
}
