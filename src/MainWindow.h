#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "OsmSlippyMap.h"
#include "CacheModel.h"
#include <QObject>
#include <QtGui>

namespace geojackal {

class MainWindow : public QMainWindow {
  Q_OBJECT
public:
  MainWindow();
  virtual ~MainWindow();

protected slots:
  void showPrefDialog();
  void importCaches();

private:
  OsmSlippyMap * pmap;
  CacheModel * pModel;
};

}
#endif // MAINWINDOW_H
