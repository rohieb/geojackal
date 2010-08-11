/**
 * @file CacheModel.cpp
 * @date 30 Apr 2010
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

#include "CacheModel.h"
#include <QDateTime>
#include <QStringList>

using namespace geojackal;

/**
 * Convert list of attributes to string.
 * @param attrs List of attributes
 * @return A string which contains a @c 0 or a @c 1 for each CacheAttribute,
 *  ordered ascending.
 */
QString geojackal::attrsToString(const QVector<CacheAttribute>& attrs) {
  QString field(NUM_ATTRIBUTES, QChar('0'));
  foreach(CacheAttribute attr, attrs) {
    field[static_cast<int>(attr)] = QChar('1');
  }
  return field;
}

/**
 * Convert a string created by attrsToString() to list of attributes
 * @param str A string which contains a @c 0 or a @c 1 for each CacheAttribute,
 *  ordered ascending.
 * @return A list of CacheAttributes that are set in @a str, or empty list if
 *  the string was not of size @c NUM_ATTRIBUTES.
 */
QVector<CacheAttribute> geojackal::stringToAttrs(const QString str) {
  if(str.size() != NUM_ATTRIBUTES) {
    return QVector<CacheAttribute>(); // invalid string
  }
  QVector<CacheAttribute> ret;
  for(ushort i = 0; i < NUM_ATTRIBUTES; ++i) {
    if(str.at(i) == QChar('1')) {
      ret.append(static_cast<CacheAttribute>(i));
    }
  }
  return ret;
}

/**
 * Test whether a value in a database table already exists in a column. A
 * QSqlDatabase connection must be open for this function to work.
 * @param table The database table
 * @param column The column in @a table
 * @param value The value to look for in @a column
 * @return @c true if there is a row in @a column containing @a value or @c
 *  false otherwise
 */
bool geojackal::sqlValueExists(const QString table, const QString column,
  const QString value) {
  QSqlQuery qe;
  qe.prepare(QString("SELECT * FROM %1 WHERE %2 = :value").arg(table).
    arg(column));
  qe.bindValue(":value", value);
  qe.exec();
  if(qe.next()) {
    return true;
  } else {
    return false;
  }
}

/**
 * Constructor
 */
CacheModel::CacheModel(QObject * parent) {
}

CacheModel::~CacheModel() {
  // clean up cache list
  foreach(Cache * cache, cacheList) {
    delete cache;
  }
}

/**
 * Connect to the database and load the caches
 * @param fileName File name of the SQLite Database
 * @throws Failure if anything goes wrong
 * @return @c true if all caches could be loaded, @c false otherwise
 */
