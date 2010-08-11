/**
 * @file Coordinate.cpp
 * @date 23 Apr 2010
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

#include "Coordinate.h"
#include <cmath>

using namespace geojackal;

/**
 * Default constructor. The latitude and longitude parts are set to an
 * invalid value.
 */
Coordinate::Coordinate() :
  lat(ANGLE_INVALID), lon(ANGLE_INVALID) {
}

/**
 * Constructor to set latitude and longitude
 * @param latitude The latitude (north-south) part of the coordinate, in degree
 * @param longitude The longitude (east-west) part of the coordinate, in degree
 */
Coordinate::Coordinate(Angle latitude, Angle longitude) :
  lat(latitude), lon(longitude) {
}

QString Coordinate::format(OutputFormat format) const {
  QString ns = (lon >= 0) ? "N" : "S";
  QString ew = (lat >= 0) ? "E" : "W";

  Angle abslon(fabs(lon));
  Angle abslat(fabs(lat));

  switch(format) {
    case FORMAT_DEG:
      return QString("%1 %2 %3 %4").arg(abslat).arg(ns).arg(abslon).arg(ew);
    case FORMAT_DEG_MIN:
      return QString("%1 %2 %3 %4 %5 %6").arg((int)abslat).arg(abslat.min()).
        arg(ns).arg((int)abslon).arg(abslon.min()).arg(ew);
    case FORMAT_DEG_MIN_SEC:
      return QString("%1 %2 %3 %4 %5 %6 %7 %8").arg((int)abslat).arg(
        (int)abslat.min()).arg(abslat.sec()).arg(ns).arg((int)abslon).arg(
        abslon.min()).arg(abslon.sec()).arg(ew);
    default:
      return QString("");
  }
}

/** Write Coordinate objects to debug stream */
QDebug& geojackal::operator<<(QDebug& dbg, const Coordinate& c) {
  dbg.nospace() << c.format(Coordinate::FORMAT_DEG);
  return dbg.maybeSpace();
}
