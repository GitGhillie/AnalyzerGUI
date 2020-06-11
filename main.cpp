#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow w;
    //QObject::connect(&w, &MainWindow::accepted, &app, &QApplication::quit);
    w.show();
    return app.exec();
}
