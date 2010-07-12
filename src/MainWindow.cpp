#include "MainWindow.h"

MainWindow::MainWindow() :
  QMainWindow(0), pmap(0) {
  // start at IZ Coordinates
  pmap = new OsmSlippyMap(
    Coordinate(Angle(52, 16, 22.79), Angle(10, 31, 30.87)), 16);
  pmap->setZoom(16);
  pmap->setFocus();
  setCentralWidget(pmap);
}

MainWindow::~MainWindow() {
}
