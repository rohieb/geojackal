#ifndef COORDINATE_H
#define COORDINATE_H

#include <cmath>
#include <QString>

/** FIXME Test! */

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

/**
 * Model of a geographic coordinate with latitude and longitude.
 */
struct Coordinate {
  /** Control of the output formatting */
  enum OutputFormat {
    /** Floating-point degree part, like <tt>52.0914782 N 10.1977256 E</tt> */
    FORMAT_DEG,
    /**
     * Integral degree and floating-point minute part, like
     * <tt>52° 14.782 N 10° 19.256 E</tt>
     */
    FORMAT_DEG_MIN,
    /**
     * Integral degree and minute, floating-point second part, like
     * <tt>52° 9' 14.02" N 10° 19' 25.12" E</tt>
     */
    FORMAT_DEG_MIN_SEC
  };

  /** The longitude (north-south) part of the coordinate, in degree */
  Angle lon;
  /** The latitude (east-west) part of the coordinate, in degree */
  Angle lat;

  /**
   * Default constructor. The latitude and longitude parts are set to an
   * invalid value.
   */
  Coordinate() :
    lon(361), lat(361.0) {
  }

  /**
   * Constructor to set latitude and longitude
   * @param latitude The latitude part of the coordinate, in degree
   * @param longitude The longitude part of the coordinate, in degree
   */
  Coordinate(Angle longitude, Angle latitude) :
    lon(longitude), lat(latitude) {
  }

  QString format(OutputFormat format) const {
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
};

}
#endif // COORDINATE_H
