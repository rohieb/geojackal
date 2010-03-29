TEMPLATE = app
CONFIG += debug_and_release ordered

TARGET = GeoJackal 

QT += core gui xml network webkit 

SOURCES += src/MainWindow.cpp src/GCSpider.cpp src/main.cpp
FORMS += src/MainWindow.ui    

