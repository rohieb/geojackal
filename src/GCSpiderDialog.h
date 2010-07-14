/**
 * @file GCSpiderDialog.h
 * @date 13 Jul 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#ifndef GCSPIDERDIALOG_H_
#define GCSPIDERDIALOG_H_

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

public slots:
  void accept();

private:
  QLineEdit * maxDist;

};

}

#endif /* GCSPIDERDIALOG_H_ */
