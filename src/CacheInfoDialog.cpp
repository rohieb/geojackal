/**
 * @file CacheInfoDialog.cpp
 * @date Jul 14, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#include "CacheInfoDialog.h"
#include "OsmSlippyMap.h" // for cacheIcon()

namespace geojackal {

/**
 * Conversion of cache size from enum to string, like "micro".
 * @param size Cache size
 * @return Lowercase textual representation of the cache size, or empty string
 */
QString sizeToText(CacheSize size) {
  switch(size) {
    case SIZE_NANO:
      return "nano";
    case SIZE_MICRO:
      return "micro";
    case SIZE_SMALL:
      return "small";
    case SIZE_REGULAR:
      return "regular";
    case SIZE_LARGE:
      return "large";
    case SIZE_OTHER:
      return "other";
    case SIZE_NONE:
      return "none";
    default:
      return "";
  }
}

/** Info tab page */
InfoTab::InfoTab(Cache * cache) : QWidget(0) {
  QVBoxLayout * l = new QVBoxLayout(this);
  l->addWidget(new QLabel("Waypoint:\t" + cache->waypoint));
  l->addWidget(new QLabel(QString("Owner:\t%1").arg(cache->owner)));
  l->addWidget(new QLabel(QString("Hidden:\t%1").arg(cache->placed->toString(
    Qt::SystemLocaleLongDate))));
  l->addWidget(new QLabel(QString("Size:\t%1").arg(sizeToText(cache->size))));
  l->addWidget(new QLabel(QString("Difficulty:\t%1").arg(qreal(
    cache->difficulty) / 2.0, 0, 'f', 1)));
  l->addWidget(new QLabel(QString("Terrain:\t%2").arg(qreal(cache->terrain)
    / 2.0, 0, 'f', 1)));
}
InfoTab::~InfoTab() {
}

/** Geocache information dialog */
CacheInfoDialog::CacheInfoDialog(Cache * cache, QWidget * parent) :
  QDialog(parent), cache_(cache) {

  setWindowTitle(cache->name + " (" + cache->waypoint + ")");

  // Grid layout as main layout
  QGridLayout * mainLayout = new QGridLayout(this);

  // cache name
  mainLayout->addWidget(new QLabel("<big><b>" + cache->name + "</b></big>"), 0,
    0, Qt::AlignLeft);

  // cache icon
  QLabel * pic = new QLabel;
  pic->setPixmap(cacheIcon(cache));
  mainLayout->addWidget(pic, 0, 1, Qt::AlignRight);

  // tab pages
  // setup description browser
  QTextBrowser * descBrowser = new QTextBrowser();
  descBrowser->setHtml(cache->desc->desc);
  descBrowser->setOpenExternalLinks(true);

  // prepare tab widgets
  QTabWidget * tab = new QTabWidget;
  tab->addTab(new InfoTab(cache), "General");
  tab->addTab(descBrowser, "Description");

  mainLayout->addWidget(tab, 1, 0, 1, 2);

  // button box
  QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
  connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
  mainLayout->addWidget(buttonBox, 2, 0, 1, 2, Qt::AlignRight);

  setLayout(mainLayout);
}

CacheInfoDialog::~CacheInfoDialog() {
}

}
