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

/**
 * Set up the General page of the Preferences dialog
 */
PrefGeneralPage::PrefGeneralPage(QWidget * parent) : QWidget(parent) {

  QVBoxLayout * mainLayout = new QVBoxLayout;

  // Profile location
  QFormLayout * profileBoxLayout = new QFormLayout;
  QGroupBox * profileBox = new QGroupBox(tr("Profile location"), this);
  profileBoxLayout->setRowWrapPolicy(QFormLayout::WrapAllRows);

  QString dir = QDir::toNativeSeparators(SettingsManager::storageLocation().
    absolutePath());
  QLineEdit * dirEdit = new QLineEdit(dir, this);
  dirEdit->setReadOnly(true);
  profileBoxLayout->addRow(tr("Your profile is &stored in:"), dirEdit);

  profileBox->setLayout(profileBoxLayout);
  mainLayout->addWidget(profileBox);

  //QGroupBox * centerBox = new QGroupBox("Home coordinates", this);

  setLayout(mainLayout);
}

/**
 * Set up the Import page of the Preferences dialog
 */
PrefImportPage::PrefImportPage(QWidget * parent) :
  QWidget(parent), userNameEdit(0), passwordEdit(0) {

  QVBoxLayout * mainLayout = new QVBoxLayout;
  QFormLayout * loginBoxLayout = new QFormLayout;
  QGroupBox * loginBox = new QGroupBox(tr("Geocaching.com login"), this);

  QLabel * loginNoticeLabel = new QLabel(tr("Insert your login data for "
    "geocaching.com here before you import geocaches."), this);
  loginNoticeLabel->setWordWrap(true);
  loginBoxLayout->addRow(loginNoticeLabel);
  
  userNameEdit = new QLineEdit(this);
  userNameEdit->setText(g_settings->gcUsername());
  loginBoxLayout->addRow(tr("&User name:"), userNameEdit);

  passwordEdit = new QLineEdit(this);
  passwordEdit->setEchoMode(QLineEdit::Password);
  passwordEdit->setText(g_settings->gcPassword());
  loginBoxLayout->addRow(tr("&Password:"), passwordEdit);

  QPushButton * verifyButton = new QPushButton(tr("&Test login"), this);
  connect(verifyButton, SIGNAL(clicked()), this, SLOT(verifyLogin()));
  loginBoxLayout->addRow(verifyButton);

  QLabel * plainTextLabel = new QLabel(tr("Please note that your password is "
    "stored in plain text."), this);
  plainTextLabel->setWordWrap(true);
  loginBoxLayout->addRow(plainTextLabel);

  loginBox->setLayout(loginBoxLayout);

  mainLayout->addWidget(loginBox);

  setLayout(mainLayout);
}

/** called when the user clicks the "Test Login" button */
void PrefImportPage::verifyLogin() {
  QString userName = userNameEdit->text();
  QString password = passwordEdit->text();

  // validate user data
  try {
    GCSpider::login(userName, password);
  } catch(Failure& f) {
    QMessageBox::critical(this, "Error", tr("There was an error while trying "
      "to log in to geocaching.com. Maybe the user name or password you "
      "supplied is wrong.\n\nPlease fill in the correct values and try again."
      "\n\nThe message was: ") + f.what());
    return;
  }
  // else: login ok
  QMessageBox::information(this, "Success", tr("The login to "
    "geocaching.com was successful."));
}

PrefDialog::PrefDialog(QWidget * parent) :
  QDialog(parent), generalPage(0), importPage(0) {

#if defined(Q_OS_SYMBIAN) || defined(Q_OS_WINCE_WM)
  showMaximized();
#endif

  setWindowTitle(tr("Preferences"));

  QVBoxLayout * mainLayout = new QVBoxLayout;

  QTabWidget * tab = new QTabWidget(this);
  generalPage = new PrefGeneralPage(this);
  tab->addTab(generalPage, tr("&General"));
  importPage = new PrefImportPage(this);
  tab->addTab(importPage, tr("&Import"));
  mainLayout->addWidget(tab);

  // OK, Cancel
  QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
    | QDialogButtonBox::Cancel);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  mainLayout->addWidget(buttonBox);

  setLayout(mainLayout);
}

/** called when the user clicks the OK button */
void PrefDialog::accept() {
  // save new preferences
  QString userName = importPage->userNameEdit->text();
  QString password = importPage->passwordEdit->text();

  qDebug() << "saving new prefs: { username:" << userName << "password:" <<
    "(omitted)" << "}";
  g_settings->setGcUsername(userName);
  g_settings->setGcPassword(password);
  QDialog::accept();
}
