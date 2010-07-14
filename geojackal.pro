TEMPLATE = app
CONFIG += debug_and_release \
    ordered
TARGET = GeoJackal
QT = core \
    gui \
    network \
    sql
SOURCES = src/Cache.cpp \
    src/PrefDialog.cpp \
    src/GCSpiderDialog.cpp \
    src/CacheModel.cpp \
    src/OsmSlippyMap.cpp \
    src/Failure.cpp \
    src/functions.cpp \
    src/Coordinate.cpp \
    src/GCSpider.cpp \
    src/GCSpiderCachePage.cpp \
    src/MainWindow.cpp \
    src/main.cpp
HEADERS = src/PrefDialog.h \
    src/GCSpiderDialog.h \
    src/CacheModel.h \
    src/OsmSlippyMap.h \
    src/global.h \
    src/Failure.h \
    src/functions.h \
    src/Cache.h \
    src/Coordinate.h \
    src/GCSpider.h \
    src/GCSpiderCachePage.h \
    src/MainWindow.h
FORMS = src/MainWindow.ui
RESOURCES = resource/geojackal.qrc
TARGET.CAPABILITY = true;
