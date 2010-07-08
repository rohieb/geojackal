#include "MainWindow.h"

#include <QtGui>
#include <QApplication>
#include "Cache.h"
#include "GCSpider.h"
#include "Failure.h"

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

int main(int argc, char *argv[]) {
  GeojackalApplication app(argc, argv);

  QVector<Cache *> * caches = new QVector<Cache *> ;
  GCSpider spider("username", "mypassword");
  spider.nearest(Coordinate(51, 10), 0.5, *caches);
  qDebug() << *caches;
  //MainWindow w;
  //w.show();
  //return app.exec();
  return 0;
}
