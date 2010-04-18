TEMPLATE = app
CONFIG += debug_and_release \
    ordered
TARGET = GeoJackal
QT += core \
    gui
SOURCES += src/*.cpp
HEADERS = src/*.h
FORMS += src/MainWindow.ui
