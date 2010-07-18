/**
 * @file GCSpiderDialog.cpp
 * @date 13 Jul 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#include "GCSpiderDialog.h"

using namespace geojackal;

GCSpiderDialog::GCSpiderDialog(QWidget * parent) :
  QDialog(parent) {

  setWindowTitle("Import from geocaching.com");

  bool ok;
  QSettings settings;
  maxDist_ = settings.value("gc/maxdist", 1.0).toDouble(&ok);
  lat_ = settings.value("gc/centerLat", 52.273).toDouble(&ok);
  lon_ = settings.value("gc/centerLon", 10.52524).toDouble(&ok);

  QGridLayout * mainLayout = new QGridLayout(this);
  mainLayout->addWidget(new QLabel("Longitude:", this), 0, 0);
  lonEdit = new QLineEdit(QString::number(lon_, 'f', 6), this);
  mainLayout->addWidget(lonEdit, 0, 1);
  mainLayout->addWidget(new QLabel("Latitude:", this), 1, 0);
  latEdit = new QLineEdit(QString::number(lat_, 'f', 6), this);
  mainLayout->addWidget(latEdit, 1, 1);
  mainLayout->addWidget(new QLabel("Max. distance (km):", this), 2, 0);
  maxDistEdit = new QLineEdit(QString::number(maxDist_, 'f', 2), this);
  mainLayout->addWidget(maxDistEdit, 2, 1);

  QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
    | QDialogButtonBox::Cancel);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  mainLayout->addWidget(buttonBox, 3, 0, 1, 2, Qt::AlignRight);

  setLayout(mainLayout);
}

GCSpiderDialog::~GCSpiderDialog() {
}

void GCSpiderDialog::accept() {
  // save new preferences
  QSettings settings;

  // validate user data
  bool ok;
  maxDist_ = maxDistEdit->text().toFloat(&ok);
  if(!ok || maxDist_ < 0) {
    QMessageBox::critical(this, "Error", "Max. distance must be a positive "
      "number.");
    return;
  }
  lon_ = lonEdit->text().toDouble(&ok);
  if(!ok || lon_ < -90 || lon_ > 90) {
    QMessageBox::critical(this, "Error", "Longitude must be between -90 and "
      "90.");
    return;
  }
  lat_ = latEdit->text().toDouble(&ok);
  if(!ok || lat_ < -90 || lat_ > 90) {
    QMessageBox::critical(this, "Error", "Latitude must be between -180 and "
      "180.");
    return;
  }

  // everything ok
  qDebug() << "saving new prefs: { maxdist:" << maxDist_ << "centerLon:" << lon_
    << "centerLat" << lat_ << "}";
  settings.setValue("gc/maxdist", maxDist_);
  settings.setValue("gc/centerLon", static_cast<double>(lon_));
  settings.setValue("gc/centerLat", static_cast<double>(lat_));
  QDialog::accept();
}
