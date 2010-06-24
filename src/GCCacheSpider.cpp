/**
 * @file GCCacheSpider.cpp
 * @date Jun 11, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#include "GCCacheSpider.h"
#include <limits>
#include <QtGlobal>

namespace geojackal {

/**
 * Constructor
 * @param text Text of the cache description page
 */
GCCacheSpider::GCCacheSpider(QString& text) :
  text(text) {
}

GCCacheSpider::~GCCacheSpider() {
}

/**
 * @defgroup Extractor Functions
 * @{
 */

/**
 * Extract all information from the cache page
 * @param buf Buffer to be filled. See the description of the other extractor
 * functions in this class to get information of the data in the returned Cache
 * object.
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::all(Cache& buf) {
  bool ret = true;
  ret |= name(buf.name);
  ret |= waypoint(buf.waypoint);
  ret |= type(buf.type);
  ret |= coord(buf.coord);
  ret |= desc(buf.desc);
  ret |= shortDesc(buf.shortDesc);
  ret |= ((buf.size = size()) != SIZE_UNKNOWN);
  ret |= ((buf.difficulty = difficulty()) != 0);
  ret |= ((buf.terrain = terrain()) != 0);
  ret |= placed(buf.placed);
  ret |= found(buf.found);
  ret |= owner(buf.owner);
  ret |= waypoints(buf.waypoints);
  ret |= logs(buf.logs);
  ret |= attrs(buf.attrs);
  ret |= hint(buf.hint);
  return ret;
}

/**
 * Extract only the name of a cache
 * @param buf Buffer to receives the name of the cache
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::name(QString& buf) {
  QRegExp rx("<span id=\"ctl00_ContentBody_CacheName\".*>(.+)</span>");
  rx.setMinimal(true);
  bool ret = (rx.indexIn(text) >= 0);
  buf = rx.cap(1);
  return ret;
}

/**
 * Extract only the waypoint of a cache
 * @param buf Buffer to receives the waypoint of the cache
 * @return @c false if the data could not be extracted, @c true otherwise.
 *
 */
bool GCCacheSpider::waypoint(QString& buf) {
  QRegExp rx("<div .*id=\"ctl00_cacheCodeWidget\"[^>]*>\\s*<p>(.+)</p");
  rx.setMinimal(true);
  bool ret = (rx.indexIn(text) >= 0);
  buf = rx.cap(1).trimmed();
  return ret;
}

/**
 * Extract the type of a cache
 * @param buf Buffer to receive the type of the cache. See @ref waypointTypes
 * for possible returned values. If the extraction fails, this buffer receives
 * the value @c TYPE_OTHER.
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::type(WaypointType& buf) {
  // Traditional
  QRegExp rx("<h2.*<img .*src=\"/images/WptTypes/(2|3|9|8|5|1858|6|453|13|137|"
    "1304|4|11|3653|12).gif\".*</h2");
  rx.setMinimal(true);
  if(rx.indexIn(text) >= 0 && !rx.cap(1).isEmpty()) {
    QString cap = rx.cap(1);
    if(cap == "2") {
      buf = TYPE_TRADI;
      return true;
    } else if(cap == "3") {
      buf = TYPE_MULTI;
      return true;
    } else if(cap == "9") {
      buf = TYPE_PROJECTAPE;
      return true;
    } else if(cap == "8") {
      buf = TYPE_MYSTERY;
      return true;
    } else if(cap == "5") {
      buf = TYPE_LETTERBOX;
      return true;
    } else if(cap == "1858") {
      buf = TYPE_WHEREIGO;
      return true;
    } else if(cap == "6") {
      buf = TYPE_EVENT;
      return true;
    } else if(cap == "453") {
      buf = TYPE_MEGAEVENT;
      return true;
    } else if(cap == "13") { // CITO
      buf = TYPE_TRADI;
      return true;
    } else if(cap == "137") {
      buf = TYPE_EARTH;
      return true;
    } else if(cap == "1304") { // GPS Adventure Exhibit
      buf = TYPE_GAME;
      return true;
    } else if(cap == "4") {
      buf = TYPE_VIRTUAL;
      return true;
    } else if(cap == "11") {
      buf = TYPE_WEBCAM;
      return true;
    } else if(cap == "3653") {
      buf = TYPE_EVENT; // 10 Years Event Caches, no exception here :P
      return true;
    } else if(cap == "12") {
      buf = TYPE_REVERSE;
      return true;
    }
  }
  // nothing appropriate
  buf = TYPE_OTHER;
  return false;
}

/**
 * Extract the coordinates of the cache
 * @param buf Buffer to receive the coordinate. If the extraction fails or
 *  the coordinate is invisible (the text says @c ??? instead of a coordinate),
 *  the @c angle fields of this value are both set to @c ANGLE_INVALID.
 * @return @c false if the data could not be extracted, @c true otherwise, even
 *  if the coordinate is invisible.
 */
