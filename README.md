# o_learning_ros_caffe
Purpose : Make robot autonomously collect image data after one-point supervision of human
          
          Target Environment (Done)
          Qt GUI Base + ROS Indigo (Receive image data from Kinect using OpenNI) + Caffe (for Deep learning)
          Pre-trained network : VGGNet, realtime image classification on GTX980-Ti Environment
          
          
          New Labeling Process
          [Collect New Data]
          After human point out a new object, Robot focus one the object and slightly move and re collect the image
          Robot can collect a bunch of image set during moving around the object
          [Learning]
          1. Resize & Initialize last Fully connected layer of the VGGnet (Output size + 1, and all of remains are fixed)
          2. learning the imagenet Data + Newly Collected Data



Requirements
ROS : Indigo
Caffe : BVLC (https://github.com/BVLC/caffe)
Reference : https://github.com/tzutalin/ros_caffe (ros_caffe sample, tzutalin)


Before you start, 

1. execute: './get_model.py' for downloading the pre-trained model

2. Check the Image Topic to perform realtime classification with image_raw topic from ROS in rosthread.h
   const std::string RECEIVE_IMG_TOPIC_NAME = "/image_raw";

3. If you want to use VGGNet, manually download the network file from caffe model zoo
   main page : https://gist.github.com/ksimonyan/fd8800eeb36e276cd6f9#file-readme-md
   name: CNN_S model from the BMVC-2014 paper: "Return of the Devil in the Details: Delving Deep into Convolutional Nets"
   mean_file_mat: http://www.robots.ox.ac.uk/~vgg/software/deep_eval/releases/bvlc/VGG_mean.mat
   mean_file_proto: http://www.robots.ox.ac.uk/~vgg/software/deep_eval/releases/bvlc/VGG_mean.binaryproto
   caffemodel: VGG_CNN_S
   caffemodel_url: http://www.robots.ox.ac.uk/~vgg/software/deep_eval/releases/bvlc/VGG_CNN_S.caffemodel
   license: non-commercial use only
   caffe_version: trained using a custom Caffe-based framework


    
    
    



