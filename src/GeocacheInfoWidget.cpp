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
InfoTab::InfoTab(Geocache * geocache) : QWidget(0) {
  if(geocache) {
    QVBoxLayout * l = new QVBoxLayout(this);
    l->addWidget(new QLabel("Waypoint:\t" + geocache->waypoint));
    l->addWidget(new QLabel(QString("Owner:\t%1").arg(geocache->owner)));
    l->addWidget(new QLabel(QString("Hidden:\t%1").arg(geocache->placed->
      toString(Qt::SystemLocaleLongDate))));
    l->addWidget(new QLabel(QString("Size:\t%1").
      arg(sizeToText(geocache->size))));
    l->addWidget(new QLabel(QString("Difficulty:\t%1").
      arg(qreal(geocache->difficulty) / 2.0, 0, 'f', 1)));
    l->addWidget(new QLabel(QString("Terrain:\t%2").
      arg(qreal(geocache->terrain) / 2.0, 0, 'f', 1)));
  }
}

InfoTab::~InfoTab() {
}

/** Geocache information dialog */
GeocacheInfoWidget::GeocacheInfoWidget(Geocache * geocache, QWidget * parent) :
  QWidget(parent), geocache_(geocache) {

  if(!geocache) {
    // No geocache selected
    QVBoxLayout * layout = new QVBoxLayout;
    QLabel * label = new QLabel(tr("No geocache selected"));
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label, Qt::AlignCenter);
    setLayout(layout);

  } else {
    // Grid layout as main layout
    QGridLayout * mainLayout = new QGridLayout(this);

    // geocache name
    mainLayout->addWidget(new QLabel("<big><b>" + geocache->name +
      "</b></big>"), 0, 0, Qt::AlignLeft);

    // geocache icon
    QLabel * pic = new QLabel;
    pic->setPixmap(geocacheIcon(geocache));
    mainLayout->addWidget(pic, 0, 1, Qt::AlignRight);

    // tab pages
    // setup description browser
    QTextBrowser * descBrowser = new QTextBrowser();
    descBrowser->setHtml(geocache->desc);
    descBrowser->setOpenExternalLinks(true);

    // prepare tab widgets
    QTabWidget * tab = new QTabWidget;
    tab->addTab(new InfoTab(geocache), "General");
    tab->addTab(descBrowser, "Description");

    mainLayout->addWidget(tab, 1, 0, 1, 2);

    // button box
    QDialogButtonBox * buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok);
    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    mainLayout->addWidget(buttonBox, 2, 0, 1, 2, Qt::AlignRight);

    setLayout(mainLayout);
  }
}

GeocacheInfoWidget::~GeocacheInfoWidget() {
}
