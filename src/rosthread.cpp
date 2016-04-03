#include "rosthread.h"
#include <QDebug>

Rosthread* rost;

void imageCallback(const sensor_msgs::ImageConstPtr& msg)
{
    try {
        if(rost->getMode()==0)
        { //Classification Mode
          cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
          cv::Mat img = cv_ptr->image;
          std::vector<Prediction> predictions = rost->classifier->Classify(img);
          rost->Classified(predictions);
        }else if(rost->getMode()==1)
        { //Data Collection Mode -> Collect to XXX   (for same amount of images with image_net dataset




          //After end of the collection --> train the whole data and save the parameters and new layers
          //Need to study about how to print out current layers architectures and parameters and so on.


        }else if(rost->getMode()==2)
        { //Train New Dataset from pre-Trained Data set and save the data
            //Initialize Last Layer
            cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
            cv::Mat img = cv_ptr->image;
            std::vector<Prediction> predictions = rost->trainer->Classify(img);
            rost->Classified(predictions);


        }



    } catch (cv_bridge::Exception& e) {
          ROS_ERROR("Could not convert from '%s' to 'bgr8'.", msg->encoding.c_str());
      }

}

void Rosthread::Classified(const std::vector<Prediction>& predictions)
{
    Prediction p = predictions[0];
    QString qs;
    qs = QString::fromStdString(p.first);
    emit FinishClassify(qs, p.second);
}

QString Rosthread::init_Layer()
{
    QString result;
    result = trainer->init_fc_layer();
    return result;
}

void Rosthread::init()
{
    ROS_INFO("RosThread run called");
    rost=this;

    const std::string ROOT_SAMPLE = ros::package::getPath("o_learning");
    model_path = ROOT_SAMPLE + "/data/deploy.prototxt";
    weights_path = ROOT_SAMPLE + "/data/bvlc_reference_caffenet.caffemodel";
    mean_file = ROOT_SAMPLE + "/data/imagenet_mean.binaryproto";
    label_file = ROOT_SAMPLE + "/data/synset_words.txt";
    image_path = ROOT_SAMPLE + "/data/cat.jpg";


    classifier = new Deep_Classifier(model_path, weights_path, mean_file, label_file);
    //Point to Train File



    model_path = ROOT_SAMPLE + "/data/VGG_CNN_S_deploy.prototxt";
    weights_path = ROOT_SAMPLE + "/data/VGG_CNN_S.caffemodel";
    //model_path = ROOT_SAMPLE + "/data/VGG_CNN_S_finetune.prototxt";


    mean_file = ROOT_SAMPLE + "/data/VGG_mean.binaryproto";
    label_file = ROOT_SAMPLE + "/data/synset_words.txt";


    trainer = new TrainNet(model_path, weights_path, mean_file, label_file);
    //VGG net and try fine tuning of last fc8 layer for new images
}


void Rosthread::run()
{

   //chatter_pub = nh.advertise<std_msgs::String>("/caffe_ret", 100);

   //Point to Classification File
    image_transport::ImageTransport it(nh);
    image_transport::Subscriber sub = it.subscribe(RECEIVE_IMG_TOPIC_NAME, 1, imageCallback);



    ros::spin();
   // run();
    ROS_INFO("Thread_Finished");
}

void Rosthread::setMode(int i_mode)
{
    if(i_mode<3 || i_mode>=0) mode=i_mode;
    return;
}

void Rosthread::train_new_label()
{
    trainer->init_fc_layer();
}


