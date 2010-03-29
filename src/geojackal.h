#ifndef GEOJACKAL_H
#define GEOJACKAL_H

#include <QtGui/QWidget>
#include "ui_geojackal.h"

class GeoJackal : public QWidget
{
    Q_OBJECT

public:
    GeoJackal(QWidget *parent = 0);
    ~GeoJackal();

private:
    Ui::GeoJackalClass ui;
};

#endif // GEOJACKAL_H
