/**
 * @file CacheModel.h
 * @date 30 Apr 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#ifndef CACHEMODEL_H_
#define CACHEMODEL_H_

#include <QAbstractTableModel>

namespace geojackal {

/** Cache Model, implemented as singleton */
class CacheModel : public QAbstractTableModel {
Q_OBJECT

public:

  static QString sqlFileName = "cachestore.sqlite";

  virtual ~CacheModel();
  static CacheModel * getInstance();

  bool open();

  int rowCount(const QModelIndex &parent = QModelIndex()) const;
  QVariant data(const QModelIndex &index, int role) const;

private:
  static QSqlQuery q;
  static CacheModel * pInstance;
  CacheModel();

};

}

#endif /* CACHEMODEL_H_ */
