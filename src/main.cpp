#include <QtGui/QApplication>
#include "mainwindow.h"

#include "modelworld.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    ModelWorld mw ("World.xml");


    return a.exec();
}
