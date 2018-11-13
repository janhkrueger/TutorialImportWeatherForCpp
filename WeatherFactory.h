/*
 * WeatherFactory.h
 *
 *  Created on: 16.01.2018
 *  Author: janhkrueger
 *  Copyright 2018 janhkrueger
 */

#ifndef SRC_WEATHERFACTORY_H_
#define SRC_WEATHERFACTORY_H_

#include "Weather.h"
#include <pqxx/pqxx>
#include <string>
#include <stdexcept>
#include <cmath>

namespace insulae {

class WeatherFactory {
 private:
  unsigned int _karteID;

 public:
  virtual ~WeatherFactory();
  Weather create(unsigned int karteid, pqxx::connection &conn);
  void DisposeObject();
};

} /* namespace insulae */

#endif  // SRC_WEATHERFACTORY_H_
