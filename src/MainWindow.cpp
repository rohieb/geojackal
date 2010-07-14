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

  pModel = new CacheModel(this);

  // map widget as central widget
  bool ok;
  QSettings settings;
  // default: IZ coordinates
  Angle lon = settings.value("gc/centerLon", 52.273).toDouble(&ok);
  Angle lat = settings.value("gc/centerLat", 10.52524).toDouble(&ok);
  pmap = new OsmSlippyMap(Coordinate(lon, lat), 16);
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
  if(settings.value("gc/username").isNull()
    || settings.value("gc/password").isNull()) {
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
  QSettings settings;
  QVariant userName = settings.value("gc/username");
  QVariant password = settings.value("gc/password");
  if(userName.isNull() || password.isNull()) {
    showPrefDialog();
  }
  // TODO customize this
  GCSpiderDialog dialog(this);
  dialog.exec();
  Coordinate center(dialog.lon(), dialog.lat());
  float maxDist = dialog.maxDist();

  QList<Cache *> cacheList;
  GCSpider spider(userName.toString(), password.toString());
  spider.nearest(center, maxDist, cacheList);

  pModel->addCaches(cacheList);
  pmap->setCenter(center);
}
