/**
 * @file Cache.h
 * @date 23 Apr 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#ifndef CACHE_H
#define CACHE_H

#include <QString>
#include <QIcon>
#include <QDateTime>
#include "Coordinate.h"

namespace geojackal {

/**
 * Generic icon with description
 */
struct DescriptedIcon {
  /** Description */
  QString desc;
  /** Icon */
  QIcon icon;
  /**
   * Constructor.
   * @param d description
   * @param i icon
   */
  DescriptedIcon(QString d = QString(), QIcon i = QIcon()) :
    desc(d), icon(i) {
  }
  virtual ~DescriptedIcon() {
  }
};

/**
 * Type of a waypoint, see @ref waypointTypes
 */
typedef DescriptedIcon WaypointType;
/**
 * @defgroup waypointTypes Icon constants for cache/waypoint types
 * @{
 */
/** Traditional Cache */
static const WaypointType TYPE_TRADI("Traditional Cache", QIcon(
  ":type-tradi.gif"));
/** Multi Cache */
static const WaypointType TYPE_MULTI("Multi Cache", QIcon(":type-multi.gif"));
/** Mystery Cache */
static const WaypointType TYPE_MYSTERY("Mystery Cache", QIcon(
  ":type-mystery.gif"));
/** Caching Event */
static const WaypointType TYPE_EVENT("Caching Event", QIcon(":type-event.gif"));
/** Virtual Cache */
static const WaypointType TYPE_VIRTUAL("Virtual Cache", QIcon(
  ":type-virtual.gif"));
/** Webcam Cache */
static const WaypointType
  TYPE_WEBCAM("Webcam Cache", QIcon(":type-webcam.gif"));
/** Mega Event */
static const WaypointType TYPE_MEGAEVENT("Mega Event", QIcon(
  ":type-megaevent.gif"));
/** Letterbox */
static const WaypointType TYPE_LETTERBOX("Letterbox", QIcon(
  ":type-letterbox.gif"));
/** Whereigo (TM) */
static const WaypointType TYPE_WHEREIGO("Whereigo (TM)", QIcon(
  ":type-whereigo.gif"));
/** Cache In Trash Out Event */
static const WaypointType TYPE_CITO("Cache In Trash Out Event", QIcon(
  ":type-cito.gif"));
/** Earth Cache */
static const WaypointType TYPE_EARTH("Earth Cache", QIcon(":type-earth.gif"));
/** Reverse Cache */
static const WaypointType TYPE_REVERSE("Reverse Cache", QIcon(
  ":type-reverse.gif"));
/** GPS Adventures Maze Exhibit */
static const WaypointType TYPE_GAME("GPS Adventures Maze Exhibit", QIcon(
  ":type-game.gif"));
/** Project A.P.E. Cache */
static const WaypointType TYPE_PROJECTAPE("Project A.P.E. Cache", QIcon(
  ":type-projectape.gif"));
/** Stage of a multicache */
static const WaypointType TYPE_STAGE("Stage of a multicache", QIcon(
  ":type-stage.gif"));
/** Final of a multicache */
static const WaypointType TYPE_FINAL("Final of a multicache", QIcon(
  ":type-final.gif"));
/** Question to answer */
static const WaypointType TYPE_QUESTION("Question to answer", QIcon(
  ":type-question.gif"));
/** Reference point */
static const WaypointType TYPE_REFERENCE("Reference point", QIcon(
  ":type-reference.gif"));
/** Parking area */
static const WaypointType TYPE_PARKING("Parking area", QIcon(
  ":type-parking.gif"));
/** Trailhead */
static const WaypointType TYPE_TRAILHEAD("Trailhead", QIcon(
  ":type-trailhead.gif"));
/** Other, not classified */
static const WaypointType TYPE_OTHER("Other, not classified", QIcon());
/** @} */

typedef DescriptedIcon CacheAttribute;
/**
 * @defgroup cacheAttributes Icon constants for cache attributes
 * @{
 */
/** Not always available */
static const CacheAttribute ATTR_AVAILABLE_NO("Not always available", QIcon(
  ":attr-available-no.gif"));
/** Always available */
static const CacheAttribute ATTR_AVAILABLE_YES("Always available", QIcon(
  ":attr-available-yes.gif"));
/** Not suitable for bike tour */
static const CacheAttribute ATTR_BICYCLES_NO("Not suitable for bike tour",
  QIcon(":attr-bicycles-no.gif"));
/** Suitable for bike tour */
static const CacheAttribute ATTR_BICYCLES_YES("Suitable for bike tour", QIcon(
  ":attr-bicycles-yes.gif"));
/** Boat required */
static const CacheAttribute ATTR_BOAT("Boat required", QIcon(":attr-boat.gif"));
/** Campfires not allowed */
static const CacheAttribute ATTR_CAMPFIRES_NO("Campfires not allowed", QIcon(
  ":attr-campfires-no.gif"));
