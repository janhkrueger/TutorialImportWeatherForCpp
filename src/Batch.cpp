/*
 * Batch.cpp
 *
 *  Created on: 20.12.2014
 *      Author: janhkrueger
 *      Copyright 2016 janhkrueger
 */

#include "Batch.h"
#include <pqxx/pqxx>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <map>
#include <cmath>

namespace insulae {

/*
 * Konstruktur.
 * Einlesen der Konfigurationsdatei
 */
Batch::Batch(std::string inputfile) {
  setSkriptName("Unbestimmt");
  parse(inputfile);
}

Batch::~Batch() {
  // TODO(janhkrueger) Auto-generated destructor stub
}

/**
 * Ermittlung des aktuellen Zeitstempels im Unix-Format.
 * @since 16.12.2014
 */
time_t Batch::aktuellerZeitstempel() {
  time_t seconds;
  seconds = time(NULL);
  return seconds;
}

void Batch::setSkriptName(std::string skriptName) {
  skriptname = skriptName;
}  // Ende setSkriptName


/**
 * Berechnung von PI
 * @since 23.12.2014
 */
double Batch::getPI() {
  return atan(1)*4;
}  // Ende getPi


int Batch::rotieren_x(int x, int radius, int schritte,
                      int aktuellerschritt) {
  int neux = static_cast<int>(x+radius * std::cos(2 * M_PI / schritte *
                              aktuellerschritt ) );
  return neux;
}

int Batch::rotieren_y(int y, int radius, int schritte,
                      int aktuellerschritt) {
  int neuy = static_cast<int>(y+radius * std::sin(2 * M_PI / schritte *
                              aktuellerschritt ) );
  return neuy;
}


int Batch::updateRotationsSchritt(int aktuellerSchritt, int maxSchritte) {
  aktuellerSchritt = aktuellerSchritt + 1;
  if (aktuellerSchritt == maxSchritte) aktuellerSchritt = 0;
  return aktuellerSchritt;
}

/**
 * Auslesen von Konfigurationsparameters aus einer Textdatei
 * @since 19.12.2014
 * TODO(janhkruger) Anstaendige Fehlerbehandlung
 */
bool Batch::parse(std::string inputfile) {
  std::string line;
  std::ifstream myfile(inputfile.c_str());
  while ( getline(myfile, line) ) {
    //  Ausfiltern von Kommentaren
    if (line[0] != '#') {
      int pos = static_cast<int>(line.find("="));
      std::string key = (line.substr(0, pos));
      std::string value = line.substr(pos+1);
      options[key] = value;
    }
  }
  myfile.close();
  // TODO(janhkrueger) Hier noch eine korrekte Behandlung
  return true;
}

std::string Batch::getConfigValue(std::string valueName) {
  return options[valueName];
}  // Ende getConfigValue



/**
 * Konfigurationsparameter aus der Datenbank lesen
 * Sofern eine Einstellung nicht gefunden wurde, liefert die Methode den mitgegebenen
 * Standartwert.
 * @since 13.12.2014
 * @param Datenbank, Wert, Default
 */
std::string Batch::holeEinstellungDefault(pqxx::connection &c,
    std::string einstellung, std::string vorgabe) {
  try {
    pqxx::nontransaction T(c, std::string("TRgetEinstellung"));
    pqxx::result R = T.prepared("getEinstellung")(einstellung).exec();
    T.commit();
    // Ich erwarte explizit nur ein Ergebnis. Alles andere ist nicht korrekt.
    if (R.size() == 1) {
      for (pqxx::result::const_iterator row = R.begin();
           row != R.end();
           ++row) {
        for (pqxx::tuple::const_iterator field = row->begin();
             field != row->end();
             ++field)
          return field->c_str();
      }  // Ende For
    } else {
      return vorgabe;
    }
  }  // Ende Try
  catch (const pqxx::sql_error &e) {
    std::cout << "Holen der Einstellung fehlgeschlagen: " << e.what() << '\n';
    return "2";
  } catch (const pqxx::broken_connection &e) {
    std::cout << "Verbindung zur Datenbank verloren: " << e.what() << '\n';
    return "2";
  } catch (const std::exception &e) {
    // TODO(janhkrueger) Exception richtig ausformulieren.
    // 12.12.2014, gibt es so etwas wie Log4J fuer C++ ?
    std::cout << "PANIK: " << e.what() << '\n';
    return "2";
  }
  return einstellung;
}  // Ende holeEinstellungDefault


/**
 * @deprecated
 */
std::string Batch::holeEinstellung(pqxx::connection &c,
                                   std::string einstellung) {
  return holeEinstellungDefault(c, einstellung, "1");
}  //  Ende holeEinstellung

} /* namespace insulae */
