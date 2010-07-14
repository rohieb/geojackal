/**
 * @file GCSpiderDialog.h
 * @date 13 Jul 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#ifndef GCSPIDERDIALOG_H_
#define GCSPIDERDIALOG_H_

#include "Coordinate.h"
#include <QtGui>

namespace geojackal {

/**
 * Dialog to allow the user input of cache spider data, like maximum distance
 * from center point
 */
class GCSpiderDialog : public QDialog {
  Q_OBJECT
public:
  GCSpiderDialog(QWidget * parent = 0);
  virtual ~GCSpiderDialog();

  float maxDist() const {
    return maxDist_;
  }
  const Angle& lat() const {
    return lat_;
  }
  const Angle& lon() const {
    return lon_;
  }

public slots:
  void accept();

private:
  QLineEdit * maxDistEdit;
  QLineEdit * latEdit;
  QLineEdit * lonEdit;
  float maxDist_;
  Angle lat_;
  Angle lon_;
};

}

#endif /* GCSPIDERDIALOG_H_ */
