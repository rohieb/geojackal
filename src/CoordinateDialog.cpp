/**
 * @file CoordinateDialog.cpp
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

#include "CoordinateDialog.h"

using namespace geojackal;

CoordinateButton::CoordinateButton(const Coordinate& coord, QWidget * parent) :
  QPushButton(parent), coord_(coord) {
  setText(coord.format(Coordinate::FormatDegMin));
  connect(this, SIGNAL(clicked()), SLOT(showCoordinateDialog()));
}

void CoordinateButton::showCoordinateDialog() {
  CoordinateDialog dlg(coordinate(), this);
  if(dlg.exec() == QDialog::Accepted) {
    setCoordinate(dlg.coordinate());
  }
}

/**
 * @todo Implement coordinate selection by map
 * @todo Implement UTM input format
 */

CoordinateDialog::CoordinateDialog(const Coordinate& c, QWidget * parent) :
  QDialog(parent), coord_(c), radioGroup_(0),
  degCombo_(0), degMinCombo_(0), degMinSecCombo_(0),
  latPrefixCombo_(0), lonPrefixCombo_(0),
  latDegEdit_(0), latDegMinEdit_(0), latDegMinSecEdit_(0),
  lonDegEdit_(0), lonDegMinEdit_(0), lonDegMinSecEdit_(0),
  doubleValidator_(0), threeDigitIntValidator_(0), twoDigitIntValidator_(0) {

  // validators for edit controls
  doubleValidator_ = new QDoubleValidator(0, 360, 10, this);
  threeDigitIntValidator_ = new QIntValidator(0, 360, this);
  twoDigitIntValidator_ = new QIntValidator(0, 60, this);

  // layout etc.
  QGridLayout * layout = new QGridLayout(this);

  // Radio buttons for input format selection
  QHBoxLayout * radioBox = new QHBoxLayout(this);
  radioGroup_ = new QButtonGroup(this);

  degCombo_ = new QRadioButton("&d.d\xb0", this);
  radioBox->addWidget(degCombo_);
  radioGroup_->addButton(degCombo_);

  degMinCombo_ = new QRadioButton("d\xb0&m.m'", this);
  radioBox->addWidget(degMinCombo_);
  radioGroup_->addButton(degMinCombo_);

  degMinSecCombo_ = new QRadioButton("d\xb0m'&s.s\"", this);
  radioBox->addWidget(degMinSecCombo_);
  radioGroup_->addButton(degMinSecCombo_);

  layout->addLayout(radioBox, 0, 0, 1, 3, Qt::AlignCenter);
  connect(radioGroup_, SIGNAL(buttonClicked(QAbstractButton *)), this,
    SLOT(chooseRadio(QAbstractButton *)));

  // input boxes
  QGridLayout * inputBoxLayout = new QGridLayout(this);

  latPrefixCombo_ = new QComboBox(this);
  latPrefixCombo_->insertItem(0, tr("N", "abbreviation for north"));
  latPrefixCombo_->insertItem(1, tr("S", "abbreviation for south"));
  inputBoxLayout->addWidget(latPrefixCombo_, 0, 0);

  latDegEdit_ = new QLineEdit(this);
  latDegEdit_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  inputBoxLayout->addWidget(latDegEdit_, 0, 1);
  latDegMinEdit_ = new QLineEdit(this);
  latDegMinEdit_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  inputBoxLayout->addWidget(latDegMinEdit_, 0, 2);
  latDegMinSecEdit_ = new QLineEdit(this);
  latDegMinSecEdit_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  inputBoxLayout->addWidget(latDegMinSecEdit_, 0, 3);

  lonPrefixCombo_ = new QComboBox(this);
  lonPrefixCombo_->insertItem(0, tr("E", "abbreviation for east"));
  lonPrefixCombo_->insertItem(1, tr("W", "abbreviation for west"));
  inputBoxLayout->addWidget(lonPrefixCombo_, 1, 0);

  lonDegEdit_ = new QLineEdit(this);
  lonDegEdit_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  inputBoxLayout->addWidget(lonDegEdit_, 1, 1);
  lonDegMinEdit_ = new QLineEdit(this);
  lonDegMinEdit_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  inputBoxLayout->addWidget(lonDegMinEdit_, 1, 2);
  lonDegMinSecEdit_ = new QLineEdit(this);
  lonDegMinSecEdit_->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Fixed);
  inputBoxLayout->addWidget(lonDegMinSecEdit_, 1, 3);

  layout->addLayout(inputBoxLayout, 1, 0, 1, 3);

  // Button box
  QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
    | QDialogButtonBox::Cancel);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  layout->addWidget(buttonBox, 2, 0, 1, 2, Qt::AlignRight);

  setLayout(layout);

  // install event filter to get focusOut events
  lonDegEdit_->installEventFilter(this);
  lonDegMinEdit_->installEventFilter(this);
  lonDegMinSecEdit_->installEventFilter(this);
  lonPrefixCombo_->installEventFilter(this);
  latDegEdit_->installEventFilter(this);
  latDegMinEdit_->installEventFilter(this);
  latDegMinSecEdit_->installEventFilter(this);
  latPrefixCombo_->installEventFilter(this);

  // fill edit controls and pre-select degree-minute format
  degMinCombo_->setChecked(true);
  latDegEdit_->setFocus();
  chooseRadio(degMinCombo_);
  setCoordinate(c);
}

CoordinateDialog::~CoordinateDialog() {
}

/**
 * Set the coordinate shown in the widget and update the text in the edit
 * controls
 */
