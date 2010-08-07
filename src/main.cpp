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

  // instantiate here to get the app name etc.
  g_settings = SettingsManager::instance();

  MainWindow w;
#if defined(Q_OS_SYMBIAN) || defined(Q_OS_WINCE_WM)
  w.showMaximized();
#else
  w.resize(640, 480);
#endif
  w.show();

  return app.exec();
}
