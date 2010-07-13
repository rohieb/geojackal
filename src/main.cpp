#include "MainWindow.h"

#include "global.h"
#include "Cache.h"
#include "GCSpider.h"
#include "OsmSlippyMap.h"
#include <QtGui>
#include <QApplication>

using namespace geojackal;

QVector<Cache *> * caches = new QVector<Cache *>;

class GeojackalApplication : public QApplication {
public:
  GeojackalApplication(int& argc, char ** argv) :
    QApplication(argc, argv) {
    // we want to do some cleanup afterwards
    connect(this, SIGNAL(aboutToQuit()), qApp, SLOT(onQuit()));
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
    return false;
  }

protected slots:
  /** cleanup */
  void onQuit() {
    // FIXME
    qDebug() << "cleaning up";
    foreach(Cache * c, *caches) {
      delete c;
    }
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
  GeojackalApplication::setApplicationName(APPNAME);

  /*
    // spider
    try {
      GCSpider spider("username", "password");
      spider.nearest(Coordinate(Angle(52, 16, 22.79), Angle(10, 31, 30.87)), 0.2,
        *caches);
    } catch(Failure& f) {
      // @todo maybe a nice dialog box
      qCritical() << "Exception thrown:" << f.what();
    }
    qDebug() << "nearest to 51 N 10 E:";
    foreach(Cache * c, *caches)
      {
        qDebug() << *c;
      }
  */
  MainWindow w;
  w.resize(640, 480);
  w.show();

  return app.exec();
}
