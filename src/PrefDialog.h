/**
 * @file PrefDialog.h
 * @date 13 Jul 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 *
 * Copyright (C) 2010 Roland Hieber
 * 
 * This program is free software; you can redistribute it and/or modify it 
 * under the terms of the GNU General Public License, version 3, as published 
 * by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT 
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or 
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for 
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with 
 * this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PREFDIALOG_H_
#define PREFDIALOG_H_

#include <QtGui>

namespace geojackal {

class PrefGeneralPage : public QWidget {
  Q_OBJECT
public:
  PrefGeneralPage(QWidget * parent);
  virtual ~PrefGeneralPage() {}

protected slots:
  void openProfileDir();
};

class PrefImportPage : public QWidget {
  Q_OBJECT
  friend class PrefDialog;
public:
  PrefImportPage(QWidget * parent);
  virtual ~PrefImportPage() {}

public slots:
  void verifyLogin();

private:
  QLineEdit * userNameEdit;
  QLineEdit * passwordEdit;
};

class PrefDialog : public QDialog {
  Q_OBJECT
public:
  PrefDialog(QWidget * parent = 0);
  virtual ~PrefDialog() {}

public slots:
  void accept();

private:
  PrefGeneralPage * generalPage;
  PrefImportPage * importPage;
};

}

#endif /* PREFDIALOG_H_ */
