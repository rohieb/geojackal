/**
 * @file Cache.h
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

#ifndef CACHE_H
#define CACHE_H

#include "global.h"
#include <QString>
#include <QDateTime>
#include <QVector>
#include "Coordinate.h"

namespace geojackal {

/**
 * Constants for cache/waypoint types
 */
enum WaypointType {
  /** Traditional Cache */
  TYPE_TRADI,
  /** Multi Cache */
  TYPE_MULTI,
  /** Mystery Cache */
  TYPE_MYSTERY,
  /** Caching Event */
  TYPE_EVENT,
  /** Virtual Cache */
  TYPE_VIRTUAL,
  /** Webcam Cache */
  TYPE_WEBCAM,
  /** Mega Event */
  TYPE_MEGAEVENT,
  /** Letterbox */
  TYPE_LETTERBOX,
  /** Whereigo (TM) */
  TYPE_WHEREIGO,
  /** Cache In Trash Out Event */
  TYPE_CITO,
  /** Earth Cache */
  TYPE_EARTH,
  /** Reverse Cache */
  TYPE_REVERSE,
  /** GPS Adventures Maze Exhibit */
  TYPE_GAME,
  /** Project A.P.E. Cache */
  TYPE_PROJECTAPE,
  /** Stage of a multicache */
  TYPE_STAGE,
  /** Final of a multicache */
  TYPE_FINAL,
  /** Question to answer */
  TYPE_QUESTION,
  /** Reference point */
  TYPE_REFERENCE,
  /** Parking area */
  TYPE_PARKING,
  /** Trailhead */
  TYPE_TRAILHEAD,
  /** Other, not classified */
  TYPE_OTHER,
  /** Unknown */
  TYPE_UNKNOWN
};

/**
 * Cache attributes
 */
