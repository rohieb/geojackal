/**
 * @file GCSpiderCachePage.cpp
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

#include "GCSpiderCachePage.h"
#include <limits>
#include <QRegExp>
#include <QStringList>

using namespace geojackal;

/**
 * Replace numerical HTML entities by their Unicode representations. Currently,
 * only decimal entities (like @c &252;) are converted, hexadecimal entities
 * (like @c &#xf3;) are ignored.
 */
QString geojackal::replaceHtmlEntities(QString buf) {
  QRegExp rx("&#(\\d+);");
  int curPos = 0;
  while((curPos = rx.indexIn(buf, curPos)) >= 0) {
    int len = rx.cap(0).size();
    bool ok;
    QChar rplcmnt(rx.cap(1).toInt(&ok)); // number == Unicode code point
    if(ok) {
      qDebug() << "replacing" << rx.cap(0) << "with" << rplcmnt;
      buf.replace(curPos, len, rplcmnt);
    }
    curPos += len; // move to end of current match
  }
  return buf;
}

/**
 * Constructor
 * @param text Text of the cache description page
 */
GCSpiderCachePage::GCSpiderCachePage(QString text) :
  text_(text) {
}

GCSpiderCachePage::~GCSpiderCachePage() {
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
bool GCSpiderCachePage::all(Cache& buf) const {
  bool ret = true;
  ret &= name(buf.name);
//  qDebug() << buf.name;
  ret &= waypoint(buf.waypoint);
//  qDebug() << buf.waypoint;
  ret &= type(buf.type);
//  qDebug() << buf.type;
  if(!buf.coord) buf.coord = new Coordinate;
  ret &= coord(*buf.coord);
  ret &= desc(buf.desc);
  ret &= shortDesc(buf.shortDesc);
  ret &= ((buf.size = size()) != SIZE_UNKNOWN);
  ret &= ((buf.difficulty = difficulty()) != 0);
  ret &= ((buf.terrain = terrain()) != 0);
  if(!buf.placed) buf.placed = new QDate;
  ret &= placed(*buf.placed);
  if(!buf.found) buf.found = new QDate;
  ret &= found(*buf.found);
  ret &= owner(buf.owner);
  if(!buf.waypoints) buf.waypoints = new QVector<Waypoint>;
  ret &= waypoints(*buf.waypoints);
  if(!buf.logs) buf.logs = new QVector<LogMessage>;
  ret &= logs(*buf.logs);
  if(!buf.attrs) buf.attrs = new QVector<CacheAttribute>;
  ret &= attrs(*buf.attrs);
  ret &= hint(buf.hint);
  buf.archived = archived();
  return ret;
}

/**
 * Extract only the name of a cache
 * @param buf Buffer to receives the name of the cache
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCSpiderCachePage::name(QString& buf) const {
  QRegExp rx("<span id=\"ctl00_ContentBody_CacheName\">(.*)</span");
  rx.setMinimal(true);
  bool ret = (rx.indexIn(text_) >= 0);

  // text is utf-8, but read as ascii
  QString cap = rx.cap(1);
  buf = QString::fromUtf8(cap.toAscii());

  buf = replaceHtmlEntities(buf);
  return ret;
}

/**
 * Extract only the waypoint of a cache
 * @param buf Buffer to receives the waypoint of the cache
 * @return @c false if the data could not be extracted, @c true otherwise.
 *
 */
bool GCSpiderCachePage::waypoint(QString& buf) const {
  QRegExp rx("<span .*id=\"ctl00_uxWaypointName\".*>([^<]+)</span");
  rx.setMinimal(true);
  bool ret = (rx.indexIn(text_) >= 0);
  buf = rx.cap(1).trimmed();
  return ret;
}

/**
 * Extract the type of a cache
 * @param buf Buffer to receive the type of the cache. If the extraction fails,
 * this buffer receives the value @c TYPE_OTHER.
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCSpiderCachePage::type(WaypointType& buf) const {
  // Traditional
  QRegExp rx("<h2.*<img .*src=\"/images/WptTypes/(2|3|9|8|5|1858|6|453|13|137|"
    "1304|4|11|3653|12).gif\".*</h2");
  rx.setMinimal(true);
  if(rx.indexIn(text_) >= 0 && !rx.cap(1).isEmpty()) {
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
    } else if(cap == "13") { /** @todo no CITO icon yet */
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
bool GCSpiderCachePage::coord(Coordinate& buf) const {
  QRegExp rx("<span [^i]*id=\"ctl00_ContentBody_LatLon\"[^>]*>(?:(\\?{1,3}|N|S)"
    "\\s+(\\d{2})°\\s+(\\d{2}\\.\\d{3})\\s+(W|E)\\s+(\\d{3})°\\s+(\\d{2}\\."
    "\\d{3}))</span");
  if(rx.indexIn(text_) >= 0 && !rx.cap(1).isEmpty()) {
    // is the coordinate "???" or something real?
    if(rx.cap(1).at(0) == QChar('?')) {
      // invisible coordinate
      buf.lat = ANGLE_INVALID;
      buf.lon = ANGLE_INVALID;
      return true;

    } else if(!rx.cap(2).isEmpty() && !rx.cap(3).isEmpty()
      && !rx.cap(4).isEmpty() && !rx.cap(5).isEmpty() && !rx.cap(6).isEmpty()) {
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
        if(!(-90 <= buf.lat.degree && buf.lat.degree <= 0)) {
          return false;
        }
      } else {
        // must be 0 <= value <= 90
        if(!(0 <= buf.lat.degree && buf.lat.degree <= 90)) {
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
        if(!(-180 <= buf.lat.degree && buf.lat.degree <= 0)) {
          return false;
        }
      } else {
        // must be 0 <= value <= 180
        if(!(0 <= buf.lat.degree && buf.lat.degree <= 180)) {
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
bool GCSpiderCachePage::desc(QString& buf) const {
  QRegExp startRx("(<span .*id=\"ctl00_ContentBody_LongDescription\"[^>]*>)");
  QRegExp endRx("</span>\\s*</div>\\s*<p>\\s*</p>.*<div .*class=\""
    "CacheDetailNavigationWidget\"");
  startRx.setMinimal(true);
  endRx.setMinimal(true);
  int start = text_.indexOf(startRx) + startRx.cap(1).length(); // after regex
  int end = text_.indexOf(endRx);

  // text is utf-8, but read as ascii
  buf = QString::fromUtf8(text_.mid(start, end - start).toAscii());

  // tidy up
  buf = replaceHtmlEntities(buf);
  buf.replace(QRegExp("[\n\r\t ]+"), " ");

  // @todo process images
  // currently: remove image tags from description
  QRegExp imgTag("<img .*>");
  imgTag.setMinimal(true);
  buf.replace(imgTag, "");

  // @todo something like html tidy, at least delete multiple empty <p>,<br/>
  // @todo process links to other caches?

  return (start != -1 && end != -1 && start < end);
}

/**
 * Extract the short description of the cache
 * @param buf Buffer to receive the short description
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCSpiderCachePage::shortDesc(QString& buf) const {
  QRegExp rx("<span .*id=\"ctl00_ContentBody_ShortDescription\"[^>]*>(.+)"
    "</span");
  rx.setMinimal(true);
  bool ret = (rx.indexIn(text_) >= 0);
  buf = rx.cap(1).trimmed();
  return ret;
}

/**
 * Extract the size of the cache
 * @return The size of the cache, or @c SIZE_UNKNOWN if the data could not be
 * extracted
 */
CacheSize GCSpiderCachePage::size() const {
  QRegExp rx("<img .*src=\"/images/icons/container/(micro|small|regular|large|"
    "other|not_chosen|virtual).gif\"");
  rx.setMinimal(true);
  if(rx.indexIn(text_) >= 0 && !rx.cap(1).isEmpty()) {
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
unsigned int GCSpiderCachePage::difficulty() const {
  QRegExp rx("Difficulty:\\s*</strong>\\s*<img .*src=\"(?:http://www.geocaching"
    ".com)?/images/stars/stars(\\d)(_5)?.gif\"");
  rx.setMinimal(true);
  if(rx.indexIn(text_) >= 0 && !rx.cap(1).isEmpty()) {
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
unsigned int GCSpiderCachePage::terrain() const {
  QRegExp rx("Terrain:\\s*</strong>\\s*<img .*src=\"(?:http://www.geocaching"
    ".com)?/images/stars/stars(\\d)(_5)?.gif\"");
  rx.setMinimal(true);
  if(rx.indexIn(text_) >= 0 && !rx.cap(1).isEmpty()) {
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
bool GCSpiderCachePage::placed(QDate& buf) const {
  QRegExp rx("<strong>\\s*(?:Hidden|Event Date)\\s*:\\s*</strong>\\s*"
    "(\\d{1,2})/(\\d{1,2})/(\\d{4})");
  rx.setMinimal(true);

  if(rx.indexIn(text_) >= 0 && !rx.cap(1).isEmpty() && !rx.cap(2).isEmpty() &&
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
 * @param buf Buffer to receive the found date, or the date 0-0-0000 if the
 * cache was not found yet
 * @return @c false if the data could not be extracted, @c true otherwise.
 */
bool GCSpiderCachePage::found(QDate& buf) const {
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
bool GCSpiderCachePage::owner(QString& buf) const {
  QRegExp rx("<td.*>\\s*<strong>\\s*A\\s*cache\\s*</strong>\\s*by\\s*"
    "<a .*>(.+)</a");
  rx.setMinimal(true);
  if(rx.indexIn(text_) >= 0 && !rx.cap(1).isEmpty()) {
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
bool GCSpiderCachePage::waypoints(QVector<Waypoint>& buf) const {
  buf.clear();

  /** @todo test! */
  QRegExp rx("<tr .*class=\"BorderBottom .*\".*>\\s*<td>.*</td>\\s*<td>\\s*"
    "<img src=\"(:?http://www.geocaching.com)/images/wpttypes/sm/(puzzle|flag|"
    "stage|pkg|waypoint|trailhead).jpg\".*>\\s*</td>\\s*<td>\\s*([a-zA-Z0-9]"
    "{1,2})\\s*</td>\\s*<td>.*</td>\\s*<td>.*</td><td>(.*)</td>\\s*" // coord
    "<td>.*</td>\\s*</tr><tr class=\"BorderBottom .*\">\\s*<td>\\s*Note:\\s*"
    "</td>\\s*<td .*>(.*)</td>\\s*</tr>"); // note
  rx.setMinimal(true);

  // walk through the document and search for chunks which match our regex
  int curPos = 0;
  while((curPos = rx.indexIn(text_, curPos)) >= 0) {
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
    GCSpiderCachePage cs(coord);
    if(!cs.coord(*wp.coord)) {
      return false;
    }

    // Note
    if(!rx.cap(4).isEmpty()) {
      wp.desc = rx.cap(4).trimmed();
    }

    buf.append(wp);
  }
  return true;
}

/**
 * Convert english month name to ordinal number
 * @return Ordinal number of the month or @c 0 on error
 */
int geojackal::monthToOrd(QString month) {
  if(month == "January") {
    return 1;
  } else if(month == "February") {
    return 2;
  } else if(month == "March") {
    return 3;
  } else if(month == "April") {
    return 4;
  } else if(month == "May") {
    return 5;
  } else if(month == "June") {
    return 6;
  } else if(month == "July") {
    return 7;
  } else if(month == "August") {
    return 8;
  } else if(month == "September") {
    return 9;
  } else if(month == "October") {
    return 10;
  } else if(month == "November") {
    return 11;
  } else if(month == "December") {
    return 12;
  } else {
    return 0;
  }
}

/**
 * Extract logs written by other users
 * @param buf a vector containing the log messages, or an empty vector if no
 * logs exist or the data could be extracted. In any case, the buffer is
 * cleared before extracted logs are inserted.
 * @return @c false if the data could not be extracted, @c true otherwise, even
 * if there are no log messages.
 */
bool GCSpiderCachePage::logs(QVector<LogMessage>& buf) const {
  /** @todo TEST */
  QRegExp rx("<tr>\\s*<td.*>\\s*<strong>\\s*<img .*src=\"(:?http://www."
    "geocaching.com)?/images/icons/icon_(attended|big-smile|camera|coord-"
    "update|disabled|dropped-off|enabled|greenlight|maint|needsmaint|note|"
    "picked-up|redlight|remove|rsvp|sad|smile|traffic-cone).gif\".*/>"
    "(?:&nbsp;)?(January|February|March|April|May|June|July|August|September|"
    "October|November|December)\\s+(\\d{1,2})(?:, (\\d{4}))?\\s+by\\s+<a .*>"
    "(.*)</a>\\s*</strong>\\s+(?:\\d*\\s+found)<br />\\s*<br />(.*)<br />\\s*"
    "<br /><small><a .*>View Log</a>\\s*</small>\\s*</td>\\s*</tr>");
  rx.setMinimal(true);

  int curPos = 0;
  while((curPos = rx.indexIn(text_, curPos)) >= 0) {
    curPos += rx.cap(0).size(); // move to end of current chunk
    if(rx.cap(1).isEmpty() || rx.cap(2).isEmpty() || rx.cap(3).isEmpty() ||
      rx.cap(5).isEmpty() || rx.cap(6).isEmpty()) {
      return false;
    }

    bool ok;
    LogMessage log;

    // type of log
    QString icon = rx.cap(1);
    if(icon == "attended") {
      log.type = LOG_ATTENDED;
    } else if(icon == "big-smile") {
      log.type = LOG_REVIEWER_NOTE;
    } else if(icon == "camera") {
      log.type = LOG_WEBCAM_PHOTO;
    } else if(icon == "coord-update") {
      log.type = LOG_COORD_UPDATE;
    } else if(icon == "disabled") {
      log.type = LOG_DISABLED;
    } else if(icon == "dropped-off") {
      log.type = LOG_TB_DROPPED;
    } else if(icon == "enabled") {
      log.type = LOG_ENABLED;
    } else if(icon == "greenlight") {
      log.type = LOG_PUBLISH;
    } else if(icon == "maint") {
      log.type = LOG_MAINT;
    } else if(icon == "needsmaint") {
      log.type = LOG_NEEDSMAINT;
    } else if(icon == "note") {
      log.type = LOG_NOTE;
    } else if(icon == "picked-up") {
      log.type = LOG_TB_GRABBED;
    } else if(icon == "redlight") {
      log.type = LOG_RETRACT;
    } else if(icon == "remove") {
      log.type = LOG_NEED_ARCHV;
    } else if(icon == "rsvp") {
      log.type = LOG_WILL_ATTEND;
    } else if(icon == "sad") {
      log.type = LOG_NOT_FOUND;
    } else if(icon == "smile") {
      log.type = LOG_FOUND;
    } else if(icon == "traffic-cone") {
      log.type = LOG_ARCHIVE;
    } else {
      return false;
    }

    // month
    int month = monthToOrd(rx.cap(2));
    if(month == 0) return false;

    // day
    int day = rx.cap(3).toInt(&ok);
    if(!ok) return false;

    // year, if given
    int year = QDate::currentDate().year();
    if(!rx.cap(4).isEmpty()) {
      year = rx.cap(4).toInt(&ok);
      if(!ok) return false;
    }

    // put date together
    if(log.date) delete log.date;
    log.date = new QDate(year, month, day);

    // author
    log.author = rx.cap(5);

    // log text
    log.msg = rx.cap(6);
    // @todo Parse smileys, images, encryptedc logs, links to other caches etc

    buf.append(log);
  }
  return true;
}

/**
 * Extract cache attributes
 * @param buf a vector of cache attributes, or an empty vector if no attributes
 * exist or the data could be extracted. In any case, the buffer is
 * cleared before extracted attributes are inserted.
 * @return @c false if the data could not be extracted, @c true otherwise, even
 * if there are no attributes.
 */
bool GCSpiderCachePage::attrs(QVector<CacheAttribute>& buf) const {
  QRegExp rx("<img src=\"/images/attributes/(available-yes|bicycles-yes|"
    "boat-yes|campfires-yes|camping-yes|camping-yes|cliff-yes|climbing-yes|"
    "cow-yes|danger-yes|dogs-yes|fee-yes|firstaid-yes|flashlight-yes|"
    "hiking-yes|horses-yes|hunting-yes|jeeps-yes|kids-yes|mine-yes|"
    "motorcycles-yes|night-yes|onehour-yes|parking-yes|phone-yes|picnic-yes|"
    "poisonoak-yes|public-yes|quads-yes|rappelling-yes|restrooms-yes|"
    "scenic-yes|scuba-yes|snakes-yes|snowmobiles-yes|stealth-yes|stroller-yes|"
    "swimming-yes|thorn-yes|ticks-yes|wading-yes|water-yes|wheelchair-yes|"
    "winter-yes).gif\" .*width=\"30\" height=\"30\" />");
  rx.setMinimal(true);

  int curPos = 0;
  while((curPos = rx.indexIn(text_, curPos)) >= 0) {
    curPos += rx.cap(0).size(); // move to end of current chunk
    if(rx.cap(1).isEmpty()) {
      return false;
    }

    QString sattr = rx.cap(1);
    if(sattr == "available-yes") {
      buf.append(ATTR_AVAILABLE_YES);
    } else if(sattr == "bicycles-yes") {
      buf.append(ATTR_BICYCLES_YES);
    } else if(sattr == "boat-yes") {
      buf.append(ATTR_BOAT);
    } else if(sattr == "campfires-yes") {
      buf.append(ATTR_CAMPFIRES_YES);
    } else if(sattr == "camping-yes") {
      buf.append(ATTR_CAMPING_YES);
    } else if(sattr == "cliff-yes") {
      buf.append(ATTR_CLIFF);
    } else if(sattr == "climbing-yes") {
      buf.append(ATTR_CLIMBING_YES);
    } else if(sattr == "cow-yes") {
      buf.append(ATTR_COW);
    } else if(sattr == "danger-yes") {
      buf.append(ATTR_DANGER);
    } else if(sattr == "dogs-yes") {
      buf.append(ATTR_DOGS_YES);
    } else if(sattr == "fee-yes") {
      buf.append(ATTR_FEE);
    } else if(sattr == "firstaid-yes") {
      buf.append(ATTR_MAINT);
    } else if(sattr == "flashlight-yes") {
      buf.append(ATTR_FLASHLIGHT);
    } else if(sattr == "hiking-yes") {
      buf.append(ATTR_HIKING_YES);
    } else if(sattr == "horses-yes") {
      buf.append(ATTR_HORSES_YES);
    } else if(sattr == "hunting-yes") {
      buf.append(ATTR_HUNTING);
    } else if(sattr == "jeeps-yes") {
      buf.append(ATTR_JEEPS_YES);
    } else if(sattr == "kids-yes") {
      buf.append(ATTR_KIDS_YES);
    } else if(sattr == "mine-yes") {
      buf.append(ATTR_MINE);
    } else if(sattr == "motorcycles-yes") {
      buf.append(ATTR_MOTORCYCLES_YES);
    } else if(sattr == "night-yes") {
      buf.append(ATTR_NIGHT_YES);
    } else if(sattr == "onehour-yes") {
      buf.append(ATTR_ONEHOUR_YES);
    } else if(sattr == "parking-yes") {
      buf.append(ATTR_PARKING_YES);
    } else if(sattr == "phone-yes") {
      buf.append(ATTR_PHONE_YES);
    } else if(sattr == "picnic-yes") {
      buf.append(ATTR_PICNIC_YES);
    } else if(sattr == "poisonoak-yes") {
      buf.append(ATTR_POISONOAK_YES);
    } else if(sattr == "public-yes") {
      buf.append(ATTR_PUBLIC);
    } else if(sattr == "quads-yes") {
      buf.append(ATTR_QUADS_YES);
    } else if(sattr == "rappelling-yes") {
      buf.append(ATTR_RAPPELLING);
    } else if(sattr == "restrooms-yes") {
      buf.append(ATTR_RESTROOMS_YES);
    } else if(sattr == "scenic-yes") {
      buf.append(ATTR_SCENIC_YES);
    } else if(sattr == "scuba-yes") {
      buf.append(ATTR_SCUBA);
    } else if(sattr == "snakes-yes") {
      buf.append(ATTR_SNAKES);
    } else if(sattr == "snowmobiles-yes") {
      buf.append(ATTR_SNOWMOBILES_YES);
    } else if(sattr == "stealth-yes") {
      buf.append(ATTR_STEALTH_YES);
    } else if(sattr == "stroller-yes") {
      buf.append(ATTR_STROLLER_YES);
    } else if(sattr == "swimming-yes") {
      buf.append(ATTR_SWIMMING);
    } else if(sattr == "thorn-yes") {
      buf.append(ATTR_THORNS);
    } else if(sattr == "ticks-yes") {
      buf.append(ATTR_TICKS);
    } else if(sattr == "wading-yes") {
      buf.append(ATTR_WADING);
    } else if(sattr == "water-yes") {
      buf.append(ATTR_WATER_YES);
    } else if(sattr == "wheelchair-yes") {
      buf.append(ATTR_WHEELCHAIR_YES);
    } else if(sattr == "winter-yes") {
      buf.append(ATTR_WINTER_YES);
    } else {
      return false;
    }
  }
  return true;
}

/**
 * Extract hints and spoiler info
 * @param buf the hints, ROT13-encrypted, or an empty string if there are no
 * hints or the data could be extracted
 * @return @c false if the data could not be extracted, @c true otherwise.
 * @see rot13()
 */
bool GCSpiderCachePage::hint(QString& buf) const {
  QRegExp rx("<div .*id=\"div_hint\".*>(.*)</div");
  rx.setMinimal(true);
  if(rx.indexIn(text_) >= 0 && !rx.cap(1).isEmpty()) {
    buf = rx.cap(1);
    return true;
  }
  return false;
}

/**
 * Determine whether the cache has beed archived.
 * @return the archive status. @c true means the cache has been archived,
 * @c false means the cache is still active or the data could not be extracted.
 */
bool GCSpiderCachePage::archived() const {
  return text_.indexOf("This cache has been archived, but is available for "
    "viewing for archival purposes.") > 0;
}
/** @} */
