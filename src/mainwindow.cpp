
#include <QtGui>
#include <QMainWindow>

#include "mainwindow.h"
#include "ui_mainwindow.h"

int count;

void MainWindow::on_thread_read(QString first, float second)
{

    QString temp;
    temp.sprintf("_has predicted, Prob = %.4f",second);
    temp=first+temp;


    ui->textEdit->setText(temp);
    return;
}


MainWindow::MainWindow(QWidget *parent):QMainWindow(parent),ui(new Ui::MainWindow)
{


    count=0;
    //ros::spin();
    this->rost = new Rosthread();
    connect(rost, SIGNAL(FinishClassify(QString, float)),this, SLOT(on_thread_read(QString, float)));
    rost->init();
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
   delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    //this->rost->start();

   // ros::Subscriber sub = n.subscribe("chatter", 1000, &RosThread::chatterCallback, this);

 //   test_pkg::msgTutorial msg;
 //   msg.data=count;

    ROS_INFO("send msg=%d", count);


    /*
    cv::Mat img = cv::imread(image_path, -1);
    std::vector<Prediction> predictions = classifier->Classify(img);

    std::cout << "Test default image under /data/cat.jpg" << std::endl;
    for (size_t i = 0; i < predictions.size(); ++i) {
        Prediction p = predictions[i];
        std::cout << std::fixed << std::setprecision(4) << p.second << " - \"" << p.first << "\"" << std::endl;
    }
    */

    count++;
    rost->setMode(0); //Classification Mode
    rost->start();


    ros::spinOnce();
    return;
}

void MainWindow::on_layerinit_clicked()
{

    rost->setMode(2); //Train Mode (This time just initialization)

    //rost->start();


    ros::spinOnce();
    return;
}

void MainWindow::on_layersave_clicked()
{
    //Train New Label
    //
    ui->textEdit_2->setText(rost->init_Layer());

    rost->quit();
    rost->setMode(1);

    ros::spinOnce();

    return;
}

void MainWindow::on_layerloadtoclassifier_clicked()
{
    //rost->load_model();

}
