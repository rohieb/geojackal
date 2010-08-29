/**
 * @file OsmSlippyMap.cpp
 * @date Jul 9, 2010
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

#include "OsmSlippyMap.h"
#include <cmath>
#include <QPointF>

using namespace geojackal;

const ushort OsmSlippyMap::TILE_DIM = 256;
const uchar OsmSlippyMap::MAX_ZOOM = 18;

const QPoint OsmSlippyMap::zoomButtonTopLeft(3, 3);
const uint OsmSlippyMap::zoomButtonSize = 15;
const uint OsmSlippyMap::zoomButtonPadding = 4;

/**
 * Primitive hash function for QPoints, but suffice for our needs
 */
uint qHash(const QPoint& p) {
  return p.x() * 17 ^ p.y();
}
/** Hash function for QRects */
uint qHash(const QRect& r) {
  // just make a string out of it
  return qHash(QString("%1,%2,%3,%4").arg(r.left()).arg(r.top()).
    arg(r.right()).arg(r.bottom()));
}

/**
 * Calculate the tile name for a given coordinate
 * @param coord Coordinate
 * @param zoom Zoom level, ranging from 0 (whole world) to 18 (detail)
 * @return A QPoint which contains the x and y tile coordinates. The integral
 *  parts of these values can be used to retrieve the tile from a OSM server,
 *  the floating-point parts are the positions on the tile.
 * @throws Failure if anything goes wrong
 * @see http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames for an
 * explanation what is done here
 */
QPointF geojackal::geoToTile(const Coordinate& coord, const uchar zoom) {
  if(zoom > OsmSlippyMap::MAX_ZOOM) {
    throw Failure("Zoom level must be between 0 and 18");
  }
  int n = (1 << zoom); // number of tiles along the x and y axis
  float xTile = (coord.lon + 180.0) / 360.0 * n;
  float yTile = (1.0 - (log(tan(coord.lat * M_PI / 180.0) + (1.0 /
    cos(coord.lat * M_PI / 180.0))) / M_PI)) / 2.0 * n;
  return QPointF(xTile, yTile);
}

/**
 * Calculate the coordinate of the northwestern edge of a given tile. To
 * retrieve the coordinate of the southeastern edge, add 1 to the tile
 * coordinates.
 * @param tile tile coordinates
 * @param zoom Zoom level, ranging from 0 (whole world) to 18 (detail)
 * @return The geodetic coordinate of the northwestern edge of the given tile
 * @throws Failure if anything goes wrong
 * @see http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames for an
 * explanation what is done here
 */
Coordinate geojackal::tileToGeo(const QPointF tile, const uchar zoom) {
  if(zoom > OsmSlippyMap::MAX_ZOOM) {
    throw Failure("Zoom level must be between 0 and 18");
  }
  uint zn = (1 << zoom); // number of tiles on the zoom level
  float lon = tile.x() / zn * 360.0 - 180.0;
  float lat = atan(sinh(M_PI * (1 - 2 * tile.y() / zn))) * 180.0 / M_PI;

  return Coordinate(lat, lon);
}

/**
 * Constructor.
 */
OsmSlippyMap::OsmSlippyMap(const Coordinate& center, const uchar zoom,
  const QDir& cacheDir, QWidget * parent) :
  QWidget(parent), pnam_(0), cacheDir_(cacheDir), zoomLevel_(zoom),
  center_(center) {

  pnam_ = new QNetworkAccessManager;
  emptyTile_ = QPixmap(TILE_DIM, TILE_DIM);
  emptyTile_.fill(Qt::lightGray);

  // set up network cache so we don't have to load every tile multiple times
  QNetworkDiskCache * cache = new QNetworkDiskCache;
  qDebug() << "caching maps in" << cacheDir.path();
  cache->setCacheDirectory(cacheDir.path());
  pnam_->setCache(cache);

  // handle incoming HTTP data
  connect(pnam_, SIGNAL(finished(QNetworkReply*)),
    this, SLOT(httpFinished(QNetworkReply*)));
}

OsmSlippyMap::~OsmSlippyMap() {
  if(pnam_ != 0) {
    delete pnam_;
  }
}

/**
 * @internal
 * Recalculate the widget
 */
