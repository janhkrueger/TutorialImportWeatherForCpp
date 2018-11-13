/*
 * Weather.h
 *
 *  Created on: 16.01.2018
 *  Author: janhkrueger
 *  Copyright 2018 janhkrueger
 */

#ifndef SRC_WEATHER_H_
#define SRC_WEATHER_H_

#include <string>

namespace insulae {
class Weather {
 private:
  // Main
  std::string _locationName;
  unsigned int _mainID;

  // Main weather features
  double _temperature;
  unsigned int _humidity;
  /// unsigned int _pressure;
  /// double _temp_min;
  /// double _temp_max;

  // Visibility
  unsigned int _visibility;

  // Wind
  double _windSpeed;
  unsigned int _windDeg;

  // Clouds
  /// std::string _clouds;

  // sys
  /// unsigned int _sysType;
  /// unsigned int _sysID;
  /// double _sysMessage;
  /// std::string _sysCountry;
  /// unsigned int _sysSunrise;
  /// unsigned int _sysSunset;

 public:
  Weather();
  explicit Weather(unsigned int locationID);
  virtual ~Weather();
  void DisposeObject();
  void setLocationName(std::string locationName);
  void setLocationID(unsigned int locationID);
  void setTemperature(double temperature);
  void setHumidity(unsigned int humidity);
  void setWindSpeed(double windSpeed);
  unsigned int getLocationID();
  std::string getLocationName();
  double getTemperature();
  unsigned int getHumidity();
  double getWindSpeed();
  void setVisibility(unsigned int visibility);
  unsigned int getVisibility();
  void setWindDeg(unsigned int windDeg);
  unsigned int getWindDeg();
};

}  // namespace insulae

#endif  // SRC_WEATHER_H_
