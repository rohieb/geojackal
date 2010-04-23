TEMPLATE = app
TARGET = test
QT += core
SOURCES = *.cpp
unix:LIBS += -lboost_unit_test_framework-mt
DEFINES += BOOST_TEST_DYN_LINK
INCLUDEPATH += ../src/

