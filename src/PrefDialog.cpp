/**
 * @file PrefDialog.cpp
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

#include "PrefDialog.h"
#include "GCSpider.h"

using namespace geojackal;

PrefDialog::PrefDialog(QWidget * parent) :
  QDialog(parent) {

#if defined(Q_OS_SYMBIAN) || defined(Q_OS_WINCE_WM)
  showMaximized();
#endif

  setWindowTitle(tr("Preferences"));

  QVBoxLayout * mainLayout = new QVBoxLayout;

  // Profile location
  QGroupBox * profileBox = new QGroupBox(tr("Profile location"));
  QFormLayout * profileBoxLayout = new QFormLayout;
  profileBoxLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);

  QString dir = QDir::toNativeSeparators(SettingsManager::storageLocation().
    absolutePath());
  QLineEdit * dirEdit = new QLineEdit(dir);
  dirEdit->setReadOnly(true);
  profileBoxLayout->addRow(tr("Your &profile is stored in:"), dirEdit);

  profileBox->setLayout(profileBoxLayout);
  mainLayout->addWidget(profileBox);

  // GC.com login data
  QGroupBox * loginBox = new QGroupBox(tr("Geocaching.com login"));
  QFormLayout * loginBoxLayout = new QFormLayout;

  QLabel * loginNoticeLabel = new QLabel(tr("Insert your login data for "
    "geocaching.com here before you import geocaches."));
  loginNoticeLabel->setWordWrap(true);
  loginBoxLayout->addRow(loginNoticeLabel);
  
  userNameEdit = new QLineEdit;
  userNameEdit->setText(g_settings->gcUsername());
  loginBoxLayout->addRow(tr("&User name:"), userNameEdit);

  passwordEdit = new QLineEdit;
  passwordEdit->setEchoMode(QLineEdit::Password);
  passwordEdit->setText(g_settings->gcPassword());
  loginBoxLayout->addRow(tr("&Password:"), passwordEdit);

  QLabel * plainTextLabel = new QLabel(tr("Please note that your password is "
    "stored in plain text."));
  plainTextLabel->setWordWrap(true);
  loginBoxLayout->addRow(plainTextLabel);

  loginBox->setLayout(loginBoxLayout);
  mainLayout->addWidget(loginBox);

  //QGroupBox * centerBox = new QGroupBox("Home coordinates", this);

  // OK, Cancel
  QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
    | QDialogButtonBox::Cancel);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  mainLayout->addWidget(buttonBox);

  setLayout(mainLayout);
}

PrefDialog::~PrefDialog() {
}

void PrefDialog::accept() {
  // save new preferences
  QString userName = userNameEdit->text();
  QString password = passwordEdit->text();

  // validate user data
  try {
    GCSpider::login(userName, password);
  } catch(Failure& f) {
    QMessageBox msg(QMessageBox::Critical, "Error", QString(tr("There was an "
      "error while trying to log in to geocaching.com. Maybe your supplied "
      "user name or password is wrong.\n\nPlease fill in the correct values "
      "and try again.\n\nThe error was: ")) + f.what(), QMessageBox::Ok, this);
    msg.exec();
    return;
  }

  // everything ok
  qDebug() << "saving new prefs: { username:" << userName << "password:" <<
    "(omitted)" << "}";
  g_settings->setGcUsername(userName);
  g_settings->setGcPassword(password);
  QDialog::accept();
}