enum CacheAttribute {
  /** Not always available */
  ATTR_AVAILABLE_NO = 0,
  /** Always available */
  ATTR_AVAILABLE_YES = 1,
  /** Not suitable for bike tour */
  ATTR_BICYCLES_NO = 2,
  /** Suitable for bike tour */
  ATTR_BICYCLES_YES = 3,
  /** Boat required */
  ATTR_BOAT = 4,
  /** Campfires not allowed */
  ATTR_CAMPFIRES_NO = 5,
  /** Campfires allowed */
  ATTR_CAMPFIRES_YES = 6,
  /** Camping not allowed */
  ATTR_CAMPING_NO = 7,
  /** Camping allowed */
  ATTR_CAMPING_YES = 8,
  /** Beware of falling rocks */
  ATTR_CLIFF = 9,
  /** Difficult climb */
  ATTR_CLIMBING_NO = 10,
  /** Easy climb */
  ATTR_CLIMBING_YES = 11,
  /** Beware of cattle */
  ATTR_COW = 12,
  /** Danger! */
  ATTR_DANGER = 13,
  /** Dogs not allowed */
  ATTR_DOGS_NO = 14,
  /** Dogs allowed */
  ATTR_DOGS_YES = 15,
  /** Fee required */
  ATTR_FEE = 16,
  /** Flashlight required */
  ATTR_FLASHLIGHT = 17,
  /** Not suitable for hiking */
  ATTR_HIKING_NO = 18,
  /** Suitable for hiking */
  ATTR_HIKING_YES = 19,
  /** Not suitable for horses */
  ATTR_HORSES_NO = 20,
  /** Suitable for horses */
  ATTR_HORSES_YES = 21,
  /** Hunting ground */
  ATTR_HUNTING = 22,
  /** Not suitable for jeeps */
  ATTR_JEEPS_NO = 23,
  /** Suitable for jeeps */
  ATTR_JEEPS_YES = 24,
  /** Not suitable for kids */
  ATTR_KIDS_NO = 25,
  /** Suitable for kids */
  ATTR_KIDS_YES = 26,
  /** Needs maintenance */
  ATTR_MAINT = 27,
  /** Abandoned mine */
  ATTR_MINE = 28,
  /** Not suitable for motorbikes */
  ATTR_MOTORCYCLES_NO = 29,
  /** Suitable for motorbikes */
  ATTR_MOTORCYCLES_YES = 30,
  /** Not recommended at night */
  ATTR_NIGHT_NO = 31,
  /** Recommended at night */
  ATTR_NIGHT_YES = 32,
  /** Estimated time over one hour */
  ATTR_ONEHOUR_NO = 33,
  /** Estimated time below one hour */
  ATTR_ONEHOUR_YES = 34,
  /** No parking available */
  ATTR_PARKING_NO = 35,
  /** Parking available */
  ATTR_PARKING_YES = 36,
  /** No phone available */
  ATTR_PHONE_NO = 37,
  /** Phone available */
  ATTR_PHONE_YES = 38,
  /** Picnic area not available */
  ATTR_PICNIC_NO = 39,
  /** Picnic area available */
  ATTR_PICNIC_YES = 40,
  /** Poison oak around */
  ATTR_POISONOAK_NO = 41,
  /** No poison oak around */
  ATTR_POISONOAK_YES = 42,
  /** Nearby public transportation available */
  ATTR_PUBLIC = 43,
  /** Not suitable for quads */
  ATTR_QUADS_NO = 44,
  /** Suitable for quads */
  ATTR_QUADS_YES = 45,
  /** Rapelling required */
  ATTR_RAPPELLING = 46,
  /** No restrooms available */
  ATTR_RESTROOMS_NO = 47,
  /** Restrooms available */
  ATTR_RESTROOMS_YES = 48,
  /** Scenic view */
  ATTR_SCENIC_NO = 49,
  /** No scenic view */
  ATTR_SCENIC_YES = 50,
  /** Scuba diving required */
  ATTR_SCUBA = 51,
  /** Beware of snakes */
  ATTR_SNAKES = 52,
  /** Not suitable for snowmobiles */
  ATTR_SNOWMOBILES_NO = 53,
  /** Suitable for snowmobiles */
  ATTR_SNOWMOBILES_YES = 54,
  /** No stealth required */
  ATTR_STEALTH_NO = 55,
  /** Stealth required */
  ATTR_STEALTH_YES = 56,
  /** Not stroller accessible */
  ATTR_STROLLER_NO = 57,
  /** Stroller accessible */
  ATTR_STROLLER_YES = 58,
  /** Swimming required */
  ATTR_SWIMMING = 59,
  /** Beware of thorns */
  ATTR_THORNS = 60,
  /** Beware of ticks */
  ATTR_TICKS = 61,
  /** Wading required */
  ATTR_WADING = 62,
  /** No drinking water available */
  ATTR_WATER_NO = 63,
  /** Drinking water available */
  ATTR_WATER_YES = 64,
  /** Not wheelchair accessible */
  ATTR_WHEELCHAIR_NO = 65,
  /** Wheelchair accessible */
  ATTR_WHEELCHAIR_YES = 66,
  /** Not available in winter */
  ATTR_WINTER_NO = 67,
  /** Available in winter */
  ATTR_WINTER_YES = 68
};

/** Number of attributes */
const ushort NUM_ATTRIBUTES = static_cast<ushort>(ATTR_WINTER_YES) + 1;

/**
 * Log types
 */
enum LogType {
  /** Attended */
  LOG_ATTENDED,
  /** Post Reviewer Note */
  LOG_REVIEWER_NOTE,
  /** Took a webcam photo */
  LOG_WEBCAM_PHOTO,
  /** Update Coordinates */
  LOG_COORD_UPDATE,
  /** Temporarily Disable Listing  */
  LOG_DISABLED,
  /** Enable Listing */
  LOG_ENABLED,
  /** Publish */
  LOG_PUBLISH,
  /** Owner Maintenance */
  LOG_MAINT,
  /** Needs Maintenance */
  LOG_NEEDSMAINT,
  /** Write Note */
  LOG_NOTE,
  /** Dropped Travelbug */
  LOG_TB_DROPPED,
  /** Travelbug retrieved */
  LOG_TB_RETRIEVED,
  /** Travelbug grabbed */
  LOG_TB_GRABBED,
  /** Travelbug discovered */
  LOG_TB_DISCOVERED,
  /** Retract */
  LOG_RETRACT,
  /** Needs Archived */
  LOG_NEED_ARCHV,
  /** Will Attend */
  LOG_WILL_ATTEND,
  /** Not Found */
  LOG_NOT_FOUND,
  /** Found */
  LOG_FOUND,
  /** Archive */
  LOG_ARCHIVE,
  /** Unarchive */
  LOG_UNARCHIVE,
  /** Unknown */
  LOG_UNKNOWN
};