void OsmSlippyMap::invalidate() {
  // center coordinates
  QPointF centerTile = geoToTile(center_, zoomLevel_);
  float centerX = centerTile.x();
  float centerY = centerTile.y();

  // top-left corner of the center tile, in pixels from upper-left widget edge
  uint centerTop = height() / 2 - (centerY - floor(centerY)) * TILE_DIM;
  uint centerLeft = width() / 2 - (centerX - floor(centerX)) * TILE_DIM;

  // first tile vertical and horizontal, in tile coordinates
  uint dx = (centerLeft + TILE_DIM - 1) / TILE_DIM;
  uint dy = (centerTop + TILE_DIM - 1) / TILE_DIM;
  uint firstX = static_cast<int>(centerX) - dx;
  uint firstY = static_cast<int>(centerY) - dy;

  // offset for top-left tile
  offset_ = QPoint(centerLeft - dx * TILE_DIM, centerTop - dy *
    TILE_DIM);

  // last tile vertical and horizontal
  uint lastX = static_cast<int> (centerX) + (width() - centerLeft - 1)
    / TILE_DIM;
  uint lastY = static_cast<int> (centerY) + (height() - centerTop - 1)
    / TILE_DIM;

  // build a rect out of it
  shownTiles_ = QRect(firstX, firstY, lastX - firstX + 1, lastY - firstY + 1);

  // download all needed tiles
  for(uint x = firstX; x <= lastX; ++x) {
    for(uint y = firstY; y <= lastY; ++y) {
      if(!tilePixmaps_.contains(QPoint(x, y))) {
        download(x, y);
      }
    }
  }
  // purge all unneeded tiles, leave margin of 2 tiles
  QRect neededTiles = shownTiles_.adjusted(-2, -2, 2, 2);
  foreach(QPoint p, tilePixmaps_.keys()) {
    if(!neededTiles.contains(p)) {
      tilePixmaps_.remove(p);
    }
  }
  update();
}

/**
 * Download a tile from the tile servers.
 * @param xTile X coordinate of the tile
 * @param yTile Y coordinate of the tile
 */
void OsmSlippyMap::download(const uint xTile, const uint yTile) {
  QPoint tileCoord(xTile, yTile);
  QString path = "http://tile.openstreetmap.org/%1/%2/%3.png";
  QUrl url(path.arg(zoomLevel_).arg(xTile).arg(yTile));
  QNetworkRequest request(url);
  QByteArray userAgent = QByteArray("GeoJackal/") + VERSION.toAscii();
  request.setRawHeader("User-Agent", userAgent);
  // save tile coordinates for later use
  request.setAttribute(QNetworkRequest::User, QVariant(tileCoord));
  pnam_->get(request);
}

/**
 * @internal
 * Handle incoming HTTP data
 */
void OsmSlippyMap::httpFinished(QNetworkReply * rply) {
  QImage img;
  if(rply->error() == QNetworkReply::NoError) {
    // load image from data
    img.load(rply, 0);
  }

  // put into our hash
  QPoint tileCoord = rply->request().attribute(QNetworkRequest::User).toPoint();
  if(img.isNull()) {
    qDebug() << "req" << rply->url().toString() << "is null";
    tilePixmaps_[tileCoord] = emptyTile_;
  } else {
    qDebug() << "got" << rply->url().toString();
    tilePixmaps_[tileCoord] = QPixmap::fromImage(img);
  }
  rply->deleteLater();

  // update the map so the tile is shown
  update(QRect(tileToPixel(tileCoord), QSize(256, 256)));
}

/**
 * Transform tile coordinate to pixels, starting at the widget's upper left
 * corner
 * @param tileCoord Tile coordinates
 * @return QRect with pixels from the widget's upper left
 * corner
 */
QPoint OsmSlippyMap::tileToPixel(const QPoint& tileCoord) {
  QPoint t = tileCoord - shownTiles_.topLeft();
  int x = t.x() * TILE_DIM + offset_.x();
  int y = t.y() * TILE_DIM + offset_.y();
  return QPoint(x, y);
}

