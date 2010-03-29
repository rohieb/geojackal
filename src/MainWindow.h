#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QWidget>
#include "ui_MainWindow.h"

class MainWindow : public QWidget
{
public:
    MainWindow(QWidget *parent = 0);
    virtual ~MainWindow();

private:
    Ui::MainWindowClass ui;
};

#endif // MAINWINDOW_H