/**
 * Size of the cache container
 */
enum CacheSize {
  /** Nano: very small container, about the size of a thumb nail */
  SIZE_NANO,
  /** Micro: about the size of a 35&nbsp;mm film box */
  SIZE_MICRO,
  /** Small: about the size of a lunchbox, less than 1 liter of storage space */
  SIZE_SMALL,
  /** Regular: about the size of a shoe box */
  SIZE_REGULAR,
  /** Large: more than 20 liter of storage space */
  SIZE_LARGE,
  /** Other size (bigger or smaller)  */
  SIZE_OTHER,
  /** No size given, or not applicable (e.&nbsp;g. for Event Caches) */
  SIZE_NONE,
  /** Unknown Size (could not be determined) */
  SIZE_UNKNOWN
};

/**
 * Additional image for a cache or log
 */
struct CacheImage {
  /** Additional description */
  QString desc;
  /** File name on disk */
  QString fileName;
};

/**
 * A log message of a person who visited the cache
 */
struct LogMessage {
  /** Author of the log message, person who visited the cache */
  QString author;
  /** Log message */
  QString msg;
  /** Type of log */
  LogType type;
  /** Whether the log message is (partially) encrypted (ROT13) */
  bool encrypted;
  /** Date of the log entry (should equal date found) */
  QDate * date;
  /** Additional images */
  QVector<CacheImage> * images;

  LogMessage() : date(0), images(0) {}

  ~LogMessage() {
    if(images != 0) delete images;
    if(date != 0) delete date;
  }
};

/** String to indicate invalid waypoints */
const QString WAYPOINT_INVALID = "!!!INVALID!!!";

/**
 * Waypoint of a cache
 */
struct Waypoint {
  /**
   * The waypoint, like <em>GC1Q743</em>. For additional waypoints of a cache,
   * this field stores the 2-letter prefix code. Use WAYPOINT_INVALID to
   * indicate an invalid value.
   */
  QString waypoint;
  /** Name of the waypoint, like <em>Wayward Drive!</em> */
  QString name;
  /** Coordinate of the waypoint, or @c 0 if not set */
  Coordinate * coord;
  /** Type of the waypoint */
  WaypointType type;
  /** Cache description */
  QString desc;

  Waypoint() : coord(0) {}

  ~Waypoint() {
    if(coord != 0) delete coord;
  }
};

/**
 * Description of a cache.
 * Use the @c name member of the Waypoint structure to store the cache name,
 * and the @c desc member to store the cache description.
 */
struct Cache : Waypoint {
  /** Short description (plain text) */
  QString shortDesc;
  /** Size of the cache container */
  CacheSize size;
  /**
   * Difficulty rating, ranging from 1 to 9. As steps of 0.5 are allowed, this
   * value stores the rating multiplied by @c 2 to save storage space.
   */
  unsigned int difficulty;
  /**
   * Terrain rating, ranging from 1 to 9. As steps of 0.5 are allowed, this
   * value stores the rating multiplied by @c 2 to save storage space.
   */
  unsigned int terrain;
  /** Date the cache was placed */
  QDate * placed;
  /** Date the cache was found, 0 if not found */
  QDate * found;
  /** Person who placed the cache */
  QString owner;
  /** Additional waypoints */
  QVector<Waypoint> * waypoints;
  /** Log messages */
  QVector<LogMessage> * logs;
  /** Additional attributes */
  QVector<CacheAttribute> * attrs;
  /** Hints and spoiler info, ROT13-ecrypted */
  QString hint;
  /** @c true if the cache is archived, @c false otherwise */
  bool archived;

  Cache() : placed(0), found(0), waypoints(0), logs(0), attrs(0) {}

  ~Cache() {
    if(placed != 0) delete placed;
    if(found != 0) delete found;
    if(waypoints != 0) delete waypoints;
    if(logs != 0) delete logs;
    if(attrs != 0) delete attrs;
  }
};

QDebug& operator<<(QDebug& dbg, const Cache& cache);

}
#endif // CACHE_H
