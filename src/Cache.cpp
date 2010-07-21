/**
 * @file Cache.cpp
 * @date Jul 14, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
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
