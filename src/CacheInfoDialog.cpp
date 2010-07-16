/**
 * @file CacheInfoDialog.cpp
 * @date Jul 14, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#include "CacheInfoDialog.h"

namespace geojackal {

CacheInfoDialog::CacheInfoDialog(Cache * cache, QWidget * parent) :
  QDialog(parent), cache_(cache) {

  setWindowTitle("Cache Information");
  QVBoxLayout * mainLayout = new QVBoxLayout(this);
  QString nameText = QString("<big><b>") + QString::fromUtf8(cache->name.
    toStdString().c_str()) + "</b></big>";
  nameLabel = new QLabel(nameText);
  mainLayout->addWidget(nameLabel);
  descEdit = new QTextEdit(QString::fromUtf8(cache->desc->desc.toStdString().
    c_str()));
  descEdit->setReadOnly(true);
  mainLayout->addWidget(descEdit);

  QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
  mainLayout->addWidget(buttonBox, 0, Qt::AlignRight);

  setLayout(mainLayout);
}

CacheInfoDialog::~CacheInfoDialog() {
}

}
