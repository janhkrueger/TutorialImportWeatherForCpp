/*
 * Weather.cpp
 *
 *  Created on: 16.01.2018
 *  Author: janhkrueger
 *  Copyright 2018 janhkrueger
 */

#include "Weather.h"
#include <string>
#include <stdexcept>
#include <cmath>

namespace insulae {

/*
 * Konstruktur.
 */
Weather::Weather(unsigned int locationID) {
  _mainID = locationID;
}

Weather::Weather() {
}

Weather::~Weather() {
  // TODO(janhkrueger) Auto-generated destructor stub
}

void Weather::DisposeObject() {
  delete this;
}

// LocatioName
void Weather::setLocationName(std::string locationName) {
  _locationName = locationName;
}

std::string Weather::getLocationName() {
  return _locationName;
}

// locationID
void Weather::setLocationID(unsigned int locationID) {
  _mainID = locationID;
}

unsigned int Weather::getLocationID() {
  return _mainID;
}

// temperature
void Weather::setTemperature(double temperature) {
  _temperature = temperature;
}

double Weather::getTemperature() {
  return _temperature;
}

// humidity
void Weather::setHumidity(unsigned int humidity) {
  _humidity = humidity;
}

unsigned int Weather::getHumidity() {
  return _humidity;
}

// windSpeed
void Weather::setWindSpeed(double windSpeed) {
  _windSpeed = windSpeed;
}

double Weather::getWindSpeed() {
  return _windSpeed;
}

// Visibility
void Weather::setVisibility(unsigned int visibility) {
  _visibility = visibility;
}

unsigned int Weather::getVisibility() {
  return _visibility;
}

// Wind Deg
void Weather::setWindDeg(unsigned int windDeg) {
  _windDeg = windDeg;
}

unsigned int Weather::getWindDeg() {
  return _windDeg;
}

} /* namespace insulae */
