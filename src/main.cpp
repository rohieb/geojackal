#include "MainWindow.h"

#include <QtGui>
#include <QApplication>
#include "global.h"
#include "Cache.h"
#include "GCSpider.h"

using namespace geojackal;

class GeojackalApplication : public QApplication {
public:
  GeojackalApplication(int& argc, char ** argv) :
    QApplication(argc, argv) {
  }
  virtual ~GeojackalApplication() {
  }

  // reimplemented from QApplication so we can throw Failures in slots
  virtual bool notify(QObject * receiver, QEvent * event) {
    try {
      return QApplication::notify(receiver, event);
    } catch(Failure& f) {
      // @todo maybe a nice dialog box
      qCritical() << "Exception thrown:" << f.what();
    }
    return false; // maybe this is ok
  }
};

QDebug& operator<<(QDebug dbg, Cache& cache) {
  dbg << "{ wp:" << cache.waypoint;
  dbg << ", name:" << cache.name;
  dbg << ", coord:" << cache.coord->lon << "," << cache.coord->lat;
  dbg << ", type:" << cache.type;
  dbg << ", size:" << cache.size;
  dbg << ", diff:" << cache.difficulty;
  dbg << ", terr:" << cache.terrain;
  dbg << ", placed:" << cache.placed;
  dbg << ", owner:" << cache.owner;
  dbg << ", archived:" << cache.archived;
  dbg << " }";
  return dbg.nospace();
}
int main(int argc, char *argv[]) {
  GeojackalApplication app(argc, argv);

  Cache cache;
  QVector<Cache *> * caches = new QVector<Cache *> ;
  try {
    GCSpider spider("username", "mypassword");
    spider.nearest(Coordinate(Angle(52, 16, 22.79), Angle(10, 31, 30.87)),
      0.5, *caches);
    for(QVector<Cache *>::iterator it = caches->begin(); it != caches->end();
      ++it) {
      qDebug() << *(*it);
      delete (*it); // clean up
    }
    //spider.loadCache("GC1552D", cache);
    //qDebug() << cache;
  } catch(Failure& f) {
    // @todo maybe a nice dialog box
    qCritical() << "Exception thrown:" << f.what();
  }
  //qDebug() << "nearest to 51 N 10 E:" << *caches;
  //MainWindow w;
  //w.show();
  //return app.exec();
  return 0;
}