/** Campfires allowed */
static const CacheAttribute ATTR_CAMPFIRES_YES("Campfires allowed", QIcon(
  ":attr-campfires-yes.gif"));
/** Camping not allowed */
static const CacheAttribute ATTR_CAMPING_NO("Camping not allowed", QIcon(
  ":attr-camping-no.gif"));
/** Camping allowed */
static const CacheAttribute ATTR_CAMPING_YES("Camping allowed", QIcon(
  ":attr-camping-yes.gif"));
/** Beware of falling rocks */
static const CacheAttribute ATTR_CLIFF("Beware of falling rocks", QIcon(
  ":attr-cliff.gif"));
/** Difficult climb */
static const CacheAttribute ATTR_CLIMBING_NO("Difficult climb", QIcon(
  ":attr-climbing-no.gif"));
/** Easy climb */
static const CacheAttribute ATTR_CLIMBING_YES("Easy climb", QIcon(
  ":attr-climbing-yes.gif"));
/** Beware of cattle */
static const CacheAttribute
  ATTR_COW("Beware of cattle", QIcon(":attr-cow.gif"));
/** Danger! */
static const CacheAttribute ATTR_DANGER("Danger!", QIcon(":attr-danger.gif"));
/** Dogs not allowed */
static const CacheAttribute ATTR_DOGS_NO("Dogs not allowed", QIcon(
  ":attr-dogs-no.gif"));
/** Dogs allowed */
static const CacheAttribute ATTR_DOGS_YES("Dogs allowed", QIcon(
  ":attr-dogs-yes.gif"));
/** Fee required */
static const CacheAttribute ATTR_FEE("Fee required", QIcon(":attr-fee.gif"));
/** Flashlight required */
static const CacheAttribute ATTR_FLASHLIGHT("Flashlight required", QIcon(
  ":attr-flashlight.gif"));
/** Not suitable for hiking */
static const CacheAttribute ATTR_HIKING_NO("Not suitable for hiking", QIcon(
  ":attr-hiking-no.gif"));
/** Suitable for hiking */
static const CacheAttribute ATTR_HIKING_YES("Suitable for hiking", QIcon(
  ":attr-hiking-yes.gif"));
/** Not suitable for horses */
static const CacheAttribute ATTR_HORSES_NO("Not suitable for horses", QIcon(
  ":attr-horses-no.gif"));
/** Suitable for horses */
static const CacheAttribute ATTR_HORSES_YES("Suitable for horses", QIcon(
  ":attr-horses-yes.gif"));
/** Hunting ground */
static const CacheAttribute ATTR_HUNTING("Hunting ground", QIcon(
  ":attr-hunting.gif"));
/** Not suitable for jeeps */
static const CacheAttribute ATTR_JEEPS_NO("Not suitable for jeeps", QIcon(
  ":attr-jeeps-no.gif"));
/** Suitable for jeeps */
static const CacheAttribute ATTR_JEEPS_YES("Suitable for jeeps", QIcon(
  ":attr-jeeps-yes.gif"));
/** Not suitable for kids */
static const CacheAttribute ATTR_KIDS_NO("Not suitable for kids", QIcon(
  ":attr-kids-no.gif"));
/** Suitable for kids */
static const CacheAttribute ATTR_KIDS_YES("Suitable for kids", QIcon(
  ":attr-kids-yes.gif"));
/** Needs maintenance */
static const CacheAttribute ATTR_MAINT("Needs maintenance", QIcon(
  ":attr-maint.gif"));
/** Abandoned mine */
static const CacheAttribute
  ATTR_MINE("Abandoned mine", QIcon(":attr-mine.gif"));
/** Not suitable for motorbikes */
static const CacheAttribute ATTR_MOTORCYCLES_NO("Not suitable for motorbikes",
  QIcon(":attr-motorcycles-no.gif"));
/** Suitable for motorbikes */
static const CacheAttribute ATTR_MOTORCYCLES_YES("Suitable for motorbikes",
  QIcon(":attr-motorcycles-yes.gif"));
/** Not recommended at night */
static const CacheAttribute ATTR_NIGHT_NO("Not recommended at night", QIcon(
  ":attr-night-no.gif"));
/** Recommended at night */
static const CacheAttribute ATTR_NIGHT_YES("Recommended at night", QIcon(
  ":attr-night-yes.gif"));
/** Estimated time over one hour */
static const CacheAttribute ATTR_ONEHOUR_NO("Estimated time over one hour",
  QIcon(":attr-onehour-no.gif"));
/** Estimated time below one hour */
static const CacheAttribute ATTR_ONEHOUR_YES("Estimated time below one hour",
  QIcon(":attr-onehour-yes.gif"));
