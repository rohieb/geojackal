#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "OsmSlippyMap.h"
#include <QObject>
#include <QtGui>

using namespace geojackal;

class MainWindow : public QWidget {
  Q_OBJECT
public:
  MainWindow();
  virtual ~MainWindow();

public slots:
  void plusClicked(bool);
  void minusClicked(bool);

private:
  OsmSlippyMap * pmap;
  uchar zoomLevel_;
};

#endif // MAINWINDOW_H
