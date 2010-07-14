/**
 * @file GCSpiderDialog.cpp
 * @date 13 Jul 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#include "GCSpiderDialog.h"
#include "GCSpider.h"

using namespace geojackal;

GCSpiderDialog::GCSpiderDialog(QWidget * parent) :
  QDialog(parent) {

  setWindowTitle("Import from geocaching.com");

  QSettings settings;
  QVBoxLayout * mainLayout = new QVBoxLayout(this);

  QGroupBox * loginBox = new QGroupBox("Geocaching.com login", this);
  QGridLayout * loginLayout = new QGridLayout(this);
  loginLayout->addWidget(new QLabel("Insert your login data for geocaching.com"
    "\nhere before you import caches."), 0, 0, 1, 2, Qt::AlignLeft);
  loginLayout->addWidget(new QLabel("User name:"), 1, 0, Qt::AlignLeft);
  loginLayout->addWidget(new QLabel("Password:"), 2, 0, Qt::AlignLeft);
  userNameEdit = new QLineEdit;
  loginLayout->addWidget(userNameEdit, 1, 1);
  userNameEdit->setText(settings.value("gc/username").toString());
  passwordEdit = new QLineEdit;
  passwordEdit->setEchoMode(QLineEdit::Password);
  passwordEdit->setText(settings.value("gc/password").toString());
  loginLayout->addWidget(passwordEdit, 2, 1);
  loginBox->setLayout(loginLayout);
  loginLayout->addWidget(new QLabel("Please note that your password is stored "
    "in plain text."), 3, 0, 1, 2, Qt::AlignLeft);
  mainLayout->addWidget(loginBox);

  //QGroupBox * centerBox = new QGroupBox("Home coordinates", this);

  QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok
    | QDialogButtonBox::Cancel);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  loginLayout->addWidget(buttonBox, 4, 0, 1, 2, Qt::AlignRight);

  setLayout(mainLayout);
}

GCSpiderDialog::~GCSpiderDialog() {
}

void GCSpiderDialog::accept() {
  // save new preferences
  QSettings settings;
  QString userName = userNameEdit->text();
  QString password = passwordEdit->text();

  // validate user data
  try {
  GCSpider spider(userName, password);
  } catch(Failure& f) {
    QMessageBox msg(QMessageBox::Critical, "Error", "There was an error while "
      "trying to log in to geocaching.com. Maybe your supplied user name or "
      "password is wrong.\n\nPlease fill in the correct values and try again.",
      QMessageBox::Ok, this);
    msg.exec();
    return;
  }

  // everything ok
  qDebug() << "saving new prefs: { username:" << userName << "password:" <<
    "(omitted)" << "}";
  settings.setValue("gc/username", userName);
  settings.setValue("gc/password", password);
  QDialog::accept();
}
