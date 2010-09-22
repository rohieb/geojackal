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

#include "ui/MainWindow.h"
#include "ui/PrefDialog.h"
#include "ui/GCSpiderDialog.h"
#include "import/GCSpider.h"
#include "logic/Geocache.h"
#include "logic/GeocacheModel.h"
#include <QSettings>

using namespace geojackal;

MainWindow::MainWindow() :
  QMainWindow(0), stack_(0), map_(0), infoPane_(0), model_(0),
  aboutAction_(0), exitAction_(0), prefAction_(0), importGCRegionAction_(0),
  importGCSingleAction_(0), detailViewAction_(0), mapViewAction_(0),
  gotoHomeAction_(0), gotoSignalMap_(0), mainViewActionGroup_(0) {

  setWindowTitle(APPNAME);
  qApp->setWindowIcon(QIcon(":/geojackal.png"));

  // setup actions and create menu
  setupActions();
  setupMenu();

  // load data
  model_ = new GeocacheModel(this);
  try {
    model_->open(g_settings->storageLocation().
      absoluteFilePath("geocaches.sqlite"));
  } catch(Failure &f) {
    QMessageBox::critical(this, "Failure", f.what());
  }

  // setup map widget
  QDir cacheDir(g_settings->storageLocation().absoluteFilePath("maps"));
  map_ = new OsmSlippyMap(this, g_settings->center(), 16, cacheDir);
  map_->setCaches(model_->geocaches());

  // setup geocache detail widget
  infoPane_ = new GeocacheInfoWidget;
  // display geocache in detail widget on click on map
  connect(map_, SIGNAL(clicked(Geocache *)), infoPane_,
    SLOT(setGeocache(Geocache *)));
  connect(map_, SIGNAL(clicked(Geocache *)), SLOT(detailView()));

  // stacked widget as central widget of the window
  stack_ = new QStackedWidget;
  stack_->addWidget(map_);
  stack_->addWidget(infoPane_);

  setCentralWidget(stack_);
  stack_->setCurrentWidget(map_);

  // select map by default
  mapView();

  // show prefs dialogue if no password or username set
  if(g_settings->gcUsername().isEmpty() || g_settings->gcPassword().isEmpty()) {
    showPrefDialog();
  }
}

MainWindow::~MainWindow() {
  if(map_) {
    delete map_;
  }
  if(model_) {
    delete model_;
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
      QMessageBox::critical(this, tr("Error"), tr("There was an error while "
        "trying to log in to geocaching.com. Maybe the user name or password "
        "you supplied is wrong.\n\nPlease fill in the correct values and try "
        "again.\n\nThe message was: ") + failMsg);

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
        if(!spider->nearest(center, maxDist, geocacheList)) {
          throw Failure(tr("Something went wrong, but I don't know what. "
            "Some geocaches could not be imported."));
        }
      } catch(Failure& f) {
        QMessageBox::critical(this, tr("Error"), f.what());
      }

      model_->addGeocaches(geocacheList);
      map_->setCaches(model_->geocaches());
      map_->setCenter(center);
    }
  }
}

/** Called when the user clicks on the Geocaches->Import single menu item */
void MainWindow::importGCSingle() {
  GCSpider * spider = validateLogin();

  if(spider) {
    bool ok;
    QString waypoint = QInputDialog::getText(this, tr("Import single geocache"),
      tr("Enter the waypoint of the geocache:"), QLineEdit::Normal, "GC", &ok);
    if(ok) {
      Geocache * pgc = new Geocache;
      try {
        spider->single(waypoint, *pgc);
      } catch(Failure& f) {
        QMessageBox::critical(this, tr("Error"), f.what());
        delete pgc;
        return;
      }
      model_->addGeocache(pgc);
      map_->setCaches(model_->geocaches());
      map_->setCenter(*pgc->coord);
      // also save in profile, like for region
      g_settings->setCenter(*pgc->coord);
    }
  }
}

/**
 * show the specified bookmark in the map
 * @param index Index of bookmark, or @c -1 for home coordinates
 */
void MainWindow::gotoBookmark(int index) {
  if(index == -1) {
    map_->setCenter(SettingsManager::instance()->center());
  }
}

