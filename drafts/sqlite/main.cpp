/*
 * main.cpp
 *
 *  Created on: 30 Mar 2010
 *      Author: rohieb
 */

#include <QApplication>
#include <QtSql>

using namespace std;

// create table
bool createTable(QSqlQuery& q) {
  qDebug() << "creating table";
  bool
    ok =
      q.exec(
        "CREATE TABLE caches(\
      id STRING PRIMARY KEY,\
      name STRING NOT NULL,\
      difficulty SMALLINT NOT NULL,\
      terrain SMALLINT NOT NULL,\
      found DATE\
      );");
  if(!ok) {
    qDebug() << "failure:" << q.lastError();
    return false;
  }
  qDebug() << "table 'caches' successfully created";
  return true;
}

// main
int main(int argc, char * argv[]) {
  QApplication app(argc, argv);

  // connect to database
  qDebug() << "connecting to database";
  QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
  db.setDatabaseName("draft-sqlite.sqlite");
  bool ok = db.open();
  if(!ok) {
    qDebug() << "failure: " << db.lastError();
    return -1;
  }

  qDebug() << "driver does" << (db.driver()->hasFeature(QSqlDriver::QuerySize)
    ? "" : "not") << "support QuerySize";

  QSqlQuery q;

  // insert some data
  qDebug() << "insert data";
  ok = q.exec(
    "INSERT INTO caches VALUES('GC12345','Test cache',1,1,'2010-02-03')");
  if(!ok) {
    qDebug() << "failure:" << q.lastError();
    if(q.lastError().number() == 1) { // table does not exist
      if(!createTable(q)) {
        return -1;
      }
      // try again
      qDebug() << "insert data (trying again)";
      ok = q.exec(
        "INSERT INTO caches VALUES('GC12345','Test cache',1,1,'2010-02-03')");
      if(!ok) {
        qDebug() << "failure:" << q.lastError();
        return -1;
      }
    } else {
      // unknown error
      return -1;
    }
  }

  qDebug() << "successfully inserted data";

  ok = q.exec("SELECT id, name FROM caches");
  if(!ok) {
    qDebug() << "failure:" << q.lastError();
    return -1;
  }
  qDebug() << "caches in table:";
  while(q.next()) {
    QString id = q.value(0).toString();
    QString name = q.value(1).toString();
    qDebug() << "id=" << id << "; name=" << name;
  }
}
