/**
 * @file PrefDialog.cpp
 * @date 13 Jul 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#include "PrefDialog.h"
#include "GCSpider.h"

using namespace geojackal;

PrefDialog::PrefDialog(QWidget * parent) :
  QDialog(parent) {

  setWindowTitle("Preferences");

  QVBoxLayout * mainLayout = new QVBoxLayout(this);

  // Profile location
  QString dir = QDir::toNativeSeparators(SettingsManager::storageLocation().
    absolutePath());
  qDebug() << "Profile dir is" << dir;
  mainLayout->addWidget(new QLabel(tr("Your profile is stored in:")));
  QLineEdit * dirEdit = new QLineEdit(dir);
  dirEdit->setReadOnly(true);
  mainLayout->addWidget(dirEdit);

  // GC.com login data
  QGroupBox * loginBox = new QGroupBox("Geocaching.com login", this);
  QGridLayout * loginLayout = new QGridLayout(this);
  loginLayout->addWidget(new QLabel("Insert your login data for geocaching.com"
    "\nhere before you import caches."), 0, 0, 1, 2, Qt::AlignLeft);
  loginLayout->addWidget(new QLabel("User name:"), 1, 0, Qt::AlignLeft);
  loginLayout->addWidget(new QLabel("Password:"), 2, 0, Qt::AlignLeft);
  userNameEdit = new QLineEdit;
  loginLayout->addWidget(userNameEdit, 1, 1);
  userNameEdit->setText(g_settings->gcUsername());
  passwordEdit = new QLineEdit;
  passwordEdit->setEchoMode(QLineEdit::Password);
  passwordEdit->setText(g_settings->gcPassword());
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
    QMessageBox msg(QMessageBox::Critical, "Error", QString("There was an "
      "error while trying to log in to geocaching.com. Maybe your supplied "
      "user name or password is wrong.\n\nPlease fill in the correct values "
      "and try again.\n\nThe error was: ") + f.what(), QMessageBox::Ok, this);
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