bool CacheModel::open(const QString& fileName) {
  qDebug() << "connecting to database" << fileName;
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(fileName);
  if(!db.open()) {
    qDebug() << "Failure: " << db.lastError();
    return false;
  }

  q = QSqlQuery(db);

  QStringList tableList = db.tables(QSql::Tables);

  // if our tables do not exist yet, create them
  if(!tableList.contains("waypoints")) {
    if(!q.exec("CREATE TABLE waypoints("
      "waypoint TEXT PRIMARY KEY,"
      "name TEXT NOT NULL,"
      "lat REAL NOT NULL,"
      "lon REAL NOT NULL,"
      "type INTEGER NOT NULL,"
      "desc TEXT"
      ");")) {
      throw Failure("Failed to create table 'waypoints'! "
        + q.lastError().text() + "\nFailed query was: " + q.executedQuery());
      return false;
    }
  }
  if(!tableList.contains("caches")) {
    if(!q.exec("CREATE TABLE caches("
      "waypoint TEXT PRIMARY KEY "
      "  REFERENCES waypoints(oid) ON DELETE CASCADE ON UPDATE CASCADE,"
      "shortdesc TEXT,"
      "size INT NOT NULL,"
      "terrain INTEGER NOT NULL,"
      "difficulty INTEGER NOT NULL,"
      "placed INTEGER NOT NULL,"
      "found INTEGER NOT NULL,"
      "owner TEXT NOT NULL,"
      "attrs BLOB NOT NULL,"
      "hint TEXT,"
      "archived INTEGER DEFAULT 0,"
      "vote INT DEFAULT 0"
      ");")) {
      throw Failure("Failed to create table 'caches'! " + q.lastError().text()
        + "\nFailed query was: " + q.executedQuery());
      return false;
    }
  }

  // not needed yet and still fails
//  if(!tableList.contains("cachewaypoints")) {
//    if(!q.exec("CREATE TABLE cachewaypoints("
//      "waypoint TEXT PRIMARY KEY "
//      "  REFERENCES waypoints ON DELETE CASCADE ON UPDATE CASCADE,"
//      "cache TEXT PRIMARY KEY "
//      "  REFERENCES caches(waypoint) ON DELETE CASCADE ON UPDATE CASCADE"
//      ");")) {
//      throw Failure("Failed to create table 'cachewaypoints'! "
//        + q.lastError().text() + "\nFailed query was: " + q.executedQuery());
//      return false;
//    }
//  }
//  if(!tableList.contains("logs")) {
//    if(!q.exec("CREATE TABLE logs("
//      "cache TEXT "
//      "  REFERENCES caches(oid) ON DELETE CASCADE ON UPDATE CASCADE,"
//      "author TEXT NOT NULL,"
//      "type INTEGER NOT NULL,"
//      "msg TEXT NOT NULL,"
//      "encrypted INTEGER NOT NULL DEFAULT 0"
//      ");")) {
//      throw Failure("Failed to create table 'logs'! " +
//        q.lastError().text() + "\nFailed query was: " + q.executedQuery());
//      return false;
//    }
//  }
//  if(!tableList.contains("images")) {
//    if(!q.exec("CREATE TABLE images("
//      "filename TEXT PRIMARY KEY,"
//      "desc TEXT"
//      ");")) {
//      throw Failure("Failed to create table 'images'! " +
//        q.lastError().text() + "\nFailed query was: " + q.executedQuery());
//      return false;
//    }
//  }
//  if(!tableList.contains("cacheimages")) {
//    if(!q.exec("CREATE TABLE cacheimages("
//      "cache INTEGER "
//      "  REFERENCES caches(oid) ON DELETE CASCADE ON UPDATE CASCADE,"
//      "image TEXT "
//      "  REFERENCES images(oid) ON DELETE CASCADE ON UPDATE CASCADE"
//      ");")) {
//      throw Failure("Failed to create table 'cacheimages'! "
//        + q.lastError().text() + "\nFailed query was: " + q.executedQuery());
//      return false;
//    }
//  }
//  if(!tableList.contains("logimages;")) {
//    if(!q.exec("CREATE TABLE logimages("
//      "log INTEGER "
//      "  REFERENCES logs(oid) ON DELETE CASCADE ON UPDATE CASCADE,"
//      "image TEXT "
//      "  REFERENCES images(oid) ON DELETE CASCADE ON UPDATE CASCADE"
//      ");")) {
//      throw Failure("Failed to create table 'logimages'! "
//        + q.lastError().text() + "\nFailed query was: " + q.executedQuery());
//      return false;
//    }
//  }

  // load caches
  q.exec("SELECT w.waypoint,w.name,w.lat,w.lon,w.type,w.desc,c.shortdesc,"
    "c.size,c.terrain,c.difficulty,c.placed,c.found,c.owner,c.attrs,c.hint,"
    "c.archived FROM waypoints w JOIN caches c ON w.waypoint = c.waypoint");
  while(q.next()) {
    Cache * cache = new Cache;
    bool ok;
    cache->waypoint = q.value(0).toString();
    cache->name = q.value(1).toString();
    cache->coord = new Coordinate(q.value(2).toDouble(&ok),
      q.value(3).toDouble(&ok));
    cache->type = static_cast<WaypointType>(q.value(4).toInt(&ok));
    cache->desc = q.value(5).toString();
    cache->shortDesc = q.value(6).toString();
    cache->size = static_cast<CacheSize>(q.value(7).toInt(&ok));
    cache->terrain = q.value(8).toInt(&ok);
    cache->difficulty = q.value(9).toInt(&ok);
    cache->placed = new QDate(QDateTime::fromTime_t(q.value(10).toInt(&ok)).
      date());
    cache->found = new QDate(QDateTime::fromTime_t(q.value(11).toInt(&ok)).
      date());
    cache->owner = q.value(12).toString();
    cache->attrs = new QVector<CacheAttribute>(
      stringToAttrs(q.value(13).toString()));
    cache->hint = q.value(13).toString();
    cache->archived = q.value(14).toBool();

    qDebug() << "loaded" << cache->waypoint << "from database";
    cacheList[cache->waypoint] = cache;
  }
  return true;
}

/**
 * Save all changed, updated and removed caches to the database.
 * @return @c true if the process was successful, @false otherwise. In the
 *  latter case, some caches may be saved, some not.
 */
