/**
 * @file GCSpiderCachePage.h
 * @date Jun 11, 2010
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
  QString text_;

public:
  GCSpiderCachePage(QString text);
  virtual ~GCSpiderCachePage();

  bool all(Cache& buf) const;
  bool name(QString& buf) const;
  bool waypoint(QString& buf) const;
  bool type(WaypointType& buf) const;
  bool coord(Coordinate& buf) const;
  bool desc(QString& buf) const;
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

QString replaceHtmlEntities(QString buf);
int monthToOrd(QString month);

}

#endif /* GCCACHESPIDER_H_ */