void CoordinateDialog::setCoordinate(const Coordinate& c) {
  coord_ = c;

  // north/south and east/west combo boxes
  latPrefixCombo_->setCurrentIndex(c.lat <= 0);
  lonPrefixCombo_->setCurrentIndex(c.lon <= 0);

  // edit controls
  QAbstractButton * button = radioGroup_->checkedButton();
  if(button == degCombo_) {
    latDegEdit_->setText(QString::number(fabs(c.lat.deg()), 'f', 6));
    latDegMinEdit_->setText("");
    latDegMinSecEdit_->setText("");
    lonDegEdit_->setText(QString::number(fabs(c.lon.deg()), 'f', 6));
    lonDegMinEdit_->setText("");
    lonDegMinSecEdit_->setText("");
  } else if(button == degMinCombo_) {
    latDegEdit_->setText(QString::number((int)abs(c.lat.deg())));
    latDegMinEdit_->setText(QString::number(c.lat.min(), 'f', 3));
    latDegMinSecEdit_->setText("");
    lonDegEdit_->setText(QString::number((int)abs(c.lon.deg())));
    lonDegMinEdit_->setText(QString::number(c.lon.min(), 'f', 3));
    lonDegMinSecEdit_->setText("");
  } else if(button == degMinSecCombo_) {
    latDegEdit_->setText(QString::number((int)abs(c.lat.deg())));
    latDegMinEdit_->setText(QString::number((int)c.lat.min()));
    latDegMinSecEdit_->setText(QString::number(c.lat.sec(), 'f', 2));
    lonDegEdit_->setText(QString::number((int)abs(c.lon.deg())));
    lonDegMinEdit_->setText(QString::number((int)c.lon.min()));
    lonDegMinSecEdit_->setText(QString::number(c.lon.sec(), 'f', 2));
  }
}

/**
 * Get the coordinate shown in the widget, or @c COORD_INVALID if the user did
 * not enter a valid coordinate
 */
Coordinate CoordinateDialog::coordinate() {
  Coordinate newCoord;
  bool ok; // we don't need this, the validators check for us
  newCoord.lat = Angle(latDegEdit_->text().toDouble(&ok),
    latDegMinEdit_->text().toDouble(&ok),
    latDegMinSecEdit_->text().toDouble(&ok));
  newCoord.lat *= (latPrefixCombo_->currentIndex() == 0) ? 1 : -1;

  newCoord.lon = Angle(lonDegEdit_->text().toDouble(&ok),
    lonDegMinEdit_->text().toDouble(&ok),
    lonDegMinSecEdit_->text().toDouble(&ok));
  newCoord.lon *= (lonPrefixCombo_->currentIndex() == 0) ? 1 : -1;

  if(!newCoord.isValid()) {
    QMessageBox::critical(this, tr("Error"), tr("This is not a valid "
      "geographical coordinate!"), QMessageBox::Ok);
    return COORD_INVALID;
  }
  return (coord_ = newCoord);
}

/** called when the user selects a radio button */
void CoordinateDialog::chooseRadio(QAbstractButton * button) {
  // enable or disable the respective edit fields and set validators
  if(button == degCombo_) {
    // show only degree
    latDegMinEdit_->setEnabled(false);
    latDegMinSecEdit_->setEnabled(false);
    lonDegMinEdit_->setEnabled(false);
    lonDegMinSecEdit_->setEnabled(false);

    doubleValidator_->setDecimals(10);
    latDegEdit_->setValidator(doubleValidator_);
    lonDegEdit_->setValidator(doubleValidator_);

  } else if(button == degMinCombo_) {
    // show degree and minute
    latDegMinEdit_->setEnabled(true);
    latDegMinSecEdit_->setEnabled(false);
    lonDegMinEdit_->setEnabled(true);
    lonDegMinSecEdit_->setEnabled(false);

    doubleValidator_->setDecimals(3);
    latDegEdit_->setValidator(threeDigitIntValidator_);
    latDegMinEdit_->setValidator(doubleValidator_);
    lonDegEdit_->setValidator(threeDigitIntValidator_);
    lonDegMinEdit_->setValidator(doubleValidator_);

} else if(button == degMinSecCombo_) {
    // show degree, minute and second
    latDegMinEdit_->setEnabled(true);
    latDegMinSecEdit_->setEnabled(true);
    lonDegMinEdit_->setEnabled(true);
    lonDegMinSecEdit_->setEnabled(true);

    doubleValidator_->setDecimals(2);
    latDegEdit_->setValidator(threeDigitIntValidator_);
    latDegMinEdit_->setValidator(twoDigitIntValidator_);
    latDegMinSecEdit_->setValidator(doubleValidator_);
    lonDegEdit_->setValidator(threeDigitIntValidator_);
    lonDegMinEdit_->setValidator(twoDigitIntValidator_);
    lonDegMinSecEdit_->setValidator(doubleValidator_);
  }
  // fill the line edits
  setCoordinate(coord_);
}

void CoordinateDialog::accept() {
  // validate before acception, or do nothing at all
  if(coordinate().isValid()) {
    QDialog::accept();
  }
}

/** update @a coord_ when a edit widget loses focus */
bool CoordinateDialog::eventFilter(QObject * watched, QEvent * event) {
  if(event->type() == QEvent::FocusOut) {
    if(watched == lonDegEdit_ || watched == lonDegMinEdit_ ||
      watched == lonDegMinSecEdit_ || watched == lonPrefixCombo_ ||
      watched == latDegEdit_ || watched == latDegMinEdit_ ||
      watched == latDegMinSecEdit_ || watched == latPrefixCombo_) {
      coordinate(); // as a side-effect, update coord_ from edit controls
    }
  }
  return QObject::eventFilter(watched, event);
}