/** return the icon for a geocache */
QPixmap geojackal::geocacheIcon(Geocache * geocache) {
  QString fileName;
  switch(geocache->type) {
    case TYPE_TRADI: fileName = "tradi.gif"; break;
    case TYPE_MULTI: fileName = "multi.gif"; break;
    case TYPE_MYSTERY: fileName = "mystery.gif"; break;
    case TYPE_EVENT: fileName = "event.gif"; break;
    case TYPE_VIRTUAL: fileName = "virtual.gif"; break;
    case TYPE_WEBCAM: fileName = "webcam.gif"; break;
    case TYPE_MEGAEVENT: fileName = "mega.gif"; break;
    case TYPE_LETTERBOX: fileName = "letterbox.gif"; break;
    case TYPE_WHEREIGO: fileName = "whereigo.gif"; break;
    case TYPE_CITO: fileName = "cito.gif"; break;
    case TYPE_EARTH: fileName = "earth.gif"; break;
    case TYPE_REVERSE: fileName = "reverse.gif"; break;
    case TYPE_GAME: fileName = "game.gif"; break;
    case TYPE_PROJECTAPE: fileName = "ape.gif"; break;
    case TYPE_STAGE: fileName = "stage.gif"; break;
    case TYPE_FINAL: fileName = "final.gif"; break;
    case TYPE_QUESTION: fileName = "question.gif"; break;
    case TYPE_REFERENCE: fileName = "reference.gif"; break;
    case TYPE_PARKING: fileName = "parking.gif"; break;
    case TYPE_TRAILHEAD: fileName = "trailhead.gif"; break;
    case TYPE_OTHER: fileName = "final.gif"; break;
    default: return QPixmap();
  }
  QPixmap icon;
  icon.load(":/cachetype/" + fileName);
  return icon;
}

/** from QWidget */
void OsmSlippyMap::paintEvent(QPaintEvent * event) {
  QPainter p;
  p.begin(this);

  // draw tiles
  for(int x = 0; x <= shownTiles_.width(); ++x) {
    for(int y = 0; y <= shownTiles_.height(); ++y) {
      QPoint tileCoord(x + shownTiles_.left(), y + shownTiles_.top());
      QRect box(tileToPixel(tileCoord), QSize(256, 256));
      // redraw only needed tiles
      if(event->rect().intersects(box)) {
        if(tilePixmaps_.contains(tileCoord)) {
          p.drawPixmap(box, tilePixmaps_.value(tileCoord));
        } else {
          p.drawPixmap(box, emptyTile_);
        }
      }
    }
  }

  // draw geocache icons
  geocacheRects.clear(); // or we get bogus positions after zooming etc.
  foreach(Geocache * gc, geocacheList) {
    QPixmap icon = geocacheIcon(gc).scaled(24, 24, Qt::KeepAspectRatio);
    QPointF tileCoordF = geoToTile(*gc->coord, zoomLevel_);
    QPointF t = tileCoordF - shownTiles_.topLeft();
    // FIXME we need a k-d-tree here
    int x = (int) (t.x() * TILE_DIM + offset_.x());
    int y = (int) (t.y() * TILE_DIM + offset_.y());
    QRect target(QPoint(x, y), QSize(24, 24));
    target.adjust(-12, -12, -12, -12);
    geocacheRects[target] = gc; // save for later
    p.drawPixmap(target, icon);
  }

  // draw copyright text
  p.setPen(Qt::black);
#if defined(Q_OS_SYMBIAN)
  QFont font = p.font();
  font.setPixelSize(13);
  p.setFont(font);
#endif
  p.drawText(rect(), Qt::AlignBottom | Qt::TextWordWrap,
    "Map data CC-BY-SA 2009 OpenStreetMap.org contributors");

  // draw zoom "buttons" in upper left corner
  QRect zoomBtn(zoomButtonTopLeft,
    QSize(zoomButtonSize - 1, zoomButtonSize - 1));
  p.fillRect(zoomBtn, Qt::white);
  p.drawRect(zoomBtn);
  float s = zoomButtonSize / 4.0f;
  p.drawLine(zoomBtn.x() + floor(s),
    zoomBtn.y() + floor(zoomButtonSize / 2.0f), zoomBtn.right() - floor(s) + 1,
    zoomBtn.y() + floor(zoomButtonSize / 2.0f));
  p.drawLine(zoomBtn.x() + floor(zoomButtonSize / 2.0f),
    zoomBtn.y() + floor(s), zoomBtn.x() + floor(zoomButtonSize / 2.0f),
    zoomBtn.bottom() - floor(s) + 1);
  zoomBtn.moveTop(zoomButtonSize + zoomButtonPadding);
  p.fillRect(zoomBtn, Qt::white);
  p.drawRect(zoomBtn);
  p.drawLine(zoomBtn.x() + floor(s),
    zoomBtn.y() + floor(zoomButtonSize / 2.0f), zoomBtn.right() - floor(s) + 1,
    zoomBtn.y() + floor(zoomButtonSize / 2.0f));

  p.end();
}

