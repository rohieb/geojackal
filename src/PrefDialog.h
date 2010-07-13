/**
 * @file PrefDialog.h
 * @date 13 Jul 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#ifndef PREFDIALOG_H_
#define PREFDIALOG_H_

#include <QtGui>

namespace geojackal {

class PrefDialog : public QDialog {
  Q_OBJECT
public:
  PrefDialog(QWidget * parent = 0);
  virtual ~PrefDialog();

public slots:
  void accept();

private:
  QLineEdit * userNameEdit;
  QLineEdit * passwordEdit;

};

}

#endif /* PREFDIALOG_H_ */
