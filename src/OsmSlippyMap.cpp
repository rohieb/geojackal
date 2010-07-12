/**
 * @file OsmSlippyMap.cpp
 * @date Jul 9, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#include "OsmSlippyMap.h"
#include <cmath>
#include <QPointF>

using namespace geojackal;

#define PI 3.1412653589

const ushort OsmSlippyMap::TILE_DIM = 256;

/**
 * Primitive hash function for QPoints, but suffice for our needs
 */
uint qHash(const QPoint& p) {
  return p.x() * 17 ^ p.y();
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
QPointF tileForCoordinate(const Coordinate& coord, const uchar zoom) {
  if(zoom > 18) {
    throw Failure("Zoom level must be between 0 and 18");
  }
  int n = (1 << zoom); // number of tiles on the zoom level
  float xTile = (coord.lon + 180.0) / 360.0 * n;
  float yTile = (1.0 - (log(tan(coord.lat * PI / 180.0) + 1.0 / cos(coord.lat
    * PI / 180.0)) / PI)) / 2.0 * n;
  return QPointF(xTile, yTile);
}

/**
 * Calculate the coordinate of the northwestern edge of a given tile. To
 * retrieve the coordinate of the southeastern edge, add 1 to the tile
 * coordinates.
 * @param xTile x tile coordinate
 * @param yTile y tile coordinate
 * @param zoom Zoom level, ranging from 0 (whole world) to 18 (detail)
 * @param buf Buffer that receives the coordinate of the northwestern
 *  coordinate of the given tile
 * @throws Failure if anything goes wrong
 * @see http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames for an
 * explanation what is done here
 */
void coordFromTile(const float xTile, const float yTile, const uchar zoom,
  Coordinate& buf) {
  if(zoom > 18) {
    throw Failure("Zoom level must be between 0 and 18");
  }
  uint zn = (1 << zoom); // number of tiles on the zoom level
  float lon = xTile / zn * 360.0 - 180.0;
  float lat = atan(sinh(PI * (1 - 2 * yTile / zn))) * 180.0 / PI;

  buf = Coordinate(lat, lon);
}

/**
 * Constructor.
 */
OsmSlippyMap::OsmSlippyMap(const Coordinate& center, const uchar zoom,
  QWidget * parent) :
  QWidget(parent), pnam_(0), zoomLevel_(zoom), center_(center) {
  pnam_ = new QNetworkAccessManager;
  emptyTile_ = QPixmap(TILE_DIM, TILE_DIM);
  emptyTile_.fill(Qt::lightGray);

  // set up network cache so we don't have to load every tile multiple times
  QNetworkDiskCache * cache = new QNetworkDiskCache;
  cache->setCacheDirectory(QDesktopServices::storageLocation
    (QDesktopServices::CacheLocation));
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
  QPointF centerTile = tileForCoordinate(center_, zoomLevel_);
  float centerX = centerTile.x();
  float centerY = centerTile.y();

  // top-left corner of the center tile
  uint centerTop = height() / 2 - (centerY - floor(centerY)) * TILE_DIM;
  uint centerLeft = width() / 2 - (centerX - floor(centerX)) * TILE_DIM;

  // first tile vertical and horizontal
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
  qDebug() << "loading map" << url.toString();
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
    tilePixmaps_[tileCoord] = emptyTile_;
  } else {
    tilePixmaps_[tileCoord] = QPixmap::fromImage(img);
  }
  rply->deleteLater();

  // update the map so the tile is shown
  update(tileRect(tileCoord));
}

/**
 * Get rectangle of a tile, in client coordinates
 * @param tileCoord Tile coordinates
 * @return QRect with client coordinates
 */
QRect OsmSlippyMap::tileRect(const QPoint& tileCoord) {
  QPoint t = tileCoord - shownTiles_.topLeft();
  int x = t.x() * TILE_DIM + offset_.x();
  int y = t.y() * TILE_DIM + offset_.y();
  return QRect(x, y, TILE_DIM, TILE_DIM);
}

/** from QWidget */
void OsmSlippyMap::paintEvent(QPaintEvent * event) {
  QPainter p;
  p.begin(this);

  // draw tiles
  for(int x = 0; x <= shownTiles_.width(); ++x) {
    for(int y = 0; y <= shownTiles_.height(); ++y) {
      QPoint tileCoord(x + shownTiles_.left(), y + shownTiles_.top());
      QRect box = tileRect(tileCoord);
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

  // draw copyright text
  p.setPen(Qt::black);
#if defined(Q_OS_SYMBIAN)
  QFont font = p.font();
  font.setPixelSize(13);
  p.setFont(font);
#endif
  p.drawText(rect(), Qt::AlignBottom | Qt::TextWordWrap,
    "Map data CC-BY-SA 2009 OpenStreetMap.org contributors");
  p.end();
}

/** from QWidget */
void OsmSlippyMap::resizeEvent(QResizeEvent *) {
  invalidate();
}

void OsmSlippyMap::mousePressEvent(QMouseEvent * event) {
  // drag on left button
  if(event->buttons() != Qt::LeftButton)
    return;
  dragPos = event->pos();
}

void OsmSlippyMap::mouseMoveEvent(QMouseEvent * event) {
  // Mouse drag, move map
  if(!event->buttons()) {
    // only with buttons pressed
    return;
  }

  QPointF dx = QPointF(event->pos() - dragPos) / float(TILE_DIM);
  QPointF newCenter = tileForCoordinate(center_, zoomLevel_) - dx;
  coordFromTile(newCenter.x(), newCenter.y(), zoomLevel_, center_);
  dragPos = event->pos();
  invalidate();
}

void OsmSlippyMap::mouseReleaseEvent(QMouseEvent *) {
  invalidate();
}

