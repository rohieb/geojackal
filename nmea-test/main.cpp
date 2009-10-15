/*
 * main.cpp
 *
 *  Created on: 15.10.2009
 *      Author: rohieb
 */

#include <iostream>
#include <string>
#include <stdlib.h>
#include <nmea/nmea.h>

using namespace std;

int main(int argc, char ** argv) {
  nmeaPARSER * pnm;
  string line;

  if(!nmea_parser_init(pnm)) {
    cerr << "Could not initialise the NMEA parser!" << endl;
    exit(1);
  }

  // read from stdin and parse on line break
  while(!cin.eof()) {
    line.append((const string&)cin);
    if(line.at(line.size()-1) == '\n') {
      nmeaINFO * pni;
      nmea_parse(pnm, line.c_str(), line.size(), pni);
      cout << "lat: " << pni->lat << ", lon: " << pni->lon << endl;
      line.empty();
    }
  }

  nmea_parser_destroy(pnm);
}
