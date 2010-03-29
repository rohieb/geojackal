#include "geojackal.h"

#include <QtGui>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    GeoJackal w;
    w.show();
    return a.exec();
}
