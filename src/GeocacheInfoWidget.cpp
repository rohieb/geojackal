/**
 * @file GeocacheInfoWidget.cpp
 * @date Jul 14, 2010
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

#include "GeocacheInfoWidget.h"
#include "OsmSlippyMap.h" // for geocacheIcon()

using namespace geojackal;

/**
 * Conversion of geocache container size from enum to string, like "micro".
 * @param size Geocache size
 * @return Lowercase textual representation of the geocache container size, or
 * empty string if invalid size or @c SIZE_UNKNOWN was given
 */
QString geojackal::sizeToText(GeocacheSize size) {
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
InfoTab::InfoTab(QWidget * parent, Geocache * geocache) : QWidget(parent) {
  waypoint_ = new QLabel(this);
  owner_ = new QLabel(this);
  placed_ = new QLabel(this);
  size_ = new QLabel(this);
  diff_ = new QLabel(this);
  terrain_ = new QLabel(this);

  QFormLayout * l = new QFormLayout(this);
  l->addRow(tr("Waypoint:\t"), waypoint_);
  l->addRow(tr("Owner:"), owner_);
  l->addRow(tr("Hidden:"), placed_);
  l->addRow(tr("Size:"), size_);
  l->addRow(tr("Difficulty:"), diff_);
  l->addRow(tr("Terrain:"), terrain_);

  setLayout(l);

  setGeocache(geocache);
}

InfoTab::~InfoTab() {
}

/**
 * Set the geocache whose data is to be displayed.
 * @param geocache the new geocache. If this parameter is @c 0, no data is
 * shown.
 */
void InfoTab::setGeocache(Geocache * geocache) {
  if(geocache) {
    waypoint_->setText(geocache->waypoint);
    owner_->setText(geocache->owner);
    placed_->setText(geocache->placed->toString(Qt::SystemLocaleShortDate));
    size_->setText(sizeToText(geocache->size));
    diff_->setText(QString("%1").arg(qreal(geocache->difficulty) / 2.0));
    terrain_->setText(QString("%1").arg(qreal(geocache->terrain) / 2.0));
  } else {
    waypoint_->setText("");
    owner_->setText("");
    placed_->setText("");
    size_->setText("");
    diff_->setText("");
    terrain_->setText("");
  }
}

/** Geocache information dialog */
GeocacheInfoWidget::GeocacheInfoWidget(Geocache * geocache, QWidget * parent) :
  QWidget(parent), geocache_(geocache) {

  mainLayout_ = new QGridLayout(this);

  // init fields, they are filled later
  // name
  geocacheName_ = new QLabel;
  mainLayout_->addWidget(geocacheName_, 0, 0, Qt::AlignLeft | Qt::AlignTop);

  // icon
  geocacheIcon_ = new QLabel;
  mainLayout_->addWidget(geocacheIcon_, 0, 1, Qt::AlignRight | Qt::AlignTop);

  // tab pages
  // geocache details
  geocacheInfoTab_ = new InfoTab;

  // description browser
  geocacheDescBrowser_ = new QTextBrowser;
  geocacheDescBrowser_->setOpenExternalLinks(true);

  tab_ = new QTabWidget;
  tab_->addTab(geocacheInfoTab_, tr("General"));
  tab_->addTab(geocacheDescBrowser_, tr("Description"));
  mainLayout_->addWidget(tab_, 1, 0, 1, 2);

  setLayout(mainLayout_);

  // fill in the values
  setGeocache(geocache);
}

GeocacheInfoWidget::~GeocacheInfoWidget() {
}

/**
 * Set the geocache whose data is to be displayed.
 * @param geocache the new geocache. If this parameter is @c 0, the controls
 * holding the geocache information are cleared.
 */
void GeocacheInfoWidget::setGeocache(Geocache * geocache) {
  if(!geocache) {
    // clear values
    geocacheName_->setText("<big><b>" + tr("No geocache selected") +
      "</b></big>");
    geocacheIcon_->setText("");
    geocacheInfoTab_->setGeocache(0);
    geocacheDescBrowser_->setText("");

  } else {
    geocacheName_->setText("<big><b>" + geocache->name + "</b></big>");
    geocacheIcon_->setPixmap(geocacheIcon(geocache));
    geocacheInfoTab_->setGeocache(geocache);
    geocacheDescBrowser_->setHtml(geocache->desc);
  }
}
