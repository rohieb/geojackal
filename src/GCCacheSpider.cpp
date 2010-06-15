/**
 * @file GCCacheSpider.cpp
 * @date Jun 11, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#include "GCCacheSpider.h"

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

/** @group Extractor Functions */

/**
 * Extract all information from the cache page
 * @param buf Buffer to be filled. See the description of the other extractor
 * functions in this class to get information of the data in the returned Cache
 * object.
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::all(Cache& buf) {
  return false;
}

/**
 * Extract only the name of a cache
 * @param buf Buffer to receives the name of the cache
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::name(QString& buf) {
  return false;
}

/**
 * Extract only the waypoint of a cache
 * @param buf Buffer to receives the waypoint of the cache
 * @return @c false if the data could not be extracted, @c true otherwise.
 *
 */
bool GCCacheSpider::waypoint(QString& buf) {
  return false;
}

/**
 * Extract the type of a cache
 * @param buf Buffer to receive the type of the cache. See @ref waypointTypes
 * for possible returned values.
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::type(WaypointType& buf) {
  return false;
}

/**
 * Extract the coordinates of the cache
 * @param buf Buffer to receive the coordinate
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCCacheSpider::coord(Coordinate& buf) {
  return false;
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

/** @endgroup */

}
