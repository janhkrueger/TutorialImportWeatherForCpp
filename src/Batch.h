/*
 * Batch.h
 *
 *  Created on: 20.12.2014
 *      Author: janhkrueger
*   Copyright 2016 janhkrueger
 */

#ifndef BATCH_H_
#define BATCH_H_

#include <pqxx/pqxx>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <map>
#include <cmath>

namespace insulae {
class Batch {
 private:
  std::map<std::string, std::string> options;
  std::string skriptname;

 public:
  explicit Batch(std::string inputfile);
  virtual ~Batch();
  time_t aktuellerZeitstempel();
  bool parse(std::string filename);
  std::string getConfigValue(std::string valueName);
  bool queriesVorbereiten(pqxx::connection &c);
  bool schreibeStatus(pqxx::connection &c, std::string varstatus);
  void setSkriptName(std::string skriptName);
  double getPI();
  int rotieren_x(int x, int radius, int schritte,
                 int aktuellerschritt);
  int rotieren_y(int y, int radius, int schritte,
                 int aktuellerschritt);
  int updateRotationsSchritt(int aktuellerSchritt, int maxSchritte);
  std::string holeEinstellungDefault(pqxx::connection &c,
                                     std::string einstellung,
                                     std::string vorgabe);
  std::string holeEinstellung(pqxx::connection &c, std::string einstellung);
};

} /* namespace insulae */

#endif /* BATCH_H_ */
