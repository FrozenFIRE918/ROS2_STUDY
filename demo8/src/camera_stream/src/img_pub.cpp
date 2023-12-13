#include<chrono>
#include<functional>
#include<memory>
#include<string>

#include "rclcpp/rclcpp.hpp"
#include "cv_bridge/cv_bridge.h"
#include "image_transport/image_transport.hpp"
#include "opencv2/core/mat.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/opencv.hpp"

using namespace rclcpp;
using namespace cv;

int main(int argc, char ** argv)
{
    init(argc,argv);
    NodeOptions options;

    VideoCapture capture;
    Mat img;

    capture.open("/dev/video0",CAP_V4L2);

    rclcpp::Node::SharedPtr node = rclcpp::Node::make_shared("img_stream_publisher",options); 
    image_transport::ImageTransport it(node);
    image_transport::Publisher pub = it.advertise("cam_img",1);


    std_msgs::msg::Header header;
    WallRate loop_rate(30);

    while (rclcpp::ok())
    {
        /* code */
        capture >> img;

        sensor_msgs::msg::Image::SharedPtr msg = cv_bridge::CvImage(header,"bgr8",img).toImageMsg();

        pub.publish(msg);

        rclcpp::spin_some(node);
        loop_rate.sleep();

    }
    
}
