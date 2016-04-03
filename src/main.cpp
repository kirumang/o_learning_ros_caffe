#include <QApplication>

#include "mainwindow.h"




int main(int argc, char **argv)
{

    ros::init(argc, argv, "test_node");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

