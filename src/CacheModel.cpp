/**
 * @file CacheModel.cpp
 * @date 30 Apr 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#include "CacheModel.h"
#include "functions.h"

namespace geojackal {

CacheModel::CacheModel() :
  pInstance(0) {
}

CacheModel::~CacheModel() {
}

/**
 * return the single instance
 */
static CacheModel CacheModel::getInstance() {
  if(pInstance == 0) {
    pInstance = new CacheModel;
    return pInstance;
  }
}
/**
 * Connect to the database
 */
bool CacheModel::open() {
  qDebug() << "connecting to database";
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName(sqlFileName);
  if(!db.open()) {
    qDebug() << "failure: " << db.lastError();
    return false;
  }

  // if our tables do not exist yet, create them
  if(!q.exec("SELECT * FROM waypoints;") && q.lastError().number() == 1) {
    if(!q.exec(
      "CREATE TABLE waypoints(\
      waypoint TEXT PRIMARY KEY,\
      name TEXT NOT NULL,\
      lat REAL NOT NULL,\
      lon REAL NOT NULL,\
      type INTEGER NOT NULL,\
      desc TEXT,\
      deschtml INTEGER\
      );")) {
      fatal("failed to create table 'waypoints'!", q.lastError().text());
      return false;
    }
  }
  if(!q.exec("SELECT * FROM caches;") && q.lastError().number() == 1) {
    if(!q.exec(
      "CREATE TABLE caches(\
      waypoint INTEGER PRIMARY KEY \
        REFERENCES waypoints(oid) ON DELETE CASCADE ON UPDATE CASCADE,\
      shortdesc TEXT,\
      size INT NOT NULL,\
      terrain INTEGER NOT NULL,\
      difficulty INTEGER NOT NULL,\
      placed INTEGER NOT NULL,\
      found INTEGER NOT NULL,\
      owner TEXT NOT NULL,\
      attrs BLOB NOT NULL,\
      hint TEXT,\
      vote INT DEFAULT 0\
      );")) {
      fatal("failed to create table 'caches'!", q.lastError().text());
      return false;
      /**  @todo tables for logs, waypoint associations */
    }
  }
  if(!q.exec("SELECT * FROM cachewaypoints;") && q.lastError().number() == 1) {
    if(!q.exec(
      "CREATE TABLE cachewaypoints(\
        waypoint TEXT PRIMARY KEY \
          REFERENCES waypoints ON DELETE CASCADE ON UPDATE CASCADE,\
        cache TEXT PRIMARY KEY \
          REFERENCES caches(waypoint) ON DELETE CASCADE ON UPDATE CASCADE,\
        );")) {
      fatal("failed to create table 'cachewaypoints'!", q.lastError().text());
      return false;
    }
  }
  if(!q.exec("SELECT * FROM logs;") && q.lastError().number() == 1) {
    if(!q.exec(
      "CREATE TABLE logs(\
        cache TEXT \
          REFERENCES caches(oid) ON DELETE CASCADE ON UPDATE CASCADE,\
        author TEXT NOT NULL,\
        msg TEXT NOT NULL,\
        encrypted INTEGER NOT NULL DEFAULT 0,\
        icon INTEGER NOT NULL,\
        );")) {
      fatal("failed to create table 'logs'!", q.lastError().text());
      return false;
    }
  }
  if(!q.exec("SELECT * FROM images;") && q.lastError().number() == 1) {
    if(!q.exec(
      "CREATE TABLE images(\
        filename TEXT PRIMARY KEY,\
        desc TEXT\
        );")) {
      fatal("failed to create table 'images'!", q.lastError().text());
      return false;
    }
  }
  if(!q.exec("SELECT * FROM cacheimages;") && q.lastError().number() == 1) {
    if(!q.exec(
      "CREATE TABLE cacheimages(\
        cache INTEGER \
          REFERENCES caches(oid) ON DELETE CASCADE ON UPDATE CASCADE,\
        image TEXT \
          REFERENCES images(oid) ON DELETE CASCADE ON UPDATE CASCADE,\
        );")) {
      fatal("failed to create table 'cacheimages'!", q.lastError().text());
      return false;
    }
  }
  if(!q.exec("SELECT * FROM logimages;") && q.lastError().number() == 1) {
    if(!q.exec(
      "CREATE TABLE logimages(\
        log INTEGER \
          REFERENCES logs(oid) ON DELETE CASCADE ON UPDATE CASCADE,\
        image TEXT \
          REFERENCES images(oid) ON DELETE CASCADE ON UPDATE CASCADE,\
        );")) {
      fatal("failed to create table 'logimages'!", q.lastError().text());
      return false;
    }
  }
}

int CacheModel::rowCount(const QModelIndex &parent = QModelIndex()) const {

}

QVariant CacheModel::data(const QModelIndex &index, int role) const {

}

}
