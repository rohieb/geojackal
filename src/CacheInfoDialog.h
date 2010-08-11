/**
 * @file CacheInfoDialog.h
 * @date Jul 14, 2010
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