/** setup the actions */
void MainWindow::setupActions() {
  QList<QKeySequence> keySeq;

  exitAction_ = new QAction(tr("&Quit"), this);
  keySeq.append(QKeySequence(Qt::CTRL | Qt::Key_Q));
  keySeq.append(QKeySequence(QKeySequence::Close));
  exitAction_->setShortcuts(keySeq);
  connect(exitAction_, SIGNAL(triggered()), qApp, SLOT(quit()));

  prefAction_ = new QAction(tr("&Preferences..."), this);
  prefAction_->setShortcut(QKeySequence(Qt::CTRL | Qt::Key_P));
  connect(prefAction_, SIGNAL(triggered()), SLOT(showPrefDialog()));

  importGCRegionAction_ = new QAction(tr("Import &region..."), this);
  connect(importGCRegionAction_, SIGNAL(triggered()), SLOT(importGCRegion()));

  importGCSingleAction_ = new QAction(tr("Import &single..."), this);
  connect(importGCSingleAction_, SIGNAL(triggered()), SLOT(importGCSingle()));

  mainViewActionGroup_ = new QActionGroup(this);

  mapViewAction_ = new QAction(tr("&Map"), mainViewActionGroup_);
  mapViewAction_->setShortcut(Qt::ALT | Qt::Key_1);
  mapViewAction_->setCheckable(true);
  connect(mapViewAction_, SIGNAL(triggered()), SLOT(mapView()));

  detailViewAction_ = new QAction(tr("Geocache &details"), mainViewActionGroup_);
  detailViewAction_->setShortcut(Qt::ALT | Qt::Key_2);
  detailViewAction_->setCheckable(true);
  connect(detailViewAction_, SIGNAL(triggered()), SLOT(detailView()));

  // wire Go To actions up to just on function which does essentially the same
  // for other bookmarks too
  gotoSignalMap_ = new QSignalMapper(this);
  gotoHomeAction_ = new QAction(tr("&Home coordinates"), this);
  gotoHomeAction_->setShortcut(Qt::Key_Home);
  gotoSignalMap_->setMapping(gotoHomeAction_, -1);
  connect(gotoHomeAction_, SIGNAL(triggered()), gotoSignalMap_, SLOT(map()));
  connect(gotoSignalMap_, SIGNAL(mapped(int)), SLOT(gotoBookmark(int)));

  aboutAction_ = new QAction(tr("A&bout..."), this);
  connect(aboutAction_, SIGNAL(triggered()), SLOT(about()));
}

/** setup the menu by inserting actions */
void MainWindow::setupMenu() {
  QMenu * appMenu = menuBar()->addMenu(tr("&Application"));
  appMenu->addAction(prefAction_);
  appMenu->addAction(exitAction_);

  QMenu * geocacheMenu = menuBar()->addMenu(tr("&Geocaches"));
  geocacheMenu->addAction(importGCRegionAction_);
  geocacheMenu->addAction(importGCSingleAction_);

  QMenu * viewMenu = menuBar()->addMenu(tr("&View"));
  viewMenu->addAction(mapViewAction_);
  viewMenu->addAction(detailViewAction_);

  QMenu * gotoMenu = menuBar()->addMenu(tr("&Places"));
  gotoMenu->addAction(gotoHomeAction_);

  QMenu * helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(aboutAction_);
}

#define STRINGIFY(x) XSTRINGIFY(x)
#define XSTRINGIFY(x) #x

/** show the about box */
void MainWindow::about() {

  class AboutBox : public QDialog {
  public:
    AboutBox(QWidget * parent = 0) : QDialog(parent) {
      // title
      setWindowTitle(tr("About %1").arg(APPNAME));

      QGridLayout * grid = new QGridLayout;

      // icon
      QLabel * iconLabel = new QLabel;
      QIcon icon = qApp->windowIcon();
      QSize size = icon.actualSize(QSize(64, 64));
      iconLabel->setPixmap(icon.pixmap(size));
      grid->addWidget(iconLabel, 0, 0, 2, 1, Qt::AlignLeft);

      // name and version
      grid->addWidget(new QLabel(QString(APPNAME) + " " + VERSION), 0, 1, 1, 1,
        Qt::AlignLeft);
      grid->addWidget(new QLabel(tr("A geocache management application")), 1,
        1, 1, 1, Qt::AlignLeft);

      // build date, revision, license in text browser
      QString gitRevision = "";
      if(!QString(STRINGIFY(GIT_REVISION)).isEmpty()) {
        gitRevision = tr(" from Git revision %1").arg(STRINGIFY(GIT_REVISION));
      }
      QString revString = tr("Built at %1 %2%3").arg(__DATE__, __TIME__,
        gitRevision);
      QTextBrowser * licenseBox = new QTextBrowser;
      licenseBox->setOpenExternalLinks(true);
      licenseBox->setHtml(QString("<p>") + revString + "</p>" +
        "<p>&copy; 2010 Roland Hieber <rohieb@rohieb.name></p>" +
        tr("<p>This program is free software: you can redistribute it and/or "
        "modify it under the terms of the GNU General Public License, version "
        "3, as published by the Free Software Foundation.</p>"
        "<p>This program is distributed in the hope that it will be useful, "
        "but WITHOUT ANY WARRANTY; without even the implied warranty of "
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU "
        "General Public License for more details.</p>"
        "<p>You should have received a copy of the GNU General Public License "
        "along with this program. If not, see <a href='http://www.gnu.org/"
        "licenses/'>http://www.gnu.org/licenses/</a>.</p>"));
      grid->addWidget(licenseBox, 2, 0, 1, 2);

      // Close button
      QDialogButtonBox * buttonBox =
        new QDialogButtonBox(QDialogButtonBox::Close);
      connect(buttonBox, SIGNAL(rejected()), this, SLOT(close()));
      grid->addWidget(buttonBox, 3, 0, 1, 2);

      setLayout(grid);
    }
  };

  AboutBox box(this);
  box.exec();
}

/** display the map view */
void MainWindow::mapView() {
  stack_->setCurrentWidget(map_);
  mapViewAction_->setChecked(true);
}

/** display the geocache detail view */
void MainWindow::detailView() {
  stack_->setCurrentWidget(infoPane_);
  detailViewAction_->setChecked(true);
}
