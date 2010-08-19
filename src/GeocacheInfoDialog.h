/**
 * @file GeocacheInfoDialog.h
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

#ifndef GEOCACHEINFODIALOG_H_
#define GEOCACHEINFODIALOG_H_

#include <QObject>
#include <QtGui>
#include "Geocache.h"

namespace geojackal {

/**
 * @{
 * Tab pages
 */
/** Geocache information */
class InfoTab : public QWidget {
  Q_OBJECT
public:
  InfoTab(Geocache * geocache);
  virtual ~InfoTab();
};
/** @} */

/** Geocache information dialog */
class GeocacheInfoDialog : public QDialog {
  Q_OBJECT
public:
  GeocacheInfoDialog(Geocache * geocache, QWidget * parent = 0);
  virtual ~GeocacheInfoDialog();

private:
  Geocache * geocache_;
};

QString sizeToText(GeocacheSize size);

}

#endif /* GEOCACHEINFODIALOG_H_ */
