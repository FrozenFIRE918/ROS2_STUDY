#include "cv_bridge/cv_bridge.h"
#include "image_transport/image_transport.hpp"
#include "opencv2/highgui.hpp"
#include "rclcpp/rclcpp.hpp"
#include "rclcpp/logging.hpp"
#include "sensor_msgs/msg/image.hpp"

void Img_ReceiveClpt_CallBack(const sensor_msgs::msg::Image::ConstSharedPtr & msg)
{
    cv::Mat img;
    try
    {
        img = cv_bridge::toCvShare(msg,"bgr8")->image;
        cv::Point circle_center = cv::Point(600,440);
        cv::Scalar color = cv::Scalar(233,250,173);
        cv::circle(img,circle_center,30,color,cv::FILLED);
        cv::imshow("view",img);

    }
    catch(const cv_bridge::Exception & e)
    {
        std::cerr << e.what() << '\n';
    }
}

int main(int argc, char ** argv)
{
    rclcpp::init(argc,argv);
    rclcpp::NodeOptions options;
    rclcpp::Node::SharedPtr node = rclcpp::Node::make_shared("img_stream_subscriber",options);

    cv::namedWindow("view");
    cv::startWindowThread();

    image_transport::ImageTransport it(node);
    image_transport::Subscriber sub = it.subscribe("cam_img",1,Img_ReceiveClpt_CallBack);

    rclcpp::spin(node);
    cv::destroyWindow("view");
}