bool GCCacheSpider::coord(Coordinate& buf) {
  QRegExp rx("<span .*id=\"ctl00_ContentBody_LatLon\".*>(:?\\?{1,3}|(N|S) "
    "(\\d{2})° (\\d{2}\\.\\d{3}) (W|E) (\\d{3})° (\\d{2}\\.\\d{3}))</span");
  rx.setMinimal(true);
  if(rx.indexIn(text) >= 0 && !rx.cap(1).isEmpty()) {
    // is the coordinate "???" or something real?
    if(rx.cap(1).at(0) == QChar('?')) {
      // invisible coordinate
      buf.lat = ANGLE_INVALID;
      buf.lon = ANGLE_INVALID;
      return true;

    } else if( (rx.cap(1).at(0) == QChar('N') || rx.cap(1).at(0) == QChar('S'))
      && !rx.cap(2).isEmpty() && !rx.cap(3).isEmpty() && !rx.cap(4).isEmpty()
      && !rx.cap(5).isEmpty() && !rx.cap(6).isEmpty() && !rx.cap(7).isEmpty()
      && !rx.cap(8).isEmpty()) {
      // real coordinate
      bool ret;

      // calc latitude
      int latdeg = rx.cap(2).toInt(&ret);
      if(!ret) {
        return false;
      }
      double latmin = rx.cap(3).toDouble(&ret);
      if(!ret) {
        return false;
      }
      buf.lat = Angle(latdeg, latmin);

      // if south, multiply by -1
      if(rx.cap(1) == "S") {
        buf.lat = ((double)buf.lat * -1);
        // must be -90 <= value <= 0
        if(buf.lat.degree <= 0 && (buf.lat.degree >= -90 || abs(buf.lat.degree
          + 90) < std::numeric_limits<double>::min())) {
          return false;
        }
      } else {
        // must be 0 <= value <= 90
        if(buf.lat.degree >= 0 && (buf.lat.degree <= 90 || abs(buf.lat.degree
          - 90) < std::numeric_limits<double>::min())) {
          return false;
        }
      }

      // calc longitude
      int londeg = rx.cap(5).toInt(&ret);
      if(!ret) {
        return false;
      }
      double lonmin = rx.cap(6).toDouble(&ret);
      if(!ret) {
        return false;
      }
      buf.lon = Angle(londeg, lonmin);

      // if west, multiply longitude by -1
      if(rx.cap(4) == "W") {
        buf.lon = ((double)buf.lon * -1);
        // must be -180 <= value <= 0
        if(buf.lat.degree <= 0 && (buf.lat.degree >= -180 || abs(buf.lat.degree
          + 180) < std::numeric_limits<double>::min())) {
          return false;
        }
      } else {
        // must be 0 <= value <= 180
        if(buf.lat.degree >= 0 && (buf.lat.degree <= 180 || abs(buf.lat.degree
          - 180) < std::numeric_limits<double>::min())) {
          return false;
        }
      }
      return true;
    }
  }
  return false;
}

/**
 * Extract the description of the cache.
 * @param buf Buffer to receive the description
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::desc(CacheDesc& buf) {
  QRegExp startRx("(<span .*id=\"ctl00_ContentBody_LongDescription\".*>)");
  startRx.setMinimal(true);
  QRegExp endRx("<div .*class=\"CacheDetailNavigationWidget\"");
  endRx.setMinimal(true);
  int start = text.indexOf(startRx) + startRx.cap(1).length(); // after regex
  int end = text.indexOf(endRx);

  // TODO process images
  // TODO process links to other caches?
  // TODO what about html tags in the text?

  // TODO maybe html flag is not needed?
  buf.desc = text.mid(start, end - start);
  buf.descHtml = true;
  return (start != -1 && end != -1 && start < end);
}

/**
 * Extract the short description of the cache
 * @param buf Buffer to receive the short description
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::shortDesc(QString& buf) {
  QRegExp rx("<span .*id=\"ctl00_ContentBody_ShortDescription\".*>(.+)</span");
  rx.setMinimal(true);
  bool ret = (rx.indexIn(text) >= 0);
  buf = rx.cap(1).trimmed();
  return ret;
}

/**
 * Extract the size of the cache
 * @return The size of the cache, or @c SIZE_UNKNOWN if the data could not be
 * extracted
 */
