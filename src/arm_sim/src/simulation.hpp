#pragma once
#include <mujoco/mujoco.h>
#include <GLFW/glfw3.h>
//#include <eigen3> // ?

#include <iostream>

#include <vector>
#include <array>
#include <string>

#include <memory>
#include <thread>
#include <chrono>

#include <std_msgs/Int32.h>

#include "../include/IK.hpp"

using namespace std;

//ik
double targetPos[3] = {0.3, 0.5, 0.3};

//poselist
typedef array<double, 5> pList;

//mujoco and sim stuff
mjModel* m = nullptr;
mjData* d = nullptr;
mjvScene scn;
mjvCamera cam;
mjvOption opt;
mjrContext con;

mjvCamera getCam(){ return cam; }
mjvScene getScene(){ return scn; }
mjData* getData(){ return d; }

//func banyak kepake
void print(string s, bool newl = true){
    cout<<s;
    if (newl) {cout<<endl;}
}

//bot states 
enum botState {
    Teleop, Posed, Animated, InverseK
};
const string stateNames[] = {
    "Teleop", "Posed", "Animated", "InverseK"
};

string STATE = stateNames[botState::Teleop];
string STATENAME = "-";

void setBotState(botState st, string stn){
    STATE = stateNames[st];
    STATENAME = stn;
};