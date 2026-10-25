//main func, put arm classes instantiated in arm.cpp here
#include "simulation.hpp"
#include "ros/ros.h"

#include <cstdio>
#include <algorithm>

#include "../include/controls.hpp"
#include "../include/callbacks.hpp"
#include "../initialize/poses.hpp"

#include "arm_sim/data.h"

int main(int argc, char **argv){
    char error[1000] = "Couldn't load model";
    
    //gmn yak biar gk hardcoded gini (path is based on whre final build exe is!!!)
    m = mj_loadXML("/home/kalelgms/Desktop/Projects/third_abinara_project/src/arm_sim/robot/arm.xml", nullptr, error, sizeof(error));
    if (!m) {
        fprintf(stderr, "%s\n", error); return 1;
    }
    d = mj_makeData(m);

    //error
    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit()) {
        fprintf(stderr, "glfwInit() gagal\n");
        return 1;
    }

    //biar nyala windownya
    GLFWwindow* window = glfwCreateWindow(900, 1200, "arm sim", nullptr, nullptr);
    if (!window){
        fprintf(stderr, "glfwCreateWindow() gagal\n");
        glfwTerminate(); return 1;
    }
    glfwMakeContextCurrent(window);

    //callbacks
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetScrollCallback(window, scroll_callback);
    glfwSetKeyCallback(window, keyboard_callback);
    
    //scene setup
    mjv_defaultOption(&opt);
    mjv_defaultScene(&scn);
    mjr_defaultContext(&con);
    mjv_makeScene(m, &scn, 2000);
    mjr_makeContext(m, &con, mjFONTSCALE_100);

    //cam setup
    mjv_defaultCamera(&cam);
    cam.lookat[2] = 0.35; // cam origin up sdikit
    cam.orthographic = 1;
    cam.distance = 1.8;
    cam.azimuth = 45;
    cam.elevation = -25;

    //create poses
    initializePoses();

    //starting pose
    playPose(*PoseList[0]); // hrs inget index apa utk play pose tertentu, 0 itu startpos

    //ROS!!!
    ros::init(argc, argv, "botdata_publish");
    ros::NodeHandle bothandle;
    ros::Publisher pub = bothandle.advertise<arm_sim::data>("botdata_topic", 10);

    double lastPublishTime = glfwGetTime();
    const double interval = 0.05;

    //sim loop
    while (!glfwWindowShouldClose(window)){
        if (currentKey != 0){
            keyHeld_update(d);
        }

        //ROS PUBLISH
        //biar bisa run dalem loop, tapi dgn interval sendiri
        double currentTime = glfwGetTime();
        if (currentTime - lastPublishTime >= interval){
            arm_sim::data bdata;
                bdata.botname = "za hando";
                bdata.state = STATE;
                bdata.statename = STATENAME;

                bdata.torso_yaw      = d->ctrl[0];
                bdata.shoulder_pitch = d->ctrl[1];
                bdata.upperarm_pitch = d->ctrl[2];
                bdata.forearm_pitch  = d->ctrl[3];
                bdata.hand_yaw       = d->ctrl[4];

                pub.publish(bdata);
                ROS_INFO("Sent bot state and position values");

            lastPublishTime = currentTime;
        }

        mj_step(m, d);

        mjrRect viewport = {0, 0, 0, 0};
        glfwGetFramebufferSize(window, &viewport.width, &viewport.height);

        mjv_updateScene(m, d, &opt, nullptr, &cam, mjCAT_ALL, &scn);
        mjr_render(viewport, &scn, &con);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    //beres beres
    mjv_freeScene(&scn);
    mjr_freeContext(&con); 
    mj_deleteData(d);
    mj_deleteModel(m);
    glfwTerminate();

    return 0;
}