CacheSize GCCacheSpider::size() {
  QRegExp rx("<img .*src=\"/images/icons/container/(micro|small|regular|large|"
    "other|not_chosen|virtual.gif\".*>");
  rx.setMinimal(true);
  if(rx.indexIn(text) >= 0 && !rx.cap(1).isEmpty()) {
    QString cap = rx.cap(1);
    if(cap == "micro") {
      return SIZE_MICRO;
    } else if(cap == "small") {
      return SIZE_SMALL;
    } else if(cap == "regular") {
      return SIZE_REGULAR;
    } else if(cap == "large") {
      return SIZE_LARGE;
    } else if(cap == "other") {
      return SIZE_OTHER;
    } else if(cap == "not_chosen") {
      return SIZE_NONE;
    } else if(cap == "virtual") { // Virtual => Not chosen
      return SIZE_NONE;
    }
  }
  return SIZE_UNKNOWN;
}

/**
 * Extract the difficulty rating of the cache
 * @return the difficulty rating multiplied by two or @c 0 if the data could
 * not be extracted
 */
unsigned int GCCacheSpider::difficulty() {
  QRegExp rx("<strong>\\s*Difficulty:\\s*</strong>\\s*<img .*src="
    "\"(:?http://www.geocaching.com)?/images/stars/stars(\\d)(_5)?.gif\"");
  rx.setMinimal(true);
  if(rx.indexIn(text) >= 0 && !rx.cap(1).isEmpty()) {
    bool ok = false;
    int rating = rx.cap(1).toInt(&ok); // full stars
    if(ok && rating != 0) {
      if(!rx.cap(2).isEmpty()) {
        // half stars
        return 2 * rating + 1; // double value as per definition
      }
      return 2 * rating;
    }
  }
  return 0;
}

/**
 * Extract the terrain rating of a cache
 * @return the terrain rating multiplied by two or @c 0 if the data could not
 * be extracted
 */
unsigned int GCCacheSpider::terrain() {
  QRegExp rx("<strong>\\s*Terrain:\\s*</strong>\\s*<img .*src="
    "\"(:?http://www.geocaching.com)?/images/stars/stars(\\d)(_5)?.gif\"");
  rx.setMinimal(true);
  if(rx.indexIn(text) >= 0 && !rx.cap(1).isEmpty()) {
    bool ok = false;
    int rating = rx.cap(1).toInt(&ok); // full stars
    if(ok && rating != 0) {
      if(!rx.cap(2).isEmpty()) {
        // half stars
        return 2 * rating + 1; // double value as per definition
      }
      return 2 * rating;
    }
  }
  return 0;
}

/**
 * Extract the date when the cache was placed
 * @param buf Buffer to receive the date of the placement
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::placed(QDate& buf) {
  QRegExp rx("<td>\\s*<strong>\\s*Hidden\\s*:</strong>(\\d{1,2})/(\\d{1,2})/"
    "(\\d{4})\\s*</td>");
  rx.setMinimal(true);

  if(rx.indexIn(text) >= 0 && !rx.cap(1).isEmpty() && !rx.cap(2).isEmpty() &&
    !rx.cap(3).isEmpty()) {
    bool ok = true;
    int month, day, year;
    month = rx.cap(1).toInt(&ok);
    if(!ok) {
      return false;
    }
    day = rx.cap(2).toInt(&ok);
    if(!ok) {
      return false;
    }
    year = rx.cap(3).toInt(&ok);
    if(!ok) {
      return false;
    }
    buf.setDate(year, month, day);
    return true;
  }
  return false;
}

/**
 * Extract the date when the cache was found by the user
 * @param buf Buffer to receive the found date, or the null pointer if the
 * cache was not found yet
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::found(QDate& buf) {
  // FIXME: extract from logs
  buf.setDate(0,0,0);
  return false;
}

/**
 * Extract the owner of the cache
 * @param buf The name of the owner of the cache or an empty string if the data
 * could not be extracted
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::owner(QString& buf) {
  QRegExp rx("<td.*>\\s*<strong>\\s*A\\s*cache\\s*</strong>\\s*by\\s*"
    "<a .*>(.+)</a");
  rx.setMinimal(true);
  if(rx.indexIn(text) >= 0 && !rx.cap(1).isEmpty()) {
    buf = rx.cap(1).trimmed();
    return true;
  }
  return false;
}

/**
 * Extract additional waypoints
 * @param buf a vector of additional waypoints, or an empty vector if no
 * waypoints exist or the data could be extracted. In any case, the buffer is
 * cleared before extracted waypoints are inserted.
 * @return @c false if the data could not be extracted, @c true otherwise, even
 * if there are no additional waypoints.
 */
