/*
 * Classifier.h
 *
 *  Created on: Aug 31, 2015
 *      Author: Tzutalin
 */

#ifndef DEEPCLASSIFIER_H
#define DEEPCLASSIFIER_H


#include <iostream>
#include <vector>
#include <sstream>
#include <caffe/caffe.hpp>
#include "caffe/blob.hpp"
#include "caffe/common.hpp"
#include "caffe/proto/caffe.pb.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace caffe;
using std::string;

//typedef double Dtype;

/* Pair (label, confidence) representing a prediction. */
typedef std::pair<string, float> Prediction;

class Deep_Classifier {
    public:
        Deep_Classifier(const string& model_file,
                   const string& trained_file,
                   const string& mean_file,
                   const string& label_file);

        std::vector<Prediction> Classify(const cv::Mat& img, int N = 5);


    private:
        void SetMean(const string& mean_file);

        std::vector<float> Predict(const cv::Mat& img);

        void WrapInputLayer(std::vector<cv::Mat>* input_channels);

        void Preprocess(const cv::Mat& img,
                        std::vector<cv::Mat>* input_channels);

    private:
        shared_ptr<Net<float> > net_;
        cv::Size input_geometry_;
        int num_channels_;
        cv::Mat mean_;
        std::vector<string> labels_;
};





#endif
