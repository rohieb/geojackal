/**
 * @file CacheModel.h
 * @date 30 Apr 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#ifndef CACHEMODEL_H_
#define CACHEMODEL_H_

#include <QObject>
#include <QtSql>
#include <QAbstractListModel>
#include "Cache.h"

namespace geojackal {

/**
 * Cache Model. Loads the caches from a SQLite database and defines the
 * interface to access them.
 * All changes to the data are cached in memory, and not transferred to the
 * database until @a save() is called.
 */
class CacheModel : public QObject {
  Q_OBJECT
public:
  CacheModel(QObject * parent = 0);
  virtual ~CacheModel();

  bool open(const QString& fileName);
  bool save();

//  int rowCount(const QModelIndex &parent = QModelIndex()) const;
//  QVariant data(const QModelIndex &index, int role) const;
  void addCaches(QList<Cache *>& caches);
  void addCache(Cache * cache);
  QList<Cache *> caches() const;

protected:

private:
  QSqlDatabase db;
  QSqlQuery q;
  /** List of caches in the model, indexed by waypoint */
  QHash<QString, Cache *> cacheList;
};

}

#endif /* CACHEMODEL_H_ */
