#include "MainWindow.h"

MainWindow::MainWindow() :
  QMainWindow(0), pmap(0) {

  // map widget
  // start at IZ Coordinates
  pmap = new OsmSlippyMap(
    Coordinate(Angle(52, 16, 22.79), Angle(10, 31, 30.87)), 16);
  pmap->setZoom(16);
  pmap->setFocus();
  setCentralWidget(pmap);

  // menu
  QMenu * cacheMenu = menuBar()->addMenu("&Caches");
  QAction * importAction = new QAction("&Import...", this);
  connect(importAction, SIGNAL(triggered()), this, SLOT(importCaches()));
  cacheMenu->addAction(importAction);
}

MainWindow::~MainWindow() {
}

void MainWindow::importCaches() {
  QMessageBox(QMessageBox::Information, "Import Caches", "foo bar");
}
