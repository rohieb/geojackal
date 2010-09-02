/**
 * @file Coordinate.h
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

#ifndef COORDINATE_H
#define COORDINATE_H

#include "global.h"
#include <QString>

namespace geojackal {

/**
 * Solid angle. Allows conversion between sexagesimal and decimal values.
 * Example:
 * @code
 * // create angle with sexagesimal values
 * Angle a(10, 21, 15);
 * double d1 = a.deg(); // d = 10.35416667, same as decimal representation
 * double m = a.min(); // m = 21.25
 * double s = a.sec(); // s = 15
 * // or, if you only want the conversion to decimal:
 * double d2 = Angle(10, 21, 15);
 * @endcode
 *
 * Note that the cast operator to @c double is overloaded accordingly to return
 * the angle in decimal degree.
 */
struct Angle {

  /** Decimal angle in degree */
  double degree;

  /**
   * Constructor to set the angle in decimal degree
   * @param deg decimal angle in degree
   */
  Angle(double deg = 0) :
    degree(deg) {
  }

  /**
   * Constructor to set the angle in sexagesimal degree (consisting of degree,
   * minute and second). No range checking is done, values for <tt>min</tt>
   * and <tt>sec</tt> greater than 60 count for the next larger part
   * (i.e. 97' = 1°37').
   * @param deg sexagesimal degree part
   * @param min sexagesimal minute part
   * @param sec sexagesimal second part
   */
  Angle(double deg, double min, double sec = 0) :
    degree(deg + (min / 60) + (sec / (60 * 60))) {
  }

  /**
   * Get the decimal/sexagesimal value of the angle in degree. Note that decimal
   * and sexagesimal values in degree are in fact the same :-)
   * @return the decimal value of the angle
   */
  inline double deg() const {
    return degree;
  }

  /**
   * Get the sexagesimal minute of the angle
   * @return the sexagesimal minute of the angle
   */
  inline double min() const {
    return (deg() - (int)deg()) * 60;
  }

  /**
   * Get the sexagesimal seconds of the angle
   * @return the sexagesimal seconds of the angle
   */
  inline double sec() const {
    return (min() - (int)min()) * 60;
  }

  /**
   * Assignment operator, sets the decimal angle in degree
   * @param val the new value, in decimal degree
   */
  Angle&
  operator=(double val) {
    degree = val;
    return *this;
  }

  /** Cast operator to double */
  operator double() const {
    return degree;
  }
};

/** The invalid angle used to determine unset values */
const static Angle ANGLE_INVALID = 361.0;

/**
 * Model of a geographic coordinate with latitude and longitude.
 */
struct Coordinate {
  /** Control of the output formatting */
  enum OutputFormat {
    /** Floating-point degree part, like <tt>N 52.09147 E 10.19772</tt> */
    FormatDeg,
    /**
     * Integral degree and floating-point minute part, like
     * <tt>N 52° 14.782 10° 19.256</tt>
     */
    FormatDegMin,
    /**
     * Integral degree and minute, floating-point second part, like
     * <tt>N 52° 9' 14.02" E 10° 19' 25.12"</tt>
     */
    FormatDegMinSec
  };

  /**
   * The latitude (north-south) part of the coordinate, in degree. Positive
   * values determine coordinates on the western hemisphere, negative values
   * determine values on the eastern hemisphere.
   */
  Angle lat;
  /**
   * The longitude (east-west) part of the coordinate, in degree. Positive
   * values determine coordinates on the northern hemisphere, negative values
   * determine values on the southern hemisphere.
   */
  Angle lon;

  Coordinate();
  Coordinate(Angle latitude, Angle longitude);
  QString format(OutputFormat format) const;
};

QDebug& operator<<(QDebug& dbg, const Coordinate& c);

}
#endif // COORDINATE_H
