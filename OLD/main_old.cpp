/*
#include "Include/Simulator.h"
#include <iostream>
#include <iomanip>

int main() {
    MujocoSim sim = MujocoSim();

    std::stringstream comPositionPrintNames;
    std::stringstream jointPrintNames;

    comPositionPrintNames << "X: \n"<< "Y: \n" << "Z: ";

    for (int i = 0; i < sim.mujModel->njnt; i++) {
        const char* name = mj_id2name(sim.mujModel, mjOBJ_JOINT, i);
        jointPrintNames << name << ": \n";
    }

    std::string comPositionNames = comPositionPrintNames.str();
    std::string jointNames = jointPrintNames.str();
    jointNames.pop_back();

    std::stringstream comPositionPrint;
    std::stringstream jointPrint;
    comPositionPrint << std::fixed << std::setprecision(5);
    jointPrint << std::fixed << std::setprecision(5);

    int simCounter = 0;

    while(!glfwWindowShouldClose(sim.window)) {
      
        for (int i = 0; i < 3; i++) {
            comPositionPrint << sim.mujData->subtree_com[i] << "\n";
            sim.comMarker.pos[i] = sim.mujData->subtree_com[i];
        }

        for (int i = 0; i < sim.mujModel->njnt; i++) {
            int jointPosAddr = sim.mujModel->jnt_qposadr[i];
            jointPrint << sim.mujData->qpos[jointPosAddr] << "\n";
        }


        if (!sim.pause) {
            mj_step(sim.mujModel, sim.mujData);
            simCounter++;
        }

        mjrRect viewport = {0, 0, 0, 0};
        glfwGetFramebufferSize(sim.window, &viewport.width, &viewport.height);

        mjv_updateScene(sim.mujModel, sim.mujData, &sim.option, nullptr, &sim.camera, mjCAT_ALL, &sim.scene);
        
        if (sim.scene.ngeom < sim.scene.maxgeom)
            sim.scene.geoms[sim.scene.ngeom++] = sim.comMarker;

        mjr_render(viewport, &sim.scene, &sim.context);
        
        std::string comPosition = comPositionPrint.str();
        std::string joint = jointPrint.str();
        comPosition.pop_back();
        joint.pop_back();
        
        mjr_overlay(mjFONT_BIG, mjGRID_TOPLEFT, viewport, comPositionNames.c_str(), comPosition.c_str(), &sim.context);
        mjr_overlay(mjFONT_BIG, mjGRID_TOPRIGHT, viewport, jointNames.c_str(), joint.c_str(), &sim.context);

        comPositionPrint.str("");
        comPositionPrint.clear();
        jointPrint.str("");
        jointPrint.clear();

        glfwSwapBuffers(sim.window);
        glfwPollEvents();
        if (simCounter > HORIZON) {
            glfwSetWindowShouldClose(sim.window, 1);
        }
    }

    mj_deleteData(sim.mujData);
    mj_deleteModel(sim.mujModel);
    mjv_freeScene(&sim.scene);
    mjr_freeContext(&sim.context);
    glfwDestroyWindow(sim.window);
    glfwTerminate();
}
*/