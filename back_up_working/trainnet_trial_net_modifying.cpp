#include "trainnet.h"



TrainNet::TrainNet(const string& model_file,
                       const string& trained_file,
                       const string& mean_file,
                       const string& label_file) {

    Caffe::set_mode(Caffe::GPU);

    /* Load the network. */
    net_.reset(new Net<float>(model_file, TEST)); //Load the structure
    net_->CopyTrainedLayersFrom(trained_file); //Load the weight

    Blob<float>* input_layer = net_->input_blobs()[0]; //get first pointer of input layer
    num_channels_ = input_layer->channels();
    input_geometry_ = cv::Size(input_layer->width(), input_layer->height());
    //get information about inpuy_layer width/height from model_file


    /* Load the binaryproto mean file. */
    SetMean(mean_file);

    /* Load labels. */
    std::ifstream labels(label_file.c_str());
    string line;
    while (std::getline(labels, line))
        labels_.push_back(string(line));

    Blob<float>* output_layer = net_->output_blobs()[0];
}

QString TrainNet::init_fc_layer()
{
    //new_net_.reset(new Net<float>("/home/kiru/catkin_ws/src/o_learning/data/VGG_CNN_S_finetune.prototxt", TEST));
    //new_net_->CopyTrainedLayersFrom("/home/kiru/catkin_ws/src/o_learning/data/VGG_CNN_S.caffemodel");
/*
    net_.swap(new_net_);
    Blob<float>* input_layer = net_->input_blobs()[0]; //get first pointer of input layer
    num_channels_ = input_layer->channels();
    input_geometry_ = cv::Size(input_layer->width(), input_layer->height());*/


    //want to do : [put the layerparameter in to new layer] and Setup
    shared_ptr<Layer<float> > fc_layer = net_->layer_by_name("fc8");

    LayerParameter layer_ip_param= fc_layer->layer_param();
    layer_ip_param.mutable_inner_product_param()->set_num_output(20);
    layer_ip_param.mutable_inner_product_param()->mutable_weight_filler()->set_type("xavier");
    layer_ip_param.mutable_name()->clear();
    layer_ip_param.mutable_name()->append("fc8_new");
    //shared_ptr<Layer<float> > new_layer(new InnerProductLayer<float>(layer_ip_param));
    Layer<float>* new_layer(new InnerProductLayer<float>(layer_ip_param));

    int size_b, size_t;
    size_b=net_->bottom_vecs().size();
    size_t=net_->top_vecs().size();
    new_layer->SetUp(net_->bottom_vecs().at(size_b-2),net_->top_vecs().at(size_t-2));

    QString qs;
    qs="NewLayer:" + QString::fromStdString(new_layer->layer_param().name());
    //net_->push_back_layer(new_layer);
    //net_->pop_back_layer();

    this->train(20);
    vector<string> k=net_->layer_names();
    net_->Reshape();
    net_->Update(); //how to change the layer of the net_??? -> if I'can, give up

    qs=qs+ "   Last :"+ QString::fromStdString(k.at(k.size()-2));

    //for(int i=0; i<k.size();i++) qs = qs +QString("%1:").arg(i)+ QString::fromStdString(k.at(i))+"\n";

    return qs;


}
void TrainNet::train(int iter)
{
    //train net iteratively using net_ class
    //using imagenet data
    //print out loss per each 100 iteration
    //first trial : using for statement

    /*
     *         cv::Mat img = cv_ptr->image;
            std::vector<Prediction> predictions = rost->trainer->Classify(img);
   */

    Blob<float>* input_layer = net_->input_blobs()[0];
    input_layer->Reshape(1, num_channels_,
                         input_geometry_.height, input_geometry_.width);
    /* Forward dimension change to all layers. */
    net_->Reshape();

    //msg = const sensor_msgs::ImageConstPtr& msg
    /*
    cv_bridge::CvImagePtr cv_ptr = cv_bridge::toCvCopy(msg, "bgr8");
    cv::Mat img = cv_ptr->image;


    std::vector<cv::Mat> input_channels;
    WrapInputLayer(&input_channels);
    Preprocess(img, &input_channels);

    net_->ForwardPrefilled();
    */
    /* Copy the output layer to a std::vector */
    /*
    Blob<float>* output_layer = net_->output_blobs()[0];
    const float* begin = output_layer->cpu_data();
    const float* end = begin + output_layer->channels();
    return std::vector<float>(begin, end);




    for(int i=0;i<iter;i++)
    {
        net_->ForwardBackward()


    }*/



    //second trial : using solver class





}

static bool PairCompare(const std::pair<float, int>& lhs,
                        const std::pair<float, int>& rhs) {
    return lhs.first > rhs.first;
}

