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
    case FormatDeg:
      return QString("%1 %2 %3 %4").arg(ns).arg(QString::number(abslat, 'f',
        5)).arg(ew).arg(QString::number(abslon, 'f', 5));
    case FormatDegMin:
      return QString("%1 %2\xb0 %3 %4 %5\xb0 %6").arg(ns).arg((int) abslat).
        arg(QString::number(abslat.min(), 'f', 3)).arg(ew).arg((int) abslon).
        arg(QString::number(abslon.min(), 'f', 3));
    case FormatDegMinSec:
      return QString("%1 %2\xb0 %3' %4\" %5 %6\xb0 %7' %8\"").
        arg(ns).arg((int) abslat).arg((int) abslat.min()).
        arg(QString::number(abslat.sec(), 'f', 2)).
        arg(ew).arg((int) abslon).arg((int) abslon.min()).
        arg(QString::number(abslon.sec(), 'f', 2));
    default:
      return QString("");
  }
}

/** Write Coordinate objects to debug stream */
QDebug& geojackal::operator<<(QDebug& dbg, const Coordinate& c) {
  dbg.nospace() << c.format(Coordinate::FormatDeg);
  return dbg.maybeSpace();
}
