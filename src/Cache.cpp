/**
 * @file Cache.cpp
 * @date Jul 14, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#include "Cache.h"

namespace geojackal {

/** dump caches to a debug stream */
QDebug& operator<<(QDebug dbg, Cache& cache) {
  dbg << "{ wp:" << cache.waypoint;
  dbg << ", name:" << cache.name;
  dbg << ", coord:" << *cache.coord;
  dbg << ", type:" << cache.type;
  dbg << ", size:" << cache.size;
  dbg << ", diff:" << cache.difficulty;
  dbg << ", terr:" << cache.terrain;
  dbg << ", placed:" << cache.placed;
  dbg << ", owner:" << cache.owner;
  dbg << ", archived:" << cache.archived;
  dbg << " }";
  return dbg.nospace();
}

}
