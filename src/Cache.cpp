/**
 * @file Cache.cpp
 * @date Jul 14, 2010
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

#include "Cache.h"

namespace geojackal {

/** dump caches to a debug stream */
QDebug& operator<<(QDebug& dbg, const Cache& cache) {
  dbg.nospace() << "{ wp: " << cache.waypoint;
  dbg.nospace() << ", name: " << cache.name;
  dbg.nospace() << ", coord: " << *cache.coord;
  dbg.nospace() << ", type: " << cache.type;
  dbg.nospace() << ", size: " << cache.size;
  dbg.nospace() << ", diff: " << cache.difficulty;
  dbg.nospace() << ", terr: " << cache.terrain;
  dbg.nospace() << ", placed: " << cache.placed->toString(Qt::ISODate);
  dbg.nospace() << ", owner: " << cache.owner;
  dbg.nospace() << ", archived: " << cache.archived;
  dbg.nospace() << " }";
  return dbg.maybeSpace();
}

}
