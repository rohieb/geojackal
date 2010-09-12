/**
 * @file CoordinateDialog.h
 * @date 2 Sep 2010
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

#ifndef COORDINATEWIDGET_H_
#define COORDINATEWIDGET_H_

#include "Coordinate.h"
#include <QtGui>

namespace geojackal {

/**
 * Push button that displays a geological coordinate and lets the user choose a
 * different one if he clicks on it
 */
class CoordinateButton : public QPushButton {
  Q_OBJECT
public:
  CoordinateButton(const Coordinate& coord, QWidget * parent = 0);
  void setCoordinate(const Coordinate& c) {
    coord_ = c;
    setText(coord_.format(Coordinate::FormatDegMin));
  }
  Coordinate coordinate() const {
    return coord_;
  }

protected slots:
  void showCoordinateDialog();

private:
  /** The coordinate shown in the widget */
  Coordinate coord_;
};

/**
 * Dialog to allow the user a selection of a geological coordinate
 */
class CoordinateDialog : public QDialog {
  Q_OBJECT
public:
  CoordinateDialog(const Coordinate& c = COORD_INVALID, QWidget * parent = 0);
  virtual ~CoordinateDialog();

  void setCoordinate(const Coordinate& c);
  Coordinate coordinate();

protected:
  bool eventFilter(QObject * watched, QEvent * event);

protected slots:
  void chooseRadio(QAbstractButton * button);
  virtual void accept();

private:
  /** The coordinate shown in the widget */
  Coordinate coord_;

  QButtonGroup * radioGroup_;
  QRadioButton * degCombo_;
  QRadioButton * degMinCombo_;
  QRadioButton * degMinSecCombo_;

  QComboBox * latPrefixCombo_;
  QComboBox * lonPrefixCombo_;

  QLineEdit * latDegEdit_;
  QLineEdit * latDegMinEdit_;
  QLineEdit * latDegMinSecEdit_;
  QLineEdit * lonDegEdit_;
  QLineEdit * lonDegMinEdit_;
  QLineEdit * lonDegMinSecEdit_;

  QDoubleValidator * doubleValidator_;
  QIntValidator * threeDigitIntValidator_;
  QIntValidator * twoDigitIntValidator_;
};

}

#endif /* COORDINATEWIDGET_H_ */
