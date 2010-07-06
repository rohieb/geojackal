TEMPLATE = app
CONFIG += debug_and_release \
    ordered
TARGET = GeoJackal
QT = core \
    gui
SOURCES = src/Coordinate.cpp \
    src/GCSpider.cpp \
    src/GCSpiderCachePage.cpp \
    src/MainWindow.cpp \
    src/main.cpp
HEADERS = src/Cache.h \
    src/Coordinate.h \
    src/GCSpider.h \
    src/GCSpiderCachePage.h \
    src/MainWindow.h
FORMS = src/MainWindow.ui
RESOURCES = resource/geojackal.qrc