/** No parking available */
static const CacheAttribute ATTR_PARKING_NO("No parking available", QIcon(
  ":attr-parking-no.gif"));
/** Parking available */
static const CacheAttribute ATTR_PARKING_YES("Parking available", QIcon(
  ":attr-parking-yes.gif"));
/** No phone available */
static const CacheAttribute ATTR_PHONE_NO("No phone available", QIcon(
  ":attr-phone-no.gif"));
/** Phone available */
static const CacheAttribute ATTR_PHONE_YES("Phone available", QIcon(
  ":attr-phone-yes.gif"));
/** Picnic area not available */
static const CacheAttribute ATTR_PICNIC_NO("Picnic area not available", QIcon(
  ":attr-picnic-no.gif"));
/** Picnic area available */
static const CacheAttribute ATTR_PICNIC_YES("Picnic area available", QIcon(
  ":attr-picnic-yes.gif"));
/** Poison oak around */
static const CacheAttribute ATTR_POISONOAK_NO("Poison oak around", QIcon(
  ":attr-poisonoak-no.gif"));
/** No poison oak around */
static const CacheAttribute ATTR_POISONOAK_YES("No poison oak around", QIcon(
  ":attr-poisonoak-yes.gif"));
/** Nearby public transportation available */
static const CacheAttribute ATTR_PUBLIC(
  "Nearby public transportation available", QIcon(":attr-public.gif"));
/** Not suitable for quads */
static const CacheAttribute ATTR_QUADS_NO("Not suitable for quads", QIcon(
  ":attr-quads-no.gif"));
/** Suitable for quads */
static const CacheAttribute ATTR_QUADS_YES("Suitable for quads", QIcon(
  ":attr-quads-yes.gif"));
/** Rapelling required */
static const CacheAttribute ATTR_RAPPELLING("Rapelling required", QIcon(
  ":attr-rappelling.gif"));
/** No restrooms available */
static const CacheAttribute ATTR_RESTROOMS_NO("No restrooms available", QIcon(
  ":attr-restrooms-no.gif"));
/** Restrooms available */
static const CacheAttribute ATTR_RESTROOMS_YES("Restrooms available", QIcon(
  ":attr-restrooms-yes.gif"));
/** Scenic view */
static const CacheAttribute ATTR_SCENIC_NO("Scenic view", QIcon(
  ":attr-scenic-no.gif"));
/** No scenic view */
static const CacheAttribute ATTR_SCENIC_YES("No scenic view", QIcon(
  ":attr-scenic-yes.gif"));
/** Scuba diving required */
static const CacheAttribute ATTR_SCUBA("Scuba diving required", QIcon(
  ":attr-scuba.gif"));
/** Beware of snakes */
static const CacheAttribute ATTR_SNAKES("Beware of snakes", QIcon(
  ":attr-snakes.gif"));
/** Not suitable for snowmobiles */
static const CacheAttribute ATTR_SNOWMOBILES_NO("Not suitable for snowmobiles",
  QIcon(":attr-snowmobiles-no.gif"));
/** Suitable for snowmobiles */
static const CacheAttribute ATTR_SNOWMOBILES_YES("Suitable for snowmobiles",
  QIcon(":attr-snowmobiles-yes.gif"));
/** No stealth required */
static const CacheAttribute ATTR_STEALTH_NO("No stealth required", QIcon(
  ":attr-stealth-no.gif"));
/** Stealth required */
static const CacheAttribute ATTR_STEALTH_YES("Stealth required", QIcon(
  ":attr-stealth-yes.gif"));
/** Not stroller accessible */
static const CacheAttribute ATTR_STROLLER_NO("Not stroller accessible", QIcon(
  ":attr-stroller-no.gif"));
/** Stroller accessible */
static const CacheAttribute ATTR_STROLLER_YES("Stroller accessible", QIcon(
  ":attr-stroller-yes.gif"));
/** Swimming required */
static const CacheAttribute ATTR_SWIMMING("Swimming required", QIcon(
  ":attr-swimming.gif"));
/** Beware of thorns */
static const CacheAttribute ATTR_THORNS("Beware of thorns", QIcon(
  ":attr-thorns.gif"));
/** Beware of ticks */
static const CacheAttribute ATTR_TICKS("Beware of ticks", QIcon(
  ":attr-ticks.gif"));
/** Wading required */
static const CacheAttribute ATTR_WADING("Wading required", QIcon(
  ":attr-wading.gif"));
/** No drinking water available */
static const CacheAttribute ATTR_WATER_NO("No drinking water available", QIcon(
  ":attr-water-no.gif"));
/** Drinking water available */
static const CacheAttribute ATTR_WATER_YES("Drinking water available", QIcon(
  ":attr-water-yes.gif"));
