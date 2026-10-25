#pragma once
#include <mujoco/mujoco.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include <vector>
#include <array>
#include <string>

#include <memory>
#include <thread>
#include <chrono>

using namespace std;

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

void wait(int s){
    this_thread::sleep_for(chrono::milliseconds(s));
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

//ubah jd ros topic
// void displayBotData(){
//     print("_____________________________________________________________");
//     print("<za hando>");
//     print("Current State   : ", false); print(STATE);
//     print("State Name      : ", false); print(STATENAME);

//     print("<Joint Values>");
//     print("Torso Yaw       : ", false); printf("%.2f \n", d->ctrl[0]);
//     print("Shoulder Pitch  : ", false); printf("%.2f \n", d->ctrl[1]);
//     print("Upper arm Pitch : ", false); printf("%.2f \n", d->ctrl[2]);
//     print("Forearm Pitch   : ", false); printf("%.2f \n", d->ctrl[3]);
//     print("Hand Yaw        : ", false); printf("%.2f \n", d->ctrl[4]);
// };