bool GCCacheSpider::waypoints(QVector<Waypoint>& buf) {
  buf.clear();

  QRegExp rx("<tr .*class=\"BorderBottom .*\".*>\\s*<td>.*</td>\\s*<td>\\s*"
    "<img src=\"(:?http://www.geocaching.com)/images/wpttypes/sm/(puzzle|flag|"
    "stage|pkg|waypoint|trailhead).jpg\".*>\\s*</td>\\s*<td>\\s*([a-zA-Z0-9]"
    "{1,2})\\s*</td>\\s*<td>.*</td>\\s*<td>.*</td><td>(.*)</td>\\s*" // coord
    "<td>.*</td>\\s*</tr><tr class=\"BorderBottom .*\">\\s*<td>\\s*Note:\\s*"
    "</td>\\s*<td .*>(.*)</td>\\s*</tr>"); // note
  rx.setMinimal(true);

  // walk through the document and search for chunks which match our regex
  int curPos = 0;
  while((curPos = rx.indexIn(text, curPos)) >= 0) {
    curPos += rx.cap(0).size(); // move to end of current chunk
    if(rx.cap(1).isEmpty() || rx.cap(2).isEmpty() || rx.cap(3).isEmpty()) {
      return false;
    }

    // Type of waypoint
    Waypoint wp;
    QString type = rx.cap(1); // puzzle|flag|stage|pkg|waypoint|trailhead
    if(type == "puzzle") {
      wp.type = TYPE_QUESTION;
    } else if(type == "flag") {
      wp.type = TYPE_FINAL;
    } else if(type == "stage") {
      wp.type = TYPE_STAGE;
    } else if(type == "pkg") {
      wp.type = TYPE_PARKING;
    } else if(type == "waypoint") {
      wp.type = TYPE_REFERENCE;
    } else if(type == "trailhead") {
      wp.type = TYPE_TRAILHEAD;
    } else {
      wp.type = TYPE_OTHER;
    }

    // Prefix
    wp.waypoint = rx.cap(2);

    // Coordinate
    QString coord = rx.cap(3);
    GCCacheSpider cs(coord);
    if(!cs.coord(wp.coord)) {
      return false;
    }

    // Note, may be HTML
    if(!rx.cap(4).isEmpty()) {
      CacheDesc desc;
      desc.descHtml = true;
      desc.desc = rx.cap(4).trimmed();
    }

    buf.append(wp);
  }
  return true;
}

/**
 * Extract logs written by other users
 * @param buf a vector containing the log messages, or an empty vector if no
 * logs exist or the data could be extracted. In any case, the buffer is
 * cleared before extracted logs are inserted.
 * @return @c false if the data could not be extracted, @c true otherwise, even
 * if there are no log messages.
 */
bool GCCacheSpider::logs(QVector<LogMessage>& buf) {
  // FIXME
  QRegExp rx("");
  rx.setMinimal(true);

  return false;
}

/**
 * Extract cache attributes
 * @param buf a vector of cache attributes, or an empty vector if no attributes
 * exist or the data could be extracted. In any case, the buffer is
 * cleared before extracted attributes are inserted.
 * @return @c false if the data could not be extracted, @c true otherwise, even
 * if there are no attributes.
 */
bool GCCacheSpider::attrs(QVector<CacheAttribute>& buf) {
  // FIXME
  QRegExp rx("");
  rx.setMinimal(true);

  return false;
}

/**
 * Extract hints and spoiler info
 * @param buf the hints, ROT13-encrypted, or an empty string if there are no
 * hints or the data could be extracted
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::hint(QString& buf) {
  QRegExp rx("<div .*id=\"div_hint\".*>(.*)</div");
  rx.setMinimal(true);
  if(rx.indexIn(text) >= 0 && !rx.cap(1).isEmpty()) {
    buf = rx.cap(1);
    return true;
  }
  return false;
}

/** @} */

}
