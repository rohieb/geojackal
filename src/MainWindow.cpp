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
  QMainWindow(0), pmap(0), pModel(0) {

  setWindowTitle("GeoJackal");
  setWindowIcon(QIcon(":/geojackal.png"));

  // load data
  pModel = new GeocacheModel(this);
  try {
    pModel->open(g_settings->storageLocation().
      absoluteFilePath("geocaches.sqlite"));
  } catch(Failure &f) {
    QMessageBox::critical(this, "Failure", f.what());
  }

  // map widget as central widget
  QDir cacheDir(g_settings->storageLocation().absoluteFilePath("maps"));
  pmap = new OsmSlippyMap(g_settings->center(), 16, cacheDir);
  pmap->setCaches(pModel->geocaches());
  setCentralWidget(pmap);
  pmap->setFocus();

  // menu
  QAction * exitAction = new QAction("&Quit", this);
  exitAction->setShortcut(QKeySequence::Close);
  connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

  QAction * prefAction = new QAction("&Preferences...", this);
  prefAction->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
  connect(prefAction, SIGNAL(triggered()), SLOT(showPrefDialog()));

  QAction * aboutAction = new QAction("&About...", this);
  connect(aboutAction, SIGNAL(triggered()), SLOT(about()));

  QAction * aboutQtAction = new QAction("A&bout Qt...", this);
  connect(aboutQtAction, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

  QMenu * appMenu = menuBar()->addMenu("&Application");
  appMenu->addAction(prefAction);
  appMenu->addAction(exitAction);
  appMenu->addAction(aboutAction);
  appMenu->addAction(aboutQtAction);

  QMenu * geocacheMenu = menuBar()->addMenu("&Geocaches");
  QAction * importRegionAction = new QAction("&Import region...", this);
  connect(importRegionAction, SIGNAL(triggered()), this,
    SLOT(importGeocacheRegion()));
  geocacheMenu->addAction(importRegionAction);

  QAction * importSingleAction = new QAction("&Import single...", this);
  connect(importSingleAction, SIGNAL(triggered()), this,
    SLOT(importSingleGeocache()));
  geocacheMenu->addAction(importSingleAction);

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


/** Called when the user clicks on the Geocaches->Import region menu item */
void MainWindow::importGeocacheRegion() {
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

      pModel->addGeocaches(geocacheList);
      pmap->setCaches(pModel->geocaches());
      pmap->setCenter(center);
    }
  }
}

/** Called when the user clicks on the Geocaches->Import single menu item */
void MainWindow::importSingleGeocache() {
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
      pModel->addGeocache(pgc);
      pmap->setCaches(pModel->geocaches());
      pmap->setCenter(*pgc->coord);
      // also save in profile, like for region
      g_settings->setCenter(*pgc->coord);
    }
  }
}

#define STRINGIFY(x) XSTRINGIFY(x)
#define XSTRINGIFY(x) #x

/** show the about box */
void MainWindow::about() {
  QString gitRevision = "";
  if(!QString(STRINGIFY(GIT_REVISION)).isEmpty()) {
    gitRevision = tr(" from Git revision %1").arg(STRINGIFY(GIT_REVISION));
  }

  QMessageBox::about(this, tr("About %1").arg(APPNAME),
    tr("%1\nVersion %2\n\n"
    "A geocache management application\n\n"
    "(C) 2010 Roland Hieber <rohieb@rohieb.name>\n\n"
    "Built at %3 %4%5\n\n"
    "This program is free software: you can redistribute it and/or modify it "
    "under the terms of the GNU General Public License, version 3, as "
    "published by the Free Software Foundation.\n\n"
    "This program is distributed in the hope that it will be useful, but "
    "WITHOUT ANY WARRANTY; without even the implied warranty of "
    "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General "
    "Public License for more details.\n\n"
    "You should have received a copy of the GNU General Public License "
    "along with this program. If not, see http://www.gnu.org/licenses/.").
    arg(APPNAME, VERSION, __DATE__, __TIME__, gitRevision));
}
