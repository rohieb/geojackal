/**
 * @file OsmSlippyMap.h
 * @date Jul 9, 2010
 * @author Roland Hieber <rohieb@rohieb.name>
 */

#ifndef OSMSLIPPYMAP_H_
#define OSMSLIPPYMAP_H_

#include "global.h"
#include "Coordinate.h"
#include <QtGui>
#include <QtNetwork>

namespace geojackal {

/**
 * Slippy map widget using OpenStreetMap data.
 * The map is loaded on-the-fly from the OpenStreetMap tile servers. It can be
 * panned by the user to provide a primitive way of navigation.
 */
class OsmSlippyMap : public QWidget {
  Q_OBJECT
public:

  /** Height and width of each tile, in pixels */
  static const ushort TILE_DIM;
  /** Maximum zoom level (in OSM zoom level units...) */
  static const uchar MAX_ZOOM;

  OsmSlippyMap(const Coordinate& center, const uchar zoom,
    QWidget * parent = 0);
  virtual ~OsmSlippyMap();

  /**
   * Set the center coordinate. This is the coordinate that is shown in the
   * center of the widget. If the center coordinate is changed, the map will
   * be panned to the new coordinate.
   * @param coord New center coordinate
   */
  inline void setCenter(const Coordinate& coord) {
    center_ = coord;
    invalidate();
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
   * @param The new zoom level, ranging from 0 (world map) to 18 (detail).
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


protected:
  void download(const uint xTile, const uint yTile);
  void invalidate();
  QRect tileRect(const QPoint& tileCoord);
  void paintEvent(QPaintEvent *event);
  void resizeEvent(QResizeEvent *);
  void mouseDoubleClickEvent(QMouseEvent * event);
  void mousePressEvent(QMouseEvent * event);
  void mouseMoveEvent(QMouseEvent * event);
  void mouseReleaseEvent(QMouseEvent *);
  void wheelEvent(QWheelEvent * event);

protected slots:
  void httpFinished(QNetworkReply * reply);

private:
  /**
   * QNetworkAccessManager instance for HTTP communication with the tile
   * servers
   */
  QNetworkAccessManager * pnam_;
  /** The empty tile */
  QPixmap emptyTile_;
  /** Zoom level, ranging from 0 (world map) to 18 (detail) */
  uchar zoomLevel_;
  /** Coordinate that is currently centered */
  Coordinate center_;
  /** Offset for top-left tile */
  QPoint offset_;
  /** Currently loaded tiles in memory, indexed by tile coordinates */
  QHash<QPoint, QPixmap> tilePixmaps_;
  /** Currently shown tile area */
  QRect shownTiles_;
  /** stuff to detect mouse drags */
  //bool mouseDrag;
  QPoint dragPos;

};

}

#endif /* OSMSLIPPYMAP_H_ */
