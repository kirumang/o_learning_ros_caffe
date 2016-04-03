#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui>
#include <QMainWindow>
#include "ui_mainwindow.h"
#include "rosthread.h"


//#include "test_pkg/msgTutorial.h"

namespace test_name {
 class MainWindow;
}

//void imageCallback(const sensor_msgs::ImageConstPtr& msg);


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void on_thread_read(QString first, float second);


    void on_layerinit_clicked();

    void on_layersave_clicked();

    void on_layerloadtoclassifier_clicked();

public:
    ros::NodeHandle nh;
    ros::Publisher chatter_pub;
    image_transport::ImageTransport* it;
    //image_transport::Subscriber sub;



    void publishRet(const std::vector<Prediction>& predictions);

private:
    Ui::MainWindow *ui;
    Rosthread *rost;

};




#endif // MAINWINDOW_H

