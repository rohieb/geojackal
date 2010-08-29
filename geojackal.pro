TEMPLATE = app
CONFIG += debug_and_release \
    ordered
TARGET = geojackal
QT = core \
    gui \
    network \
    sql
SOURCES = src/SettingsManager.cpp \
    src/GeocacheInfoWidget.cpp \
    src/Geocache.cpp \
    src/PrefDialog.cpp \
    src/GCSpiderDialog.cpp \
    src/GeocacheModel.cpp \
    src/OsmSlippyMap.cpp \
    src/Failure.cpp \
    src/Coordinate.cpp \
    src/GCSpider.cpp \
    src/GCSpiderCachePage.cpp \
    src/MainWindow.cpp \
    src/main.cpp
HEADERS = src/SettingsManager.h \
    src/GeocacheInfoWidget.h \
    src/PrefDialog.h \
    src/GCSpiderDialog.h \
    src/GeocacheModel.h \
    src/OsmSlippyMap.h \
    src/global.h \
    src/Failure.h \
    src/Geocache.h \
    src/Coordinate.h \
    src/GCSpider.h \
    src/GCSpiderCachePage.h \
    src/MainWindow.h
RESOURCES = resource/geojackal.qrc
RC_FILE = resource/geojackal.rc
unix:DEFINES += GIT_REVISION=$$system(git log -1 --pretty='%H')
TARGET.CAPABILITY = true;
