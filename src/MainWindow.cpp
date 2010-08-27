/**
 * @file MainWindow.cpp
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

#include "MainWindow.h"
#include "PrefDialog.h"
#include "Geocache.h"
#include "GCSpider.h"
#include "GeocacheModel.h"
#include "GCSpiderDialog.h"
#include <QSettings>

using namespace geojackal;

MainWindow::MainWindow() :
  QMainWindow(0), pmap_(0), pModel_(0) {

  setWindowTitle("GeoJackal");
  setWindowIcon(QIcon(":/geojackal.png"));

  // setup actions and create menu
  setupActions();
  setupMenu();

  // load data
  pModel_ = new GeocacheModel(this);
  try {
    pModel_->open(g_settings->storageLocation().
      absoluteFilePath("geocaches.sqlite"));
  } catch(Failure &f) {
    QMessageBox::critical(this, "Failure", f.what());
  }

  // map widget as central widget
  QDir cacheDir(g_settings->storageLocation().absoluteFilePath("maps"));
  pmap_ = new OsmSlippyMap(g_settings->center(), 16, cacheDir);
  pmap_->setCaches(pModel_->geocaches());
  setCentralWidget(pmap_);
  pmap_->setFocus();

  // show prefs dialogue if no password or username set
  if(g_settings->gcUsername().isEmpty() || g_settings->gcPassword().isEmpty()) {
    showPrefDialog();
  }
}

MainWindow::~MainWindow() {
  if(pmap_) {
    delete pmap_;
  }
  if(pModel_) {
    delete pModel_;
  }
}

/**
 * Shows the preferences dialog to the user
 * @return whatever PrefDialog::exec() returns
 */
int MainWindow::showPrefDialog() {
  PrefDialog prefs(this);
  return prefs.exec();
}

/**
 * Validate the geocaching.com login data by displaying the Preferences dialog
 * until login succeds. The login data are taken from the application settings.
 * @return A GCSpider instance, or @c 0 if the user cancelled the Preferences
 *  dialog.
 */
GCSpider * MainWindow::validateLogin() {
  GCSpider * spider = 0;

  // try to login until login succeeds or the user gets weary
  bool validLogin = false;
  while(!validLogin) {
    QString failMsg;
    try {
      spider = GCSpider::login(g_settings->gcUsername(),
        g_settings->gcPassword());
    } catch(Failure &f) {
      validLogin = false;
      failMsg = f.what();
    }
    if(spider) { // login successful
      validLogin = true; // for the accounts... *cough*
      return spider;
    }

    // show prefs dialogue if login fails
    if(!validLogin) {
      QMessageBox::critical(this, "Error", tr("There was an error while trying "
        "to log in to geocaching.com. Maybe the user name or password you "
        "supplied is wrong.\n\nPlease fill in the correct values and try again."
        "\n\nThe message was: ") + failMsg);

      if(showPrefDialog() == QDialog::Rejected) {
        return 0; // user gave up
      }
    }
  }
  Q_ASSERT(spider == 0 && !validLogin);
  return 0; // anyways, should not happen!
}


/** Called when the user clicks on the Geocaches->Import region menu item */
void MainWindow::importGCRegion() {
  GCSpider * spider = validateLogin();

  if(spider) {
    GCSpiderDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted) {
      Coordinate center(dialog.lat(), dialog.lon());
      float maxDist = dialog.maxDist();

      QList<Geocache *> geocacheList;
      try {
        spider->nearest(center, maxDist, geocacheList);
      } catch(Failure& f) {
        QMessageBox::critical(this, "Error", f.what());
      }

      pModel_->addGeocaches(geocacheList);
      pmap_->setCaches(pModel_->geocaches());
      pmap_->setCenter(center);
    }
  }
}

/** Called when the user clicks on the Geocaches->Import single menu item */
void MainWindow::importGCSingle() {
  GCSpider * spider = validateLogin();

  if(spider) {
    bool ok;
    QString waypoint = QInputDialog::getText(this, "Import single geocache",
      "Enter the waypoint of the geocache:", QLineEdit::Normal, "GC", &ok);
    if(ok) {
      Geocache * pgc = new Geocache;
      try {
        spider->single(waypoint, *pgc);
      } catch(Failure& f) {
        QMessageBox::critical(this, "Error", f.what());
        delete pgc;
        return;
      }
      pModel_->addGeocache(pgc);
      pmap_->setCaches(pModel_->geocaches());
      pmap_->setCenter(*pgc->coord);
      // also save in profile, like for region
      g_settings->setCenter(*pgc->coord);
    }
  }
}

/** setup the actions */
void MainWindow::setupActions() {
  exitAction_ = new QAction("&Quit", this);
  exitAction_->setShortcut(QKeySequence::Close);
  connect(exitAction_, SIGNAL(triggered()), qApp, SLOT(quit()));

  prefAction_ = new QAction("&Preferences...", this);
  prefAction_->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
  connect(prefAction_, SIGNAL(triggered()), SLOT(showPrefDialog()));

  importGCRegionAction_ = new QAction("&Import region...", this);
  connect(importGCRegionAction_, SIGNAL(triggered()), SLOT(importGCRegion()));

  importGCSingleAction_ = new QAction("&Import single...", this);
  connect(importGCSingleAction_, SIGNAL(triggered()), SLOT(importGCSingle()));
}

/** setup the menu by inserting actions */
void MainWindow::setupMenu() {
  QMenu * appMenu = menuBar()->addMenu("&Application");
  appMenu->addAction(prefAction_);
  appMenu->addAction(exitAction_);

  QMenu * geocacheMenu = menuBar()->addMenu("&Geocaches");
  geocacheMenu->addAction(importGCRegionAction_);
  geocacheMenu->addAction(importGCSingleAction_);
}

/** display the map view */
void MainWindow::mapView() {

}

/** display the geocache detail view */
void MainWindow::detailView() {

}

