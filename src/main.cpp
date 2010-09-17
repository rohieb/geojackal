/**
 * @file main.cpp
 * Entry point for the application
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
 
#include "global.h"
#include "ui/MainWindow.h"
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
