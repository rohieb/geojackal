/**
 * @file OsmSlippyMap.h
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

#ifndef OSMSLIPPYMAP_H_
#define OSMSLIPPYMAP_H_

#include "global.h"
#include "logic/Coordinate.h"
#include "logic/Geocache.h"
#include <QtGui>
#include <QList>
#include <QtNetwork>

namespace geojackal {

/**
 * Slippy map widget using OpenStreetMap data.
 * The map is loaded on-the-fly from the OpenStreetMap tile servers. It can be
 * panned by the user to provide a primitive way of navigation.
 *
 * @todo "cross hair" for coordinate selection
 */
class OsmSlippyMap : public QWidget {
  Q_OBJECT
public:

  /** Height and width of each tile, in pixels */
  static const ushort TILE_DIM;
  /** Maximum zoom level (in OSM zoom level units...) */
  static const uchar MAX_ZOOM;

  OsmSlippyMap(QWidget * parent = 0, const Coordinate& center = COORD_INVALID,
    const uchar zoom = 16,
    const QDir& cacheDir = QDir(QCoreApplication::applicationDirPath()));
  virtual ~OsmSlippyMap();

  /** Return a reasonable size of the widget */
  virtual QSize sizeHint() const {
    return QSize(140, 140);
  }

  /**
   * Set the center coordinate. This is the coordinate that is shown in the
   * center of the widget. If the center coordinate is changed, the map will
   * be panned to the new coordinate.
   * @param coord New center coordinate
   */
  inline void setCenter(const Coordinate& coord) {
    center_ = coord;
    invalidate();
    emit centerChanged(center_);
  }
  /**
   * Get the current center coordinate.
   * @return The current center coordinate.
   */
  inline Coordinate center() const {
    return center_;
  }

  /**
   * Set the zoom level.
   * @param zoom The new zoom level, ranging from 0 (world map) to 18 (detail).
   * @throws Failure if anything goes wrong
   */
  inline void setZoom(const uchar zoom) {
    if(zoom > MAX_ZOOM) {
      throw Failure("Zoom level must be between 0 and 18");
    }
    zoomLevel_ = zoom;
    invalidate();
  }
  /**
   * Get the current zoom level.
   * @return The current zoom level, ranging from 0 (world map) to 18 (detail).
   */
  inline uchar zoom() const {
    return zoomLevel_;
  }

  /**
   * Enable or disable the zoom buttons.
   * Note: if zoom buttons are disabled, the user is still able to zoom by
   * double clicking.
   * @param b The new state of the zoom buttons, @c true to display the buttons,
   * or @c false to hide them.
   */
  inline void setDrawZoomButtons(bool b) {
    drawZoomButtons_ = b;
    invalidate();
  }

  /**
   * Get the state of the zoom buttons
   * @return @c true if the zoom buttons are displayed, @c false otherwise
   */
  inline bool drawZoomButtons() {
    return drawZoomButtons_;
  }

  /** Get the directory where the map tiles are cached */
  inline QDir cacheDir() {
    return cacheDir_;
  }

  void setCaches(QList<Geocache *> geocaches) {
    geocacheList = geocaches;
  }

signals:
  /**
   * Emitted if the user clicks on a geocache icon
   * @param gc The geocache the user clicked on
   */
  void clicked(Geocache * gc);
  /**
   * Emitted if the center has changed, particularly when the user has dragged
   * the map.
   * @param c The new center coordinate
   */
  void centerChanged(Coordinate& c);

protected:
  void download(const uint xTile, const uint yTile);
  void invalidate();
  QPoint tileToPixel(const QPoint& tileCoord);

  virtual void paintEvent(QPaintEvent *event);
  virtual void resizeEvent(QResizeEvent *);
  virtual void mouseDoubleClickEvent(QMouseEvent * event);
  virtual void mousePressEvent(QMouseEvent * event);
  virtual void mouseMoveEvent(QMouseEvent * event);
  virtual void wheelEvent(QWheelEvent * event);
  virtual void keyPressEvent(QKeyEvent * event);

protected slots:
  void httpFinished(QNetworkReply * reply);

private:
  /**
   * QNetworkAccessManager instance for HTTP communication with the tile
   * servers
   */
  QNetworkAccessManager * pnam_;
  /** The directory where the map tiles are cached */
  QDir cacheDir_;
  /** The empty tile */
  QPixmap emptyTile_;
  /** Zoom level, ranging from 0 (world map) to 18 (detail) */
  uchar zoomLevel_;
  /** Whether to draw the zoom buttons */
  bool drawZoomButtons_;
  /** Coordinate that is currently centered */
  Coordinate center_;
  /** Offset for top-left tile */
  QPoint offset_;
  /** Currently loaded tiles in memory, indexed by tile coordinates */
  QHash<QPoint, QPixmap> tilePixmaps_;
  /** Currently shown tile area */
  QRect shownTiles_;
  /** Mouse drag start position */
  QPoint dragPos;

  /** Position of the zoom buttons, pixels from upper-left corner of widget */
  static const QPoint zoomButtonTopLeft;
  /** Size (height and width) of the zoom buttons, in pixels */
  static const uint zoomButtonSize;
  /** Horizontal padding between the zoom buttons, in pixels */
  static const uint zoomButtonPadding;

  /** List of geocaches */
  QList<Geocache *> geocacheList;
  QHash<QRect, Geocache *> geocacheRects;
};

QPixmap geocacheIcon(Geocache * geocache);
QPointF geoToTile(const Coordinate& coord, const uchar zoom);
Coordinate tileToGeo(const QPointF tile, const uchar zoom);

}
#endif /* OSMSLIPPYMAP_H_ */
