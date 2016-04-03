# o_learning_ros_caffe

This combination of ros & caffe & Qt GUI Library

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


    