/** from QWidget */
void OsmSlippyMap::resizeEvent(QResizeEvent *) {
  invalidate();
}

void OsmSlippyMap::mousePressEvent(QMouseEvent * event) {
  // drag on left button
  if(event->buttons() == Qt::LeftButton) {
    // if on zoom button
    QRect zoomInBtn(zoomButtonTopLeft, QSize(zoomButtonSize, zoomButtonSize));
    QRect zoomOutBtn = zoomInBtn.adjusted(0, zoomButtonSize + zoomButtonPadding,
      0, zoomButtonSize + zoomButtonPadding);

    if(zoomInBtn.contains(event->pos())) {
      setZoom(zoom() + 1);
    } else if(zoomOutBtn.contains(event->pos())) {
      setZoom(zoom() - 1);
    } else {
      // not on zoom button, maybe on geocache icon
      foreach(QRect gcr, geocacheRects.keys()) {
        qDebug() << gcr << "?contains?" << event->pos();
        if(gcr.contains(event->pos())) {
          qDebug() << "you clicked on" << geocacheRects.value(gcr)->name;

          // FIXME emit signal
          //GeocacheInfoDialog dialog(geocacheRects.value(gcr), this);
          //dialog.exec();
          // only do it the first time
          event->accept();
          return;
        }
      }
      dragPos = event->pos();
    }
    event->accept();
  } else {
    event->ignore();
  }
}

void OsmSlippyMap::mouseMoveEvent(QMouseEvent * event) {
  // Mouse drag, move map
  if(!event->buttons()) {
    // only with buttons pressed
    event->ignore();
    return;
  }

  // drag distance in tile units
  QPointF dx = QPointF(event->pos() - dragPos) / float(TILE_DIM);
  QPointF newCenter = geoToTile(center_, zoomLevel_) - dx;
  setCenter(tileToGeo(newCenter, zoomLevel_));
  dragPos = event->pos();
  qDebug() << "Mouse move to " << center_;
  invalidate();
  event->accept();
}

void OsmSlippyMap::mouseDoubleClickEvent(QMouseEvent * event) {
  // zoom in on mouse position at left button double click
  if(event->buttons() == Qt::LeftButton) {
    setZoom(zoom() + 1);
    // calc geo coordinate of mouse click
    QPointF newCenter = geoToTile(center_, zoomLevel_);
    setCenter(tileToGeo(newCenter, zoomLevel_));
    qDebug() << "Mouse doubleclick at " << event->pos() << "=" << center_;
    invalidate();
    event->accept();
  } else {
    event->ignore();
  }
}

void OsmSlippyMap::wheelEvent(QWheelEvent * event) {
  // zoom on mouse wheel events
  int delta = event->delta();
  if(delta > 0) {
    setZoom(zoom() + 1);
    invalidate();
    event->accept();
  } else if(delta < 0) {
    setZoom(zoom() - 1);
    invalidate();
    event->accept();
  } else { // should not occur ...
    event->ignore();
  }
}

// arrow keys for panning
void OsmSlippyMap::keyPressEvent(QKeyEvent * event) {
  QPointF dx;
  bool handle = false;
  if(event->key() == Qt::Key_Left) {
    dx = QPointF(.1, 0);
    handle = true;
  }
  if(event->key() == Qt::Key_Right) {
    dx = QPointF(-.1, 0);
    handle = true;
  }
  if(event->key() == Qt::Key_Up) {
    dx = QPointF(0, .1);
    handle = true;
  }
  if(event->key() == Qt::Key_Down) {
    dx = QPointF(0, -.1);
    handle = true;
  }

  if(handle) {
    QPointF newCenter = geoToTile(center_, zoomLevel_) - dx;
    setCenter(tileToGeo(newCenter, zoomLevel_));
  } else {
    return QWidget::keyPressEvent(event);
  }
}
