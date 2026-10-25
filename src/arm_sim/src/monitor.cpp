//for tracking bot data, replaces displayBotData()
#include "simulation.hpp"
#include "ros/ros.h"
#include "arm_sim/data.h"

#include <sstream>
#include <iomanip>

void data_callback(const arm_sim::data::ConstPtr& sent){
    array<double, 5> doubpos = {
        sent->torso_yaw,
        sent->shoulder_pitch,
        sent->upperarm_pitch,
        sent->forearm_pitch,
        sent->hand_yaw
    };

    //setup pos values
    array<string, 5> strpos = {};
    for (int i = 0; i < 5; i++){
        ostringstream pos;
        pos << fixed << setprecision(2) << doubpos[i];
        strpos[i] = pos.str();
    }

    //string jadiin 1 biar enak diliatz
    string final = 
        "_____________________________________________________________\n" +
        string("<") + sent->botname + ">\n" +
        "Current State   : " + sent->state + "\n" +
        "State Name      : " + sent->statename + "\n\n" +
        "<Joint Values>\n" +
        "Torso Yaw       : " + strpos[0] + "\n" +
        "Shoulder Pitch  : " + strpos[1] + "\n" +
        "Upper arm Pitch : " + strpos[2] + "\n" +
        "Forearm Pitch   : " + strpos[3] + "\n" +
        "Hand Yaw        : " + strpos[4] + "\n";

    ROS_INFO("%s", final.c_str());
}

int main(int argc, char **argv){
    ros::init(argc, argv, "botdata_subscriber");
    ros::NodeHandle nh; 

    //listen up
    ros::Subscriber sub = nh.subscribe("botdata_topic", 10, data_callback);
    ros::spin();

    return 0;   
}