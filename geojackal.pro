TEMPLATE = app
CONFIG += debug_and_release \
    ordered
TARGET = geojackal
QT = core \
    gui \
    network \
    sql
INCLUDEPATH += src/
SOURCES = src/main.cpp \
  src/ui/MainWindow.cpp \
  src/ui/GCSpiderDialog.cpp \
  src/ui/PrefDialog.cpp \
  src/ui/GeocacheInfoWidget.cpp \
  src/ui/OsmSlippyMap.cpp \
  src/ui/CoordinateDialog.cpp \
  src/import/GCSpiderCachePage.cpp \
  src/import/GCSpider.cpp \
  src/logic/SettingsManager.cpp \
  src/logic/Geocache.cpp \
  src/logic/Failure.cpp \
  src/logic/GeocacheModel.cpp \
  src/logic/Coordinate.cpp \

HEADERS = src/global.h \
  src/ui/GeocacheInfoWidget.h \
  src/ui/PrefDialog.h \
  src/ui/OsmSlippyMap.h \
  src/ui/GCSpiderDialog.h \
  src/ui/CoordinateDialog.h \
  src/ui/MainWindow.h \
  src/import/GCSpiderCachePage.h \
  src/import/GCSpider.h \
  src/logic/SettingsManager.h \
  src/logic/Geocache.h \
  src/logic/Failure.h \
  src/logic/GeocacheModel.h \
  src/logic/Coordinate.h \

RESOURCES = resource/geojackal.qrc
RC_FILE = resource/geojackal.rc
DEFINES += GIT_REVISION=$$system(git log -1 --pretty='%H')
TARGET.CAPABILITY = true;
