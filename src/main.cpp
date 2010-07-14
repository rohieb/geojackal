#include "MainWindow.h"

#include "global.h"
#include "Cache.h"
#include "GCSpider.h"
#include "OsmSlippyMap.h"
#include <QtGui>
#include <QApplication>

namespace geojackal {

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
    return false;
  }
};

}

using namespace geojackal;

int main(int argc, char *argv[]) {
  GeojackalApplication app(argc, argv);
  GeojackalApplication::setApplicationName(APPNAME);
  GeojackalApplication::setApplicationVersion(VERSION);
  GeojackalApplication::setOrganizationDomain("rohieb.name");
  GeojackalApplication::setOrganizationName(APPNAME);

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
