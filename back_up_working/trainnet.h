#ifndef TRAINNET_H
#define TRAINNET_H


#include "Classifier.h"
#include <QString>
#include <caffe/caffe.hpp>
#include "caffe/blob.hpp"
#include "caffe/common.hpp"
#include "caffe/layers/inner_product_layer.hpp"

typedef float Dtype;

class TrainNet
{

public:
    TrainNet(const string& model_file,
               const string& trained_file,
               const string& mean_file,
               const string& label_file);

    std::vector<Prediction> Classify(const cv::Mat& img, int N = 5);
    QString init_fc_layer();
    void train(int iter);


private:
    void SetMean(const string& mean_file);

    std::vector<float> Predict(const cv::Mat& img);

    void WrapInputLayer(std::vector<cv::Mat>* input_channels);

    void Preprocess(const cv::Mat& img,
                    std::vector<cv::Mat>* input_channels);

private:
    shared_ptr<Net<float> > net_;
    shared_ptr<Net<float> > new_net_;

    cv::Size input_geometry_;
    int num_channels_;
    cv::Mat mean_;
    std::vector<string> labels_;

};

#endif // TRAINNET_H
