/**
 * @file CacheInfoDialog.h
 * @date Jul 14, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#ifndef CACHEINFODIALOG_H_
#define CACHEINFODIALOG_H_

#include <QObject>
#include <QtGui>
#include "Cache.h"

namespace geojackal {

/**
 * @{
 * Tab pages
 */
/** Geocache information */
class InfoTab : public QWidget {
  Q_OBJECT
public:
  InfoTab(Cache * cache);
  virtual ~InfoTab();
};
/** @} */

/** Geocache information dialog */
class CacheInfoDialog : public QDialog {
  Q_OBJECT
public:
  CacheInfoDialog(Cache * cache, QWidget * parent = 0);
  virtual ~CacheInfoDialog();

private:
  Cache * cache_;
};

}

#endif /* CACHEINFODIALOG_H_ */
