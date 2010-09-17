/**
 * @file Geocache.cpp
 * @date Jul 14, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 *
 * Copyright (C) 2010 Roland Hieber
 * 
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License, version 3, as published 
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with 
 * this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include "logic/Geocache.h"

/** dump geocaches to a debug stream */
QDebug& geojackal::operator<<(QDebug& dbg, const Geocache& geocache) {
  dbg.nospace() << "{ wp: " << geocache.waypoint;
  dbg.nospace() << ", name: " << geocache.name;
  dbg.nospace() << ", coord: " << *geocache.coord;
  dbg.nospace() << ", type: " << geocache.type;
  dbg.nospace() << ", size: " << geocache.size;
  dbg.nospace() << ", diff: " << geocache.difficulty;
  dbg.nospace() << ", terr: " << geocache.terrain;
  dbg.nospace() << ", placed: " << geocache.placed->toString(Qt::ISODate);
  dbg.nospace() << ", owner: " << geocache.owner;
  dbg.nospace() << ", archived: " << geocache.archived;
  dbg.nospace() << " }";
  return dbg.maybeSpace();
}


/**
 * ROT13 function to encode/decode a geocache hint. Due to the nature of ROT13,
 * both these operations are absolutely equivalent.
 * @param text Text to be encoded or decoded
 * @return Encoded/decoded text
 */
QString geojackal::rot13(QString& text) {
  int n = text.length();
  QString out = text;
  while(n--) {
    if((text.at(n) >= QChar('a') && text.at(n) <= QChar('m')) ||
      (text.at(n) >= QChar('A') && text.at(n) <= QChar('M'))) {
      // letters between a/A and m/M
      out[n] = QChar(text.at(n).toAscii() + 13);
    } else if((text.at(n) > QChar('n') && text.at(n) <= QChar('z')) ||
      (text.at(n) >= QChar('N') && text.at(n) <= QChar('Z'))) {
      // letters between n/N and z/Z
      out[n] = QChar(text.at(n).toAscii() - 13);
    }
  }
  return out;
}
