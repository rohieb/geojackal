#include "MainWindow.h"

MainWindow::MainWindow() :
  QWidget(0), pmap(0), zoomLevel_(16) {
  QHBoxLayout * hLayout = new QHBoxLayout();
  QPushButton * plus = new QPushButton("+");
  connect(plus, SIGNAL(clicked(bool)), this, SLOT(plusClicked(bool)));
  QPushButton * minus = new QPushButton("-");
  connect(minus, SIGNAL(clicked(bool)), this, SLOT(minusClicked(bool)));
  hLayout->addWidget(minus);
  hLayout->addWidget(plus);

  QVBoxLayout * vLayout = new QVBoxLayout(this);

  // IZ Coordinates
  pmap = new OsmSlippyMap(
    Coordinate(Angle(52, 16, 22.79), Angle(10, 31, 30.87)), 16);
  pmap->setZoom(zoomLevel_);
  vLayout->addWidget(pmap);
  vLayout->addLayout(hLayout);

  setLayout(vLayout);
  pmap->setFocus();
}

MainWindow::~MainWindow() {
}

void MainWindow::minusClicked(bool) {
  if(zoomLevel_ > 0) {
    zoomLevel_--;
    pmap->setZoom(zoomLevel_);
  }
}

void MainWindow::plusClicked(bool) {
  if(zoomLevel_ < 18) {
    zoomLevel_++;
    pmap->setZoom(zoomLevel_);
  }
}
