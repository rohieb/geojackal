#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "OsmSlippyMap.h"
#include <QObject>
#include <QtGui>

using namespace geojackal;

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow();
  virtual ~MainWindow();

protected slots:
  void importCaches();

private:
  OsmSlippyMap * pmap;
};

#endif // MAINWINDOW_H