/* Return the indices of the top N values of vector v. */
static std::vector<int> Argmax(const std::vector<float>& v, int N) {
    std::vector<std::pair<float, int> > pairs;
    for (size_t i = 0; i < v.size(); ++i)
        pairs.push_back(std::make_pair(v[i], i));
    std::partial_sort(pairs.begin(), pairs.begin() + N, pairs.end(), PairCompare);

    std::vector<int> result;
    for (int i = 0; i < N; ++i)
        result.push_back(pairs[i].second);
    return result;
}

/* Return the top N predictions. */
std::vector<Prediction> TrainNet::Classify(const cv::Mat& img, int N) {
    std::vector<float> output = Predict(img);

    std::vector<int> maxN = Argmax(output, N);
    std::vector<Prediction> predictions;
    for (int i = 0; i < N; ++i) {
        int idx = maxN[i];
        predictions.push_back(std::make_pair(labels_[idx], output[idx]));
    }

    return predictions;
}

/* Load the mean file in binaryproto format. */
void TrainNet::SetMean(const string& mean_file) {
    BlobProto blob_proto;
    ReadProtoFromBinaryFileOrDie(mean_file.c_str(), &blob_proto);
                                //const char*, message blob

    /* Convert from BlobProto to Blob<float> */
    Blob<float> mean_blob;
    mean_blob.FromProto(blob_proto);

    /* The format of the mean file is planar 32-bit float BGR or grayscale. */
    std::vector<cv::Mat> channels;
    float* data = mean_blob.mutable_cpu_data();
    for (int i = 0; i < num_channels_; ++i) {
        /* Extract an individual channel. */
        cv::Mat channel(mean_blob.height(), mean_blob.width(), CV_32FC1, data);
        channels.push_back(channel);
        data += mean_blob.height() * mean_blob.width();
    }

    /* Merge the separate channels into a single image. */
    cv::Mat mean;
    cv::merge(channels, mean);
    /* Compute the global mean pixel value and create a mean image
     * filled with this value. */
    cv::Scalar channel_mean = cv::mean(mean);

    mean_ = cv::Mat(input_geometry_, mean.type(), channel_mean);

}

std::vector<float> TrainNet::Predict(const cv::Mat& img) {
    Blob<float>* input_layer = net_->input_blobs()[0];
    input_layer->Reshape(1, num_channels_,
                         input_geometry_.height, input_geometry_.width);
    /* Forward dimension change to all layers. */
    net_->Reshape();

    std::vector<cv::Mat> input_channels;
    WrapInputLayer(&input_channels);

    Preprocess(img, &input_channels);

    net_->ForwardPrefilled();

    /* Copy the output layer to a std::vector */
    Blob<float>* output_layer = net_->output_blobs()[0];
    const float* begin = output_layer->cpu_data();
    const float* end = begin + output_layer->channels();
    return std::vector<float>(begin, end);
}

/* Wrap the input layer of the network in separate cv::Mat objects
 * (one per channel). This way we save one memcpy operation and we
 * don't need to rely on cudaMemcpy2D. The last preprocessing
 * operation will write the separate channels directly to the input
 * layer. */
void TrainNet::WrapInputLayer(std::vector<cv::Mat>* input_channels) {
    Blob<float>* input_layer = net_->input_blobs()[0];

    int width = input_layer->width();
    int height = input_layer->height();
    float* input_data = input_layer->mutable_cpu_data();
    for (int i = 0; i < input_layer->channels(); ++i) {
        cv::Mat channel(height, width, CV_32FC1, input_data);
        input_channels->push_back(channel);
        input_data += width * height;
    }
}

void TrainNet::Preprocess(const cv::Mat& img,
                            std::vector<cv::Mat>* input_channels) {
    /* Convert the input image to the input image format of the network. */
    cv::Mat sample;
    if (img.channels() == 3 && num_channels_ == 1)
        cv::cvtColor(img, sample, CV_BGR2GRAY);
    else if (img.channels() == 4 && num_channels_ == 1)
        cv::cvtColor(img, sample, CV_BGRA2GRAY);
    else if (img.channels() == 4 && num_channels_ == 3)
        cv::cvtColor(img, sample, CV_BGRA2BGR);
    else if (img.channels() == 1 && num_channels_ == 3)
        cv::cvtColor(img, sample, CV_GRAY2BGR);
    else
        sample = img;

    cv::Mat sample_resized;
    if (sample.size() != input_geometry_)
        cv::resize(sample, sample_resized, input_geometry_);
    else
        sample_resized = sample;

    cv::Mat sample_float;
    if (num_channels_ == 3)
        sample_resized.convertTo(sample_float, CV_32FC3);
    else
        sample_resized.convertTo(sample_float, CV_32FC1);

    cv::Mat sample_normalized;
    cv::subtract(sample_float, mean_, sample_normalized);

    /* This operation will write the separate BGR planes directly to the
     * input layer of the network because it is wrapped by the cv::Mat
     * objects in input_channels. */
    cv::split(sample_normalized, *input_channels);
}
