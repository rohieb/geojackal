#include "MainWindow.h"
#include "PrefDialog.h"

using namespace geojackal;

MainWindow::MainWindow() :
  QMainWindow(0), pmap(0) {

  // map widget as central widget
  // start at IZ Coordinates
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
}

void MainWindow::showPrefDialog() {
  PrefDialog prefs(this);
  prefs.exec();
}

void MainWindow::importCaches() {
  // TODO
  QMessageBox(QMessageBox::Information, "Import Caches", "foo bar");
}