/** Not wheelchair accessible */
static const CacheAttribute ATTR_WHEELCHAIR_NO("Not wheelchair accessible",
  QIcon(":attr-wheelchair-no.gif"));
/** Wheelchair accessible */
static const CacheAttribute ATTR_WHEELCHAIR_YES("Wheelchair accessible", QIcon(
  ":attr-wheelchair-yes.gif"));
/** Not available in winter */
static const CacheAttribute ATTR_WINTER_NO("Not available in winter", QIcon(
  ":attr-winter-no.gif"));
/** Available in winter */
static const CacheAttribute ATTR_WINTER_YES("Available in winter", QIcon(
  ":attr-winter-yes.gif"));
/** @} */

typedef DescriptedIcon LogType;
/**
 * @defgroup logTypes  Icon constants for log types
 * @{
 */
/** Attended */
static const LogType LOG_ATTENDED("Attended", QIcon(":icon-attended.gif"));
/** Post Reviewer Note */
static const LogType LOG_REVIEWER_NOTE("Post Reviewer Note", QIcon(
  ":icon-big-smile.gif"));
/** Took a webcam photo */
static const LogType LOG_WEBCAM_PHOTO("Took a webcam photo", QIcon(
  ":icon-camera.gif"));
/** Update Coordinates */
static const LogType LOG_COORD_UPDATE("Update Coordinates", QIcon(
  ":icon-coord-update.gif"));
/** Temporarily Disable Listing  */
static const LogType LOG_DISABLED("Temporarily Disable Listing ", QIcon(
  ":icon-disabled.gif"));
/** Enable Listing */
static const LogType LOG_ENABLED("Enable Listing", QIcon(":icon-enabled.gif"));
/** Publish */
static const LogType LOG_PUBLISH("Publish", QIcon(":icon-greenlight.gif"));
/** Owner Maintenance */
static const LogType LOG_MAINT("Owner Maintenance", QIcon(":icon-maint.gif"));
/** Needs Maintenance */
static const LogType LOG_NEEDSMAINT("Needs Maintenance", QIcon(
  ":icon-needsmaint.gif"));
/** Write Note */
static const LogType LOG_NOTE("Write Note", QIcon(":icon-note.gif"));
/** Dropped Travelbug */
static const LogType LOG_TB_DROPPED("Dropped Travelbug", QIcon(
  ":icon-dropped-off.gif"));
/** Travelbug retrieved */
static const LogType LOG_TB_RETRIEVED("Travelbug retrieved", QIcon(
  ":icon-picked-up.gif"));
/** Travelbug grabbed */
static const LogType LOG_TB_GRABBED("Travelbug grabbed", QIcon(
  ":icon-transfer.gif"));
/** Travelbug discovered */
static const LogType LOG_TB_DISCOVERED("Travelbug discovered", QIcon(
  ":icon-smile.gif"));
/** Retract */
static const LogType LOG_RETRACT("Retract", QIcon(":icon-redlight.gif"));
/** Needs Archived */
static const LogType
  LOG_NEED_ARCHV("Needs Archived", QIcon(":icon-remove.gif"));
/** Will Attend */
static const LogType LOG_WILL_ATTEND("Will Attend", QIcon(":icon-rsvp.gif"));
/** Not Found */
static const LogType LOG_NOT_FOUND("Not Found", QIcon(":icon-sad.gif"));
/** Found */
static const LogType LOG_FOUND("Found", QIcon(":icon-smile.gif"));
/** Archive */
static const LogType LOG_ARCHIVE("Archive", QIcon(":icon-traffic-cone.gif"));
/** Unarchive */
static const LogType
  LOG_UNARCHIVE("Unarchive", QIcon(":icon-traffic-cone.gif"));
/** @} */

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
  /** Log icon, see @ref logTypes */
  LogType icon;
  /** Whether the log message is (partially) encrypted (ROT13) */
  bool encrypted;
  /** Additional images */
  QVector<CacheImage> images;
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
  /** Name of the waypoint, like <em>Wayward Drive!</em>*/
  QString name;
  /** Coordinate of the waypoint, or @c 0 if not set */
  Coordinate coord;
  /** Type of the waypoint, see @ref waypointTypes */
  WaypointType type;
  /** Cache description */
  CacheDesc desc;
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
  QDate placed;
  /** Date the cache was found, 0 if not found */
  QDate found;
  /** Person who placed the cache */
  QString owner;
  /** Additional waypoints */
  QVector<Waypoint> waypoints;
  /** Log messages */
  QVector<LogMessage> logs;
  /** Attributes, see @ref cacheAttributes */
  QVector<CacheAttribute> attrs;
  /** Hints and spoiler info, ROT13-ecrypted */
  QString hint;
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
