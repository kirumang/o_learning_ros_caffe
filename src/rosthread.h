#ifndef ROSTHREAD_H
#define ROSTHREAD_H

#include <QString>
#include <QThread>

#include "ros/ros.h"
#include <ros/package.h>

#include "std_msgs/String.h"
#include <image_transport/image_transport.h>
#include <std_msgs/String.h>
#include <cv_bridge/cv_bridge.h>
#include <sstream>

#include "Classifier.h"
#include "trainnet.h"



const std::string RECEIVE_IMG_TOPIC_NAME = "/image_raw";
//const std::string caffe_root = "/home/kiru/caffe/";




void imageCallback(const sensor_msgs::ImageConstPtr& msg);

class Rosthread : public QThread
{
        Q_OBJECT

public:
   void run();
   void init();

   Deep_Classifier* classifier;
   TrainNet* trainer;

   ros::NodeHandle nh;

   std::string model_path;
   std::string weights_path;
   std::string mean_file;
   std::string label_file;
   std::string image_path;

   void Classified(const std::vector<Prediction>& predictions);
   int mode=0; //0-classifier, 1-data collection mode, 2-train mode
   void setMode(int i_mode);
   int getMode() {return mode;}
   QString init_Layer();
   void train_new_label();



signals:
   void FinishClassify(QString first, float second);


private:

   // void FinishCount(const int value);

};

#endif // ROSTHREAD_H
