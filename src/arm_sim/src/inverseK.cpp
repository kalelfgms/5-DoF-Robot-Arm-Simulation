// //for tracking bot data, replaces displayBotData()
// #include "simulation.hpp"

// //msg and srv
// #include "ros/ros.h"
// #include "arm_sim/data.h"
// #include "arm_sim/IK.h"
// #include "arm_sim/IK_target.h"

// #include <sstream>
// #include <iomanip>

// //gets current bot position and rotation from sim.cpp (listen to inverseK_topic)
// //  why not botdata_topic? cuz itu ngirim info bot lebih lambat daripada simulation, IK butuh yang gk delay
// //gets target pos and rot from sim.cpp (service biar cuman send sekali) 
// //  (or maybe masukin inverseK_topic klo butuh error to target)

// //after ik solve, send pos and rot data back to sim.cpp and setpose (send service to sim.cpp)

// void solveIK(const arm_sim::data::ConstPtr& IKdata){ // IK finder


//     ROS_INFO("IK iteration solved, moving");
// }

// int main(int argc, char **argv){
//     //ros init
//     ros::init(argc, argv, "botdata_subscriber");
//     ros::NodeHandle nh; 

//     //pub
//     ros::Publisher pub_IK = nh.advertise<arm_sim::data>("IK_topic", 10); // send hasil IK
//     //sub
//     ros::Subscriber sub = nh.subscribe("currentPos_topic", 10, solveIK); // current pos

//     ros::spin();



//     return 0;   
// }