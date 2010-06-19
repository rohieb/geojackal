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
  ret |= ( (buf.size = size()) != SIZE_NONE);
  ret |= ( (buf.difficulty = difficulty()) != 0);
  ret |= ( (buf.terrain = terrain()) != 0);
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
  QRegExp rx("<span id=\"ctl00_ContentBody_CacheName\"[^<]*>([^<]+)<\\/span>");
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
  QRegExp rx("<div[^>]*id=\"ctl00_cacheCodeWidget\"[^>]*>\\s*<p>\\s*([^<]*)"
    "<\/p");
  bool ret = (rx.indexIn(text) >= 0);
  buf = rx.cap(1);
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
  QRegExp rx("<h2.*<img[^>]*src=\"/images/WptTypes/2.gif\".*<\\/h2");
  rx.setMinimal(true);
  if(ret = (rx.indexIn(text) >= 0)) {
    buf = TYPE_TRADI;
    return true;
  }
  // Multi
  rx.setPattern("<h2.*<img[^>]*src=\"/images/WptTypes/3.gif\".*<\\/h2");
  if(ret = (rx.indexIn(text) >= 0)) {
    buf = TYPE_MULTI;
    return true;
  }
  // Project APE
  rx.setPattern("<h2.*<img[^>]*src=\"/images/WptTypes/9.gif\".*<\\/h2");
  if(ret = (rx.indexIn(text) >= 0)) {
    buf = TYPE_PROJECTAPE;
    return true;
  }
  // Mystery
  rx.setPattern("<h2.*<img[^>]*src=\"/images/WptTypes/8.gif\".*<\\/h2");
  if(ret = (rx.indexIn(text) >= 0)) {
    buf = TYPE_MYSTERY;
    return true;
  }
  // Letterbox Hybrid
  rx.setPattern("<h2.*<img[^>]*src=\"/images/WptTypes/5.gif\".*<\\/h2");
  if(ret = (rx.indexIn(text) >= 0)) {
    buf = TYPE_LETTERBOX;
    return true;
  }
  // Whereigo
  rx.setPattern("<h2.*<img[^>]*src=\"/images/WptTypes/1858.gif\".*<\\/h2");
  if(ret = (rx.indexIn(text) >= 0)) {
    buf = TYPE_WHEREIGO;
    return true;
  }
  // Event
  rx.setPattern("<h2.*<img[^>]*src=\"/images/WptTypes/6.gif\".*<\\/h2");
  if(ret = (rx.indexIn(text) >= 0)) {
    buf = TYPE_EVENT;
    return true;
  }
  // Mega Event
  rx.setPattern("<h2.*<img[^>]*src=\"/images/WptTypes/453.gif\".*<\\/h2");
  if(ret = (rx.indexIn(text) >= 0)) {
    buf = TYPE_MEGAEVENT;
    return true;
  }
  // CITO
  rx.setPattern("<h2.*<img[^>]*src=\"/images/WptTypes/13.gif\".*<\\/h2");
  if(ret = (rx.indexIn(text) >= 0)) {
    buf = TYPE_TRADI;
    return true;
  }
  // Earthcache
  rx.setPattern("<h2.*<img[^>]*src=\"/images/WptTypes/137.gif\".*<\\/h2");
  if(ret = (rx.indexIn(text) >= 0)) {
    buf = TYPE_EARTH;
    return true;
  }
  // GPS Adventure Exhibit
  rx.setPattern("<h2.*<img[^>]*src=\"/images/WptTypes/1304.gif\".*<\\/h2");
  if(ret = (rx.indexIn(text) >= 0)) {
    buf = TYPE_GAME;
    return true;
  }
  // Virtual
  rx.setPattern("<h2.*<img[^>]*src=\"/images/WptTypes/4.gif\".*<\\/h2");
  if(ret = (rx.indexIn(text) >= 0)) {
    buf = TYPE_VIRTUAL;
    return true;
  }
  // Webcam
  rx.setPattern("<h2.*<img[^>]*src=\"/images/WptTypes/11.gif\".*<\\/h2");
  if(ret = (rx.indexIn(text) >= 0)) {
    buf = TYPE_WEBCAM;
    return true;
  }
  // 10 Years Event Caches
  rx.setPattern("<h2.*<img[^>]*src=\"/images/WptTypes/3653.gif\".*<\\/h2");
  if(ret = (rx.indexIn(text) >= 0)) {
    buf = TYPE_EVENT; // no exception here :P
    return true;
  }
  // Locationless
  rx.setPattern("<h2.*<img[^>]*src=\"/images/WptTypes/12.gif\".*<\\/h2");
  if(ret = (rx.indexIn(text) >= 0)) {
    buf = TYPE_REVERSE;
    return true;
  }
  // nothing appropriate
  buf = TYPE_OTHER;
  return false;
}

/**
 * Extract the coordinates of the cache
 * @param buf Buffer to receive the coordinate. If the conversion fails, some
 *  parts of the coordinate may be @c 0.
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::coord(Coordinate& buf) {
  QRegExp rx("<span[^>]*id=\"ctl00_ContentBody_LatLon\"[^>]*>(N|S) (\\d{2})° "
    "(\\d{2}\\.\\d{3}) (W|E) (\\d{3})° (\\d{2}\\.\\d{3})<\/span");
  bool ret = (rx.indexIn(text) >= 0);

  // calc latitude
  int latdeg = rx.cap(2).toInt(&ret);
  double latmin = rx.cap(3).toDouble(&ret);
  buf.lat = Angle(latdeg, latmin);
  // if south, multiply by -1
  if(rx.cap(1) == "S") {
    buf.lat = ((double)buf.lat * -1);
    // must be -90 <= value <= 0
    Q_ASSERT(buf.lat.degree <= 0 && (buf.lat.degree >= -90 ||
      abs(buf.lat.degree + 90) < std::numeric_limits<double>::min()));
  } else {
    // must be 0 <= value <= 90
    Q_ASSERT(buf.lat.degree >= 0 && (buf.lat.degree <= 90 ||
      abs(buf.lat.degree - 90) < std::numeric_limits<double>::min()));
  }

  int londeg = rx.cap(5).toInt(&ret);
  double lonmin = rx.cap(6).toDouble(&ret);
  buf.lon = Angle(londeg, lonmin);

  // if west, multiply longitude by -1
  if(rx.cap(4) == "W") {
    buf.lon = ((double)buf.lon * -1);
    // must be -180 <= value <= 0
    ASSERT(buf.lat.degree <= 0 && (buf.lat.degree >= -180 ||
      abs(buf.lat.degree + 180) < std::numeric_limits<double>::min()));
  } else {
    // must be 0 <= value <= 180
    ASSERT(buf.lat.degree >= 0 && (buf.lat.degree <= 180 ||
      abs(buf.lat.degree - 180) < std::numeric_limits<double>::min()));
  }

  return ret;
}

/**
 * Extract the description of the cache.
 * @param buf Buffer to receive the description
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::desc(CacheDesc& buf) {
  return false;
}

/**
 * Extract the short description of the cache
 * @param buf Buffer to receive the short description
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::shortDesc(QString& buf) {
  return false;
}

/**
 * Extract the size of the cache
 * @return The size of the cache, or @c SIZE_NONE if the data could not be
 * extracted
 */
CacheSize GCCacheSpider::size() {
  return SIZE_NONE;
}

/**
 * Extract the difficulty rating of the cache
 * @return the difficulty rating multiplied by two or @c 0 if the data could
 * not be extracted
 */
unsigned int GCCacheSpider::difficulty() {
  return 0;
}

/**
 * Extract the terrain rating of a cache
 * @return the terrain rating multiplied by two or @c 0 if the data could not
 * be extracted
 */
unsigned int GCCacheSpider::terrain() {
  return 0;
}

/**
 * Extract the date when the cache was placed
 * @param buf Buffer to receive the date of the placement
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::placed(QDate& buf) {
  return false;
}

/**
 * Extract the date when the cache was found by the user
 * @param buf Buffer to receive the found date, or the null pointer if the
 * cache was not found yet
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::found(QDate& buf) {
  return false;
}

/**
 * Extract the owner of the cache
 * @param buf The name of the owner of the cache or an empty string if the data
 * could not be extracted
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::owner(QString& buf) {
  return false;
}

/**
 * Extract additional waypoints
 * @param buf a vector of additional waypoints, or the null pointer if no
 * waypoints exist or the data could be extracted
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::waypoints(QVector<Waypoint>& buf) {
  return false;
}

/**
 * Extract logs written by other users
 * @param buf a vector containing the log messages, or the null pointer if no
 * logs exist or the data could be extracted
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::logs(QVector<LogMessage>& buf) {
  return false;
}

/**
 * Extract cache attributes
 * @param buf a vector of cache attributes, or the null pointer if no attributes
 * exist or the data could be extracted
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::attrs(QVector<CacheAttribute>& buf) {
  return false;
}

/**
 * Extract hints and spoiler info
 * @param buf the hints, ROT13-encrypted, or an empty string if there are no
 * hints or the data could be extracted
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::hint(QString& buf) {
  return false;
}

/** @} */

}