bool CacheModel::save() {
  bool success = true;

  foreach(QString wp, cacheList.keys()) {
    Cache * cache = cacheList.value(wp);

    // table 'waypoints'
    bool alreadyExists = sqlValueExists("waypoints", "waypoint", wp);
    if(alreadyExists) {
      qDebug() << "updating" << wp;
      q.prepare("UPDATE waypoints SET name = :name, lat = :lat, lon = :lon, "
        "type = :type, desc = :desc WHERE waypoint = :waypoint");
    } else {
      qDebug() << "inserting" << wp;
      q.prepare("INSERT INTO waypoints (waypoint, name, lat, lon, type, desc) "
        "VALUES (:waypoint, :name, :lat, :lon, :type, :desc);");
    }
    q.bindValue(":name", cache->name);
    q.bindValue(":lat", static_cast<double>(cache->coord->lat));
    q.bindValue(":lon", static_cast<double>(cache->coord->lon));
    q.bindValue(":type", static_cast<int>(cache->type));
    q.bindValue(":desc", cache->desc);
    q.bindValue(":waypoint", cache->waypoint);
    if(!(success = q.exec())) {
      throw Failure("Error while trying to save to SQL table 'waypoints': " +
        q.lastError().text() + "\nFailed query was: " + q.executedQuery());
    }
    q.finish();

    // table 'caches'
    if(alreadyExists) {
      q.prepare("UPDATE caches SET shortdesc = :shortdesc, size = :size, "
        "terrain = :terrain, difficulty = :difficulty, placed = :placed, "
        "found = :found, owner = :owner, attrs = :attrs, hint = :hint, "
        "archived = :archived WHERE waypoint = :waypoint");
    } else {
      q.prepare("INSERT INTO caches (waypoint, shortdesc, size, terrain, "
        "difficulty, placed, found, owner, attrs, hint, archived) "
        "VALUES(:waypoint, :shortdesc, :size, :terrain, :difficulty, :placed, "
        ":found, :owner, :attrs, :hint, :archived);");
    }
    q.bindValue(":waypoint", cache->waypoint);
    q.bindValue(":shortdesc", cache->shortDesc);
    q.bindValue(":size", cache->size);
    q.bindValue(":terrain", cache->terrain);
    q.bindValue(":difficulty", cache->difficulty);
    q.bindValue(":placed", QDateTime(*cache->placed).toUTC().toTime_t());
    q.bindValue(":found", QDateTime(*cache->found).toUTC().toTime_t());
    q.bindValue(":owner", cache->owner);
    q.bindValue(":attrs", attrsToString(*cache->attrs));
    q.bindValue(":hint", cache->hint);
    q.bindValue(":archived", cache->archived);
    if(!(success = q.exec())) {
      throw Failure("Error while trying to save to SQL table 'caches': " +
        q.lastError().text() + "\nFailed query was: " + q.executedQuery());
    }
  }

  return success;
}

///** from QAbstractItemModel: get number of elements in the model */
//int CacheModel::rowCount(const QModelIndex &parent) const {
//  return cacheList.size();
//}
//
///** from QAbstractItemModel: get data in the model */
//QVariant CacheModel::data(const QModelIndex &index, int role) const {
//  if(!index.isValid()) {
//    return QVariant();
//  }
//  if(index.row() >= cacheList.size()) {
//    return QVariant();
//  }
//
//  if(role == Qt::DisplayRole) {
//    return cacheList.at(index.row());
//  } else {
//    return QVariant();
//  }
//}

/**
 * Add geocaches to the database. Because geocaches are indexed by their 
 * waypoint, already existent geocaches with the same waypoint are overwritten.
 * @param caches A list of geocaches. You do not have to care about the 
 *  elements of this list to be deleted, they are automagically deleted if no 
 *  longer needed.
 */
void CacheModel::addCaches(QList<Cache *>& caches) {
  foreach(Cache * cache, caches) {
    cacheList[cache->waypoint] = cache;
  }
  save();
}

/**
 * Add a single geocache to the database. Because geocaches are indexed by 
 * their waypoint, already existent geocaches with the same waypoint are 
 * overwritten.
 * @param cache The geocache. You do not have to care about the pointer to be 
 *  deleted, it is automagically deleted if no longer needed.
 */
void CacheModel::addCache(Cache * cache) {
  cacheList[cache->waypoint] = cache;
  save();
}
/**
 * Get list of caches
 */
QList<Cache *> CacheModel::caches() const {
  return cacheList.values();
}
