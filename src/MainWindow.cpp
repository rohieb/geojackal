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

  // load data
  pModel = new CacheModel(this);

  // map widget as central widget
  pmap = new OsmSlippyMap(g_settings.center(), 16);
  pmap->setCaches(pModel->caches());
  pmap->setZoom(16);
  pmap->setFocus();
  setCentralWidget(pmap);

  // menu
  QAction * exitAction = new QAction("&Quit", this);
  exitAction->setShortcut(QKeySequence::Close);
  connect(exitAction, SIGNAL(triggered()), qApp, SLOT(quit()));

  QAction * prefAction = new QAction("&Preferences...", this);
  connect(prefAction, SIGNAL(triggered()), SLOT(showPrefDialog()));

  QMenu * appMenu = menuBar()->addMenu("&Application");
  appMenu->addAction(prefAction);
  appMenu->addAction(exitAction);

  QMenu * cacheMenu = menuBar()->addMenu("&Caches");
  QAction * importAction = new QAction("&Import...", this);
  connect(importAction, SIGNAL(triggered()), this, SLOT(importCaches()));
  cacheMenu->addAction(importAction);

  // show prefs dialogue if no password or username set
  if(g_settings.gcUsername().isEmpty() || g_settings.gcPassword().isEmpty()) {
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

void MainWindow::showPrefDialog() {
  PrefDialog prefs(this);
  prefs.exec();
}

void MainWindow::importCaches() {
  // show prefs dialogue if no password or username set
  QString userName = g_settings.gcUsername();
  QString password = g_settings.gcPassword();
  if(userName.isEmpty() || password.isEmpty()) {
    showPrefDialog();
  }

  GCSpiderDialog dialog(this);
  dialog.exec();
  Coordinate center(dialog.lat(), dialog.lon());
  float maxDist = dialog.maxDist();

  QList<Cache *> cacheList;
  GCSpider spider(userName, password);
  spider.nearest(center, maxDist, cacheList);

  pModel->addCaches(cacheList);
  pmap->setCaches(pModel->caches());
  pmap->setCenter(center);
}
