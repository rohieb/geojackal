/**
 * @file GCSpiderDialog.h
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

#ifndef GCSPIDERDIALOG_H_
#define GCSPIDERDIALOG_H_

#include "logic/Coordinate.h"
#include <QtGui>

namespace geojackal {

/**
 * Dialog to allow the user input of geocache spider data, like maximum distance
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
