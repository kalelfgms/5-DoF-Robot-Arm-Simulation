//functions for controlling arms
#pragma once
#include "../src/simulation.hpp"

void moveJoint(int joint, double dir){
    //dir: left -1, right 1
    mjData* data = getData();
    const double step = dir*1; 
    double ctrlMin, ctrlMax, ctrlCurrent;
    ctrlMin = m->actuator_ctrlrange[2*joint];
    ctrlMax = m->actuator_ctrlrange[2*joint+1];
    
    //set state an statename
    setBotState(botState::Teleop, "-");

    const double vmax = 6; // max speed, biar gk pencet sekali tbtb sampe ujung
    d->ctrl[joint] = clamp( // batesin
        d->ctrl[joint] += step * vmax * m->opt.timestep,
        ctrlMin, ctrlMax);
}

//POSE
class Pose {
    private:
        string name;
        pList orients = {0, 0, 0, 0, 0};
        int key = 0;

    public:
        Pose(string n, int k, pList ori): name(n), key(k), orients(ori) {};

        pList getPose(){ return orients; };

        int getKey(){ return key; };
        string getName(){ return name; };
};

vector<unique_ptr<Pose>> PoseList;

void playPose(Pose pose){
    mjData* data = getData();
    pList orients = pose.getPose();

    //set state and statename
    setBotState(botState::Posed, pose.getName());

    for (int i = 0; i < 5; i++){
        data->ctrl[i] = orients[i];
    };
};

void setPose(Pose pose){
    mjData* data = getData();
    pList orients = pose.getPose();

    for (int i = 0; i < 5; i++){
        data->ctrl[i] = orients[i];
    };
};

//IK

