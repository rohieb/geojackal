/**
 * @file GCCacheSpider.h
 * @date Jun 11, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#ifndef GCCACHESPIDER_H_
#define GCCACHESPIDER_H_

#include <QString>
#include "Cache.h"

namespace geojackal {

/**
 * Extracts cache information from a cache description page
 */
class GCCacheSpider {
private:
  /** Text of the cache description page */
  QString& text;

public:
  GCCacheSpider(QString& text);
  virtual ~GCCacheSpider();

  bool all(Cache& buf);
  bool name(QString& buf);
  bool waypoint(QString& buf);
  bool type(WaypointType& buf);
  bool coord(Coordinate& buf);
  bool desc(CacheDesc& buf);
  bool shortDesc(QString& buf);
  CacheSize size();
  unsigned int difficulty();
  unsigned int terrain();
  bool placed(QDate& buf);
  bool found(QDate& buf);
  bool owner(QString& buf);
  bool waypoints(QVector<Waypoint>& buf);
  bool logs(QVector<LogMessage>& buf);
  bool attrs(QVector<CacheAttribute>& buf);
  bool hint(QString& buf);
  bool archived();
};

}

#endif /* GCCACHESPIDER_H_ */
