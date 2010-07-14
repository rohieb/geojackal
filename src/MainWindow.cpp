#include "MainWindow.h"
#include "PrefDialog.h"
#include "Cache.h"
#include "GCSpider.h"
#include "CacheModel.h"

using namespace geojackal;

MainWindow::MainWindow() :
  QMainWindow(0), pmap(0), pModel(0) {

  pModel = new CacheModel(this);

  // map widget as central widget
  // start at IZ Coordinates
  // @todo change this to profile center
  pmap = new OsmSlippyMap(
    Coordinate(Angle(52, 16, 22.79), Angle(10, 31, 30.87)), 16);
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
  QSettings settings;
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
  // TODO
  // show prefs dialogue if no password or username set
  QSettings settings;
  QVariant userName = settings.value("gc/username");
  QVariant password = settings.value("gc/password");
  if(userName.isNull() || password.isNull()) {
    showPrefDialog();
  }
  // TODO customize this
  Coordinate center(Angle(52, 16, 22.79), Angle(10, 31, 30.87));
  float maxDist = 0.2f;

  QList<Cache *> cacheList;
  GCSpider spider(userName.toString(), password.toString());
  spider.nearest(center, maxDist, cacheList);

  pModel->addCaches(cacheList);
}
