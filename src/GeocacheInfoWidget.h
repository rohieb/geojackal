/**
 * @file GeocacheInfoWidget.h
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

#ifndef GEOCACHEINFOWIDGET_H_
#define GEOCACHEINFOWIDGET_H_

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
  InfoTab(QWidget * parent = 0, Geocache * geocache = 0);
  virtual ~InfoTab();
  void setGeocache(Geocache * geocache = 0);

private:
  QLabel * waypoint_;
  QLabel * owner_;
  QLabel * placed_;
  QLabel * size_;
  QLabel * diff_;
  QLabel * terrain_;
};
/** @} */

/**
 * Geocache information dialog
 */
class GeocacheInfoWidget : public QWidget {
  Q_OBJECT
public:
  GeocacheInfoWidget(Geocache * geocache = 0, QWidget * parent = 0);
  virtual ~GeocacheInfoWidget();

public slots:
  void setGeocache(Geocache * geocache);

private:
  Geocache * geocache_;
  QLabel * geocacheName_;
  QLabel * geocacheIcon_;
  QTextBrowser * geocacheDescBrowser_;
  InfoTab * geocacheInfoTab_;
  QTabWidget * tab_;
  QGridLayout * mainLayout_;
};

QString sizeToText(GeocacheSize size);

}

#endif /* GEOCACHEINFOWIDGET_H_ */
