#ifndef COORDINATE_H
#define COORDINATE_H

#include <cmath>
#include <QString>

/** FIXME Test! */

namespace geojackal {

/**
 * Helper functions to convert between sexagesimal and decimal values.
 * Example:
 * @code
 * // convert 10°21'15" to decimal: d = 10.35416667
 * double d = Angle::getDecimal(10, 21, 15);
 * double m = Angle::getMinute(d); // m = 21.25
 * double s = Angle::getSecond(d); // s = 15
 * @endcode
 */
struct Angle {
  /**
   * Get the sexagesimal minute of a value
   * @param val decimal value
   * @return the sexagesimal minute of the angle
   */
  static inline double getMinute(double val) const {
    return (int) (val - (int)val) * 60;
  }

  /**
   * Get the sexagesimal seconds of an angle
   * @param val decimal value
   * @return the sexagesimal seconds of the angle
   */
  static inline double getSecond(double val) const {
    return (int) (getMinute(val) - (int)getMinute(val)) * 60;
  }

  /**
   * Convert sexagesimal value given in degree, minute and second to decimal.
   * No range checking is done, values for <tt>min</tt> and <tt>sec</tt>
   * greater than 60 count for the next larger part (i.e. 97' = 1°37').
   * @param deg sexagesimal degree part
   * @param min sexagesimal minute part
   * @param sec sexagesimal second part
   * @return decimal value
   */
  static inline double getDecimal(double deg, double min, double sec = 0) {
    return deg + (min / 60) + (sec / (60*60));
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
  double lon;
  /** The latitude (east-west) part of the coordinate, in degree */
  double lat;

  /**
   * Default constructor. The latitude and longitude parts are set to an
   * invalid value.
   */
  Coordinate() {
    lat = 361;
    lon = 361;
  }

  /**
   * Constructor to set latitude and longitude
   * @param lat The latitude part of the coordinate, in degree
   * @param lon The longitude part of the coordinate, in degree
   */
  Coordinate(double lon, double lat) {
    this->lon = lon;
    this->lat = lat;
  }

  static QString format(OutputFormat format) const {
    QString ns = (lon >= 0) ? "N" : "S";
    QString ew = (lat >= 0) ? "E" : "W";

    double abslon = fabs(lon);
    double abslat = fabs(lat);
    switch(format) {
      case FORMAT_DEG:
        return QString("%f %s %f %s").arg(abslon, ns, abslat, ew);
      case FORMAT_DEG_MIN:
        return QString("%d %f %s %d %f %s").arg((int)abslon, Angle::getMinute(
          abslon), ns, (int)abslat, Angle::getMinute(abslon), ew);
      case FORMAT_DEG_MIN_SEC:
        return QString("%d %d %f %s %d %d %f %s").arg((int)abslon,
          (int)Angle::getMinute(abslon), Angle::getSecond(abslon), ns,
          (int)abslat, (int)Angle::getMinute(abslon), Angle::getSecond(abslon),
          ew);
      default:
        return QString("");
    }
  }
};

}
#endif // COORDINATE_H
