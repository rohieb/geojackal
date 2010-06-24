/**
 * @file Coordinate.cpp
 * @date 23 Apr 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#include "Coordinate.h"
#include <cmath>

using namespace geojackal;

/**
 * Default constructor. The latitude and longitude parts are set to an
 * invalid value.
 */
Coordinate::Coordinate() :
  lon(ANGLE_INVALID), lat(ANGLE_INVALID) {
}

/**
 * Constructor to set latitude and longitude
 * @param latitude The latitude part of the coordinate, in degree
 * @param longitude The longitude part of the coordinate, in degree
 */
Coordinate::Coordinate(Angle longitude, Angle latitude) :
  lon(longitude), lat(latitude) {
}

QString Coordinate::format(OutputFormat format) const {
  QString ns = (lon >= 0) ? "N" : "S";
  QString ew = (lat >= 0) ? "E" : "W";

  Angle abslon(fabs(lon));
  Angle abslat(fabs(lat));

  switch(format) {
    case FORMAT_DEG:
      return QString("%f %s %f %s").arg(abslon).arg(ns).arg(abslat).arg(ew);
    case FORMAT_DEG_MIN:
      return QString("%d %f %s %d %f %s").arg((int)abslon).arg(abslon.min()).
        arg(ns).arg((int)abslat).arg(abslat.min()).arg(ew);
    case FORMAT_DEG_MIN_SEC:
      return QString("%d %d %f %s %d %d %f %s").arg((int)abslon).arg(
        (int)abslon.min()).arg(abslon.sec()).arg(ns).arg((int)abslat).arg(
        abslat.min()).arg(abslat.sec()).arg(ew);
    default:
      return QString("");
  }
}
