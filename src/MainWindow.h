/**
 * @file MainWindow.h
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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "OsmSlippyMap.h"
#include "GeocacheModel.h"
#include "GCSpider.h"
#include <QObject>
#include <QtGui>

namespace geojackal {

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow();
  virtual ~MainWindow();
  GCSpider * validateLogin();

protected:
  void setupActions();
  void setupMenu();

protected slots:
  int showPrefDialog();
  void importGCRegion();
  void importGCSingle();

private:
  OsmSlippyMap * pmap_;
  GeocacheModel * pModel_;

  QAction * exitAction_;
  QAction * prefAction_;
  QAction * importGCRegionAction_;
  QAction * importGCSingleAction_;
};

}
#endif // MAINWINDOW_H
