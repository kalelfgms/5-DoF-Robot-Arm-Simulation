//loop kirim data to "animate" bot
#include "simulation.hpp"
#include "ros/ros.h"
#include "arm_sim/triangle.h"
#include "arm_sim/triangle_point.h"

bool running = true;

bool end_callback(arm_sim::triangle::Request &req, arm_sim::triangle::Response &res) {
    if (req.stop) {
        running = false;
        res.success = true;
        ROS_INFO("stopping");
    } else {
        res.success = false;
    }
    return true;
}

int main(int argc, char **argv){
    ros::init(argc, argv, "trianglePoint_topic");
    ros::NodeHandle nh;
 
    // triangle_point.msg
    ros::Publisher point_pub = nh.advertise<std_msgs::Int32>("triangle_point", 10);
    // triangle.srv
    ros::ServiceServer stop_service = nh.advertiseService("triangle_stop", end_callback);

    ros::Rate rate(0.5); // 0.5 = tiap 2s
    int current_point = 1;

    ROS_INFO("triangle animating");
    while (ros::ok() && running) {
       
        arm_sim::triangle_point tri;
        tri.point = current_point;
        point_pub.publish(tri);
        ROS_INFO("current point: %d", current_point);

        current_point++;
        if (current_point > 3) {
            current_point = 1;
        }

        ros::spinOnce();
        rate.sleep();
    }

    ROS_INFO("triangle shutdown");
    return 0;
}