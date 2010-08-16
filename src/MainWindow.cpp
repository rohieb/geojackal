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
#include "Cache.h"
#include "GCSpider.h"
#include "CacheModel.h"
#include "GCSpiderDialog.h"
#include <QSettings>

using namespace geojackal;

MainWindow::MainWindow() :
  QMainWindow(0), pmap(0), pModel(0) {

  setWindowTitle("GeoJackal");
  setWindowIcon(QIcon(":/geojackal.png"));

  // load data
  pModel = new CacheModel(this);
  try {
    pModel->open(g_settings->storageLocation().
      absoluteFilePath("geocaches.sqlite"));
  } catch(Failure &f) {
    QMessageBox::critical(this, "Failure", f.what());
  }

  // map widget as central widget
  QDir cacheDir(g_settings->storageLocation().absoluteFilePath("maps"));
  pmap = new OsmSlippyMap(g_settings->center(), 16, cacheDir);
  pmap->setCaches(pModel->caches());
  setCentralWidget(pmap);
  pmap->setFocus();

  // menu
  QAction * exitAction = new QAction("&Quit", this);
  exitAction->setShortcut(QKeySequence::Close);
  connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

  QAction * prefAction = new QAction("&Preferences...", this);
  prefAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
  connect(prefAction, SIGNAL(triggered()), SLOT(showPrefDialog()));

  QMenu * appMenu = menuBar()->addMenu("&Application");
  appMenu->addAction(prefAction);
  appMenu->addAction(exitAction);

  QMenu * cacheMenu = menuBar()->addMenu("&Caches");
  QAction * importRegionAction = new QAction("&Import region...", this);
  connect(importRegionAction, SIGNAL(triggered()), this, SLOT(importCaches()));
  cacheMenu->addAction(importRegionAction);

  QAction * importSingleAction = new QAction("&Import single...", this);
  connect(importSingleAction, SIGNAL(triggered()), this, SLOT(importSingleCache()));
  cacheMenu->addAction(importSingleAction);

  // show prefs dialogue if no password or username set
  if(g_settings->gcUsername().isEmpty() || g_settings->gcPassword().isEmpty()) {
    showPrefDialog();
  }
}

MainWindow::~MainWindow() {
  if(pmap) {
    delete pmap;
  }
  if(pModel) {
    delete pModel;
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


/** Called when the user clicks on the Caches->Import region menu item */
void MainWindow::importCaches() {
  GCSpider * spider = validateLogin();

  if(spider) {
    GCSpiderDialog dialog(this);
    if(dialog.exec() == QDialog::Accepted) {
      Coordinate center(dialog.lat(), dialog.lon());
      float maxDist = dialog.maxDist();

      QList<Cache *> cacheList;
      try {
        spider->nearest(center, maxDist, cacheList);
      } catch(Failure& f) {
        QMessageBox::critical(this, "Error", f.what());
      }

      pModel->addCaches(cacheList);
      pmap->setCaches(pModel->caches());
      pmap->setCenter(center);
    }
  }
}

/** Called when the user clicks on the Caches->Import single menu item */
void MainWindow::importSingleCache() {
  GCSpider * spider = validateLogin();

  if(spider) {
    bool ok;
    QString waypoint = QInputDialog::getText(this, "Import cache", "Enter the "
      "waypoint of the cache:", QLineEdit::Normal, "GC", &ok);
    if(ok) {
      Cache * cache = new Cache;
      try {
        spider->loadCache(waypoint, *cache);
      } catch(Failure& f) {
        QMessageBox::critical(this, "Error", f.what());
        delete cache;
        return;
      }
      pModel->addCache(cache);
      pmap->setCaches(pModel->caches());
      pmap->setCenter(*cache->coord);
      // also save in profile, like for region
      g_settings->setCenter(*cache->coord);
    }
  }
}
