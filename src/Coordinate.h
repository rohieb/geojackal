#ifndef COORDINATE_H
#define COORDINATE_H

#include <cmath>
#include <QString>

/** FIXME Test! */

namespace geojackal {

/**
 * Helper functions to convert angular values between decimal and
 * degree/minute/second
 */
struct Angle {
  /**
   * Get the decimal minute of an angle
   * @param angle value in degree
   * @return the decimal minute of the angle
   */
  static double getMinute(double angle) const {
    return (int) (deg - (int) deg) * 60;
  }

  /**
   * Get the decimal seconds of an angle
   * @param angle valuein degree
   * @return the decimal seconds of the angle
   */
  static double getSecond(double angle) const {
    return (int) (getMinute(angle) - (int) getMinute(angle)) * 60;
  }
};

/**
 * Model of a geographic coordinate with latitude and longitude.
 */
struct Coordinate
{
  /** Control of the output formatting */
  enum OutputFormat {
    /** Decimal degree, like <tt>52.0914782 N 10.1977256 E</tt> */
    FORMAT_DEG,
    /**
     * Integral degree and decimal minutes, like
     * <tt>52° 14.782 N 10° 19.256 E</tt>
     */
    FORMAT_DEG_MIN,
    /**
     * Integral degree and minutes, and decimal seconds, like
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
        return QString("%d %f %s %d %f %s").arg(
          (int) abslon, Angle::getMinute(abslon), ns,
          (int) abslat, Angle::getMinute(abslon), ew);
      case FORMAT_DEG_MIN_SEC:
        return QString("%d %d %f %s %d %d %f %s").arg((int) abslon,
          (int) Angle::getMinute(abslon), Angle::getSecond(abslon), ns,
          (int) abslat, (int) Angle::getMinute(abslon),
          Angle::getSecond(abslon), ew);
      default:
        return QString("");
    }
  }
};

}
#endif // COORDINATE_H
