/**
 * @file Cache.h
 * @date 23 Apr 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#ifndef CACHE_H
#define CACHE_H

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
  ATTR_AVAILABLE_NO,
  /** Always available */
  ATTR_AVAILABLE_YES,
  /** Not suitable for bike tour */
  ATTR_BICYCLES_NO,
  /** Suitable for bike tour */
  ATTR_BICYCLES_YES,
  /** Boat required */
  ATTR_BOAT,
  /** Campfires not allowed */
  ATTR_CAMPFIRES_NO,
  /** Campfires allowed */
  ATTR_CAMPFIRES_YES,
  /** Camping not allowed */
  ATTR_CAMPING_NO,
  /** Camping allowed */
  ATTR_CAMPING_YES,
  /** Beware of falling rocks */
  ATTR_CLIFF,
  /** Difficult climb */
  ATTR_CLIMBING_NO,
  /** Easy climb */
  ATTR_CLIMBING_YES,
  /** Beware of cattle */
  ATTR_COW,
  /** Danger! */
  ATTR_DANGER,
  /** Dogs not allowed */
  ATTR_DOGS_NO,
  /** Dogs allowed */
  ATTR_DOGS_YES,
  /** Fee required */
  ATTR_FEE,
  /** Flashlight required */
  ATTR_FLASHLIGHT,
  /** Not suitable for hiking */
  ATTR_HIKING_NO,
  /** Suitable for hiking */
  ATTR_HIKING_YES,
  /** Not suitable for horses */
  ATTR_HORSES_NO,
  /** Suitable for horses */
  ATTR_HORSES_YES,
  /** Hunting ground */
  ATTR_HUNTING,
  /** Not suitable for jeeps */
  ATTR_JEEPS_NO,
  /** Suitable for jeeps */
  ATTR_JEEPS_YES,
  /** Not suitable for kids */
  ATTR_KIDS_NO,
  /** Suitable for kids */
  ATTR_KIDS_YES,
  /** Needs maintenance */
  ATTR_MAINT,
  /** Abandoned mine */
  ATTR_MINE,
  /** Not suitable for motorbikes */
  ATTR_MOTORCYCLES_NO,
  /** Suitable for motorbikes */
  ATTR_MOTORCYCLES_YES,
  /** Not recommended at night */
  ATTR_NIGHT_NO,
  /** Recommended at night */
  ATTR_NIGHT_YES,
  /** Estimated time over one hour */
  ATTR_ONEHOUR_NO,
  /** Estimated time below one hour */
  ATTR_ONEHOUR_YES,
  /** No parking available */
  ATTR_PARKING_NO,
  /** Parking available */
  ATTR_PARKING_YES,
  /** No phone available */
  ATTR_PHONE_NO,
  /** Phone available */
  ATTR_PHONE_YES,
  /** Picnic area not available */
  ATTR_PICNIC_NO,
  /** Picnic area available */
  ATTR_PICNIC_YES,
  /** Poison oak around */
  ATTR_POISONOAK_NO,
  /** No poison oak around */
  ATTR_POISONOAK_YES,
  /** Nearby public transportation available */
  ATTR_PUBLIC,
  /** Not suitable for quads */
  ATTR_QUADS_NO,
  /** Suitable for quads */
  ATTR_QUADS_YES,
  /** Rapelling required */
  ATTR_RAPPELLING,
  /** No restrooms available */
  ATTR_RESTROOMS_NO,
  /** Restrooms available */
  ATTR_RESTROOMS_YES,
  /** Scenic view */
  ATTR_SCENIC_NO,
  /** No scenic view */
  ATTR_SCENIC_YES,
  /** Scuba diving required */
  ATTR_SCUBA,
  /** Beware of snakes */
  ATTR_SNAKES,
  /** Not suitable for snowmobiles */
  ATTR_SNOWMOBILES_NO,
  /** Suitable for snowmobiles */
  ATTR_SNOWMOBILES_YES,
  /** No stealth required */
  ATTR_STEALTH_NO,
  /** Stealth required */
  ATTR_STEALTH_YES,
  /** Not stroller accessible */
  ATTR_STROLLER_NO,
  /** Stroller accessible */
  ATTR_STROLLER_YES,
  /** Swimming required */
  ATTR_SWIMMING,
  /** Beware of thorns */
  ATTR_THORNS,
  /** Beware of ticks */
  ATTR_TICKS,
  /** Wading required */
  ATTR_WADING,
  /** No drinking water available */
  ATTR_WATER_NO,
  /** Drinking water available */
  ATTR_WATER_YES,
  /** Not wheelchair accessible */
  ATTR_WHEELCHAIR_NO,
  /** Wheelchair accessible */
  ATTR_WHEELCHAIR_YES,
  /** Not available in winter */
  ATTR_WINTER_NO,
  /** Available in winter */
  ATTR_WINTER_YES
};

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

/**
 * Cache Description, can be plain text or HTML
 */
struct CacheDesc {
  /** Descriptive text, notes */
  QString desc;
  /** Is the description in HTML format? */
  bool descHtml;
};

/**
 * Waypoint of a cache
 */
struct Waypoint {
  /**
   * The waypoint, like <em>GC1Q743</em>. For additional waypoints of a cache,
   * this field stores the 2-letter prefix code
   */
  QString waypoint;
  /** Name of the waypoint, like <em>Wayward Drive!</em> */
  QString name;
  /** Coordinate of the waypoint, or @c 0 if not set */
  Coordinate * coord;
  /** Type of the waypoint, see @ref waypointTypes */
  WaypointType type;
  /** Cache description */
  CacheDesc * desc;

  Waypoint() : coord(0), desc(0) {}

  ~Waypoint() {
    if(coord != 0) delete coord;
    if(desc != 0) delete desc;
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
  /** Attributes, see @ref cacheAttributes */
  QVector<CacheAttribute> * attrs;
  /** Hints and spoiler info, ROT13-ecrypted */
  QString hint;
  /** @c true if the cache is archived, @false otherwise */
  bool archived;

  Cache() : placed(0), found(0), waypoints(0), logs(0), attrs(0) {}

  ~Cache() {
    if(placed != 0) delete placed;
    if(found != 0) delete found;
    if(waypoints != 0) delete waypoints;
    if(logs != 0) delete found;
    if(attrs != 0) delete attrs;
  }
};

/**
 * ROT13 function to encode/decode a cache hint. Due to the nature of ROT13,
 * both these operations are absolutely equivalent.
 * @param Text to be encoded or decoded
 * @return Encoded/decoded text
 */
QString rot13(QString& text) {
  int n = text.length();
  QString out = text;
  while(n--) {
    if((text.at(n) >= QChar('a') && text.at(n) <= QChar('m')) ||
      (text.at(n) >= QChar('A') && text.at(n) <= QChar('M'))) {
      // letters between a/A and m/M
      out[n] = QChar(text.at(n).toAscii() + 13);
    } else if((text.at(n) > QChar('n') && text.at(n) <= QChar('z')) ||
      (text.at(n) >= QChar('N') && text.at(n) <= QChar('Z'))) {
      // letters between n/N and z/Z
      out[n] = QChar(text.at(n).toAscii() - 13);
    }
  }
  return out;
}

}
#endif // CACHE_H
