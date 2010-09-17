/**
 * @file GeocacheModel.cpp
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

#include "logic/GeocacheModel.h"
#include <QDateTime>
#include <QStringList>

using namespace geojackal;

/**
 * Convert list of attributes to string.
 * @param attrs List of attributes
 * @return A string which contains a @c 0 or a @c 1 for each GeocacheAttribute,
 *  ordered ascending.
 */
QString geojackal::attrsToString(const QVector<GeocacheAttribute>& attrs) {
  QString field(NUM_ATTRIBUTES, QChar('0'));
  foreach(GeocacheAttribute attr, attrs) {
    field[static_cast<int>(attr)] = QChar('1');
  }
  return field;
}

/**
 * Convert a string created by attrsToString() to list of attributes
 * @param str A string which contains a @c 0 or a @c 1 for each
 *  GeocacheAttribute, ordered ascending.
 * @return A list of GeoacheAttributes that are set in @a str, or empty list if
 *  the string was not of size @c NUM_ATTRIBUTES.
 */
QVector<GeocacheAttribute> geojackal::stringToAttrs(const QString str) {
  if(str.size() != NUM_ATTRIBUTES) {
    return QVector<GeocacheAttribute>(); // invalid string
  }
  QVector<GeocacheAttribute> ret;
  for(ushort i = 0; i < NUM_ATTRIBUTES; ++i) {
    if(str.at(i) == QChar('1')) {
      ret.append(static_cast<GeocacheAttribute>(i));
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
GeocacheModel::GeocacheModel(QObject * parent) {
}

GeocacheModel::~GeocacheModel() {
  // clean up geocache list
  foreach(Geocache * pgc, geocacheList) {
    delete pgc;
  }
}

/**
 * Connect to the database and load the geocaches
 * @param fileName File name of the SQLite Database
 * @throws Failure if anything goes wrong
 * @return @c true if all geocaches could be loaded, @c false otherwise
 */
bool GeocacheModel::open(const QString& fileName) {
  qDebug() << "connecting to database" << fileName;

  // create data folder if if does not eist
  QDir dir = QDir(fileName + "/.."); // trim file name
  dir.makeAbsolute();
  qDebug() << "dir" << dir.absolutePath();
  if(!dir.exists()) {
    if(!dir.mkpath(dir.absolutePath())) {
      qDebug() << "Failure: Could not mkdir" << dir.absolutePath();
      return false;
    }
  }

  // open sqlite database
  db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(fileName);
  if(!db.open()) {
    qDebug() << "Failure: " << db.lastError();
    return false;
  }

  q = QSqlQuery(db);

  // if our tables do not exist yet, create them
  QStringList tableList = db.tables(QSql::Tables);

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
  if(!tableList.contains("geocaches")) {
    if(!q.exec("CREATE TABLE geocaches("
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
      throw Failure("Failed to create table 'geocaches'! " + q.lastError().
        text() + "\nFailed query was: " + q.executedQuery());
      return false;
    }
  }

  // not needed yet and still fails
//  if(!tableList.contains("geocachewaypoints")) {
//    if(!q.exec("CREATE TABLE geocachewaypoints("
//      "waypoint TEXT PRIMARY KEY "
//      "  REFERENCES waypoints ON DELETE CASCADE ON UPDATE CASCADE,"
//      "geocache TEXT PRIMARY KEY "
//      "  REFERENCES geocaches(waypoint) ON DELETE CASCADE ON UPDATE CASCADE"
//      ");")) {
//      throw Failure("Failed to create table 'geocachewaypoints'! "
//        + q.lastError().text() + "\nFailed query was: " + q.executedQuery());
//      return false;
//    }
//  }
//  if(!tableList.contains("logs")) {
//    if(!q.exec("CREATE TABLE logs("
//      "geocache TEXT "
//      "  REFERENCES geocaches(oid) ON DELETE CASCADE ON UPDATE CASCADE,"
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
//  if(!tableList.contains("geocacheimages")) {
//    if(!q.exec("CREATE TABLE geocacheimages("
//      "geocache INTEGER "
//      "  REFERENCES geocaches(oid) ON DELETE CASCADE ON UPDATE CASCADE,"
//      "image TEXT "
//      "  REFERENCES images(oid) ON DELETE CASCADE ON UPDATE CASCADE"
//      ");")) {
//      throw Failure("Failed to create table 'geocacheimages'! "
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

  // load geocaches
  q.exec("SELECT w.waypoint,w.name,w.lat,w.lon,w.type,w.desc,c.shortdesc,"
    "c.size,c.terrain,c.difficulty,c.placed,c.found,c.owner,c.attrs,c.hint,"
    "c.archived FROM waypoints w JOIN geocaches c ON w.waypoint = c.waypoint");
  while(q.next()) {
    Geocache * geocache = new Geocache;
    bool ok;
    geocache->waypoint = q.value(0).toString();
    geocache->name = q.value(1).toString();
    geocache->coord = new Coordinate(q.value(2).toDouble(&ok),
      q.value(3).toDouble(&ok));
    geocache->type = static_cast<WaypointType>(q.value(4).toInt(&ok));
    geocache->desc = q.value(5).toString();
    geocache->shortDesc = q.value(6).toString();
    geocache->size = static_cast<GeocacheSize>(q.value(7).toInt(&ok));
    geocache->terrain = q.value(8).toInt(&ok);
    geocache->difficulty = q.value(9).toInt(&ok);
    geocache->placed = new QDate(QDateTime::fromTime_t(q.value(10).toInt(&ok)).
      date());
    geocache->found = new QDate(QDateTime::fromTime_t(q.value(11).toInt(&ok)).
      date());
    geocache->owner = q.value(12).toString();
    geocache->attrs = new QVector<GeocacheAttribute>(
      stringToAttrs(q.value(13).toString()));
    geocache->hint = q.value(13).toString();
    geocache->archived = q.value(14).toBool();

    qDebug() << "loaded" << geocache->waypoint << "from database";
    geocacheList[geocache->waypoint] = geocache;
  }
  return true;
}

/**
 * Save all changed, updated and removed geocaches to the database.
 * @return @c true if the process was successful, @c false otherwise. In the
 *  latter case, some geocaches may be saved, some not.
 */
bool GeocacheModel::save() {
  bool success = true;

  foreach(QString wp, geocacheList.keys()) {
    Geocache * geocache = geocacheList.value(wp);

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
    q.bindValue(":name", geocache->name);
    q.bindValue(":lat", static_cast<double>(geocache->coord->lat));
    q.bindValue(":lon", static_cast<double>(geocache->coord->lon));
    q.bindValue(":type", static_cast<int>(geocache->type));
    q.bindValue(":desc", geocache->desc);
    q.bindValue(":waypoint", geocache->waypoint);
    if(!(success = q.exec())) {
      throw Failure("Error while trying to save to SQL table 'waypoints': " +
        q.lastError().text() + "\nFailed query was: " + q.executedQuery());
    }
    q.finish();

    // table 'geocaches'
    if(alreadyExists) {
      q.prepare("UPDATE geocaches SET shortdesc = :shortdesc, size = :size, "
        "terrain = :terrain, difficulty = :difficulty, placed = :placed, "
        "found = :found, owner = :owner, attrs = :attrs, hint = :hint, "
        "archived = :archived WHERE waypoint = :waypoint");
    } else {
      q.prepare("INSERT INTO geocaches (waypoint, shortdesc, size, terrain, "
        "difficulty, placed, found, owner, attrs, hint, archived) "
        "VALUES(:waypoint, :shortdesc, :size, :terrain, :difficulty, :placed, "
        ":found, :owner, :attrs, :hint, :archived);");
    }
    q.bindValue(":waypoint", geocache->waypoint);
    q.bindValue(":shortdesc", geocache->shortDesc);
    q.bindValue(":size", geocache->size);
    q.bindValue(":terrain", geocache->terrain);
    q.bindValue(":difficulty", geocache->difficulty);
    q.bindValue(":placed", QDateTime(*geocache->placed).toUTC().toTime_t());
    q.bindValue(":found", QDateTime(*geocache->found).toUTC().toTime_t());
    q.bindValue(":owner", geocache->owner);
    q.bindValue(":attrs", attrsToString(*geocache->attrs));
    q.bindValue(":hint", geocache->hint);
    q.bindValue(":archived", geocache->archived);
    if(!(success = q.exec())) {
      throw Failure("Error while trying to save to SQL table 'geocaches': " +
        q.lastError().text() + "\nFailed query was: " + q.executedQuery());
    }
  }

  return success;
}

///** from QAbstractItemModel: get number of elements in the model */
//int GeocacheModel::rowCount(const QModelIndex &parent) const {
//  return geocacheList.size();
//}
//
///** from QAbstractItemModel: get data in the model */
//QVariant GeocacheModel::data(const QModelIndex &index, int role) const {
//  if(!index.isValid()) {
//    return QVariant();
//  }
//  if(index.row() >= geocacheList.size()) {
//    return QVariant();
//  }
//
//  if(role == Qt::DisplayRole) {
//    return geocacheList.at(index.row());
//  } else {
//    return QVariant();
//  }
//}

/**
 * Add geocaches to the database. Because geocaches are indexed by their 
 * waypoint, already existent geocaches with the same waypoint are overwritten.
 * @param geocaches A list of geocaches. You do not have to care about the 
 *  elements of this list to be deleted, they are automagically deleted if no 
 *  longer needed.
 */
void GeocacheModel::addGeocaches(QList<Geocache *>& geocaches) {
  foreach(Geocache * geocache, geocaches) {
    geocacheList[geocache->waypoint] = geocache;
  }
  save();
}

/**
 * Add a single geocache to the database. Because geocaches are indexed by 
 * their waypoint, already existent geocaches with the same waypoint are 
 * overwritten.
 * @param geocache The geocache. You do not have to care about the pointer to be
 *  deleted, it is automagically deleted if no longer needed.
 */
void GeocacheModel::addGeocache(Geocache * geocache) {
  geocacheList[geocache->waypoint] = geocache;
  save();
}
/**
 * Get list of geocaches
 */
QList<Geocache *> GeocacheModel::geocaches() const {
  return geocacheList.values();
}
