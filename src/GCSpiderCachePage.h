/**
 * @file GCSpiderCachePage.h
 * @date Jun 11, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#ifndef GCCACHESPIDER_H_
#define GCCACHESPIDER_H_

#include "global.h"
#include "Cache.h"
#include <QString>

namespace geojackal {

/**
 * Extracts cache information from a geocaching.com cache description page.
 * Note: All functions in this class are reentrant.
 * @todo how to use?
 */
class GCSpiderCachePage {
private:
  /** Text of the cache description page */
  const QString& text_;

public:
  GCSpiderCachePage(const QString& text);
  virtual ~GCSpiderCachePage();

  bool all(Cache& buf) const;
  bool name(QString& buf) const;
  bool waypoint(QString& buf) const;
  bool type(WaypointType& buf) const;
  bool coord(Coordinate& buf) const;
  bool desc(CacheDesc& buf) const;
  bool shortDesc(QString& buf) const;
  CacheSize size() const;
  unsigned int difficulty() const;
  unsigned int terrain() const;
  bool placed(QDate& buf) const;
  bool found(QDate& buf) const;
  bool owner(QString& buf) const;
  bool waypoints(QVector<Waypoint>& buf) const;
  bool logs(QVector<LogMessage>& buf) const;
  bool attrs(QVector<CacheAttribute>& buf) const;
  bool hint(QString& buf) const;
  bool archived() const;
};

}

#endif /* GCCACHESPIDER_H_ */
