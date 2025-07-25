#pragma once

#include <iostream>
#include <Eigen/Dense>
#include <GLFW/glfw3.h>
#include <mujoco/mujoco.h>

using namespace Eigen;

constexpr int HORIZON = 150;

class MujocoSim {
    public:
        MujocoSim();

        char errorBuffer[1000] = "Error in Mujoco Simulation";
        mjModel* mujModel;
        mjData* mujData;

        GLFWwindow* window;
        mjvScene scene;
        mjvCamera camera;
        mjvOption option;
        mjrContext context;
        mjvGeom comMarker;

        bool buttonLeft;
        bool buttonMiddle;
        bool buttonRight;
        double lastX;
        double lastY;

        bool pause;
        bool draggingRobot;

        void setMujocoState(VectorXd pinState);
        void setMujocoTorque(VectorXd pinTorque);

        static void mouseButtonCallback(GLFWwindow* window, int button, int act, int mods);
        static void mouseMoveCallback(GLFWwindow* window, int xPos, int yPos);
        static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);
        static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
        static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
};
