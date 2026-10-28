#pragma once
#include <mujoco/mujoco.h>

#include <algorithm>
#include <vector>

using namespace std;

inline bool solveIK(const mjModel* m, const mjData* d, const char* siteName,
                        const double target[3], double* ctrlOut,
                        int maxIter = 300, double tolerance = 1e-3, double lambda = 0.05){
    int site_id = mj_name2id(m, mjOBJ_SITE, siteName); // posisi dlm hand yg jadi tumpuan buat ke target
    if (site_id < 0) return false;

    const int nv = m->nv; // kecepatanz, brp banyak DoF

    //buat iterasi bikin copy mjmodel dlu
    mjData* temp = mj_makeData(m);
    mju_copy(temp->qpos, d->qpos, m->nq); // copy pose sekarang

    vector<mjtNum> jac(3 * nv), dq(nv);
    mjtNum err[3], y[3], A[9];

    for (int it = 0; it < maxIter; it++) {
        mj_kinematics(m, temp); // FK
        mj_comPos(m, temp); // pusat massa 
        mju_sub3(err, target, temp->site_xpos + 3 * site_id);
        if (mju_norm3(err) < tolerance) break;             // close enough

        //jacobian utk site
        mj_jacSite(m, temp, jac.data(), nullptr, site_id);

        // A = ((J*J^T) + (lambda^2)*I), dq = (J^T)*(A^-1)*E
        mju_mulMatMatT(A, jac.data(), jac.data(), 3, nv, 3);
        for (int k = 0; k < 3; k++) A[4 * k] += lambda * lambda;
        mju_cholFactor(A, 3, 1e-12);
        mju_cholSolve(y, A, err, 3);
        mju_mulMatTVec(dq.data(), jac.data(), y, 3, nv);

        for (int j = 0; j < nv; j++) {               // hinge joints: qpos index == dof index
            double q = temp->qpos[j] + dq[j];
            if (m->jnt_limited[j])
                q = clamp(q, (double)m->jnt_range[2 * j], (double)m->jnt_range[2 * j + 1]);
            temp->qpos[j] = q;
        }
    }

    // dist final to target
    mj_kinematics(m, temp);
    mju_sub3(err, target, temp->site_xpos + 3 * site_id);
    double finalError = mju_norm3(err);

    // joint angles -> actuator targets (clamped to each actuator's ctrlrange)
    for (int i = 0; i < m->nu; i++) {
        int jid = m->actuator_trnid[2 * i]; // id joint
        double q = temp->qpos[m->jnt_qposadr[jid]]; // new rotation value

        double ctrlMin, ctrlMax, ctrlCurrent;
        ctrlMin = m->actuator_ctrlrange[2 * i];
        ctrlMax = m->actuator_ctrlrange[2 * i + 1];
        
        d->ctrl[i] = clamp(q, ctrlMin, ctrlMax); // move

        ctrlOut[i] = q;
    }

    mj_deleteData(temp); // beres beresss
    return (finalError < tolerance * 2.0); //berhasil or nah
}