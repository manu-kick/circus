#include "Include/Simulator.h"
#include "mujoco/mjvisualize.h"

MujocoSim::MujocoSim() {
    mujModel = mj_loadXML("../Resources/scene.xml", 0, errorBuffer, 1000);
    if (!mujModel) {
        std::cerr << "Error during the load of the XML file: " << errorBuffer << std::endl;
    }
    mujData = mj_makeData(mujModel);

    buttonLeft = false;
    buttonRight = false;
    lastX = 0.0;
    lastY = 0.0;

    pause = false;

    glfwInit();
    window = glfwCreateWindow(1200, 900, "Booster Simulation", NULL, NULL);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    mjv_defaultCamera(&camera);
    mjv_defaultOption(&option);
    mjr_defaultContext(&context);
    mjv_defaultScene(&scene);

    // What I am interested to visualize aka the Contact Forces
    option.flags[mjVIS_CONTACTFORCE] = true;
    option.flags[mjVIS_CONTACTFORCE] = true;
    option.flags[mjVIS_TRANSPARENT] = true;
    // option.frame = mjFRAME_SITE; // comment if you don't want to see the RFs
    camera.distance = 4.0;

    mjv_makeScene(mujModel, &scene, 1000);
    mjr_makeContext(mujModel, &context, mjFONTSCALE_100);

    // Mouse callbacks
    glfwSetWindowUserPointer(window, this);
    glfwSetMouseButtonCallback(window, MujocoSim::mouseButtonCallback);
    glfwSetScrollCallback(window, MujocoSim::scrollCallback);
    glfwSetCursorPosCallback(window, MujocoSim::cursorPositionCallback);
    glfwSetKeyCallback(window, MujocoSim::keyboardCallback);

    // com marker
    mjv_initGeom(&comMarker, mjGEOM_SPHERE, nullptr, nullptr, nullptr, nullptr);
    comMarker.size[0] = 0.03;
    comMarker.size[1] = 0.03;
    comMarker.size[2] = 0.03;

    comMarker.rgba[0] = 1.0f;
    comMarker.rgba[1] = 0.0f;
    comMarker.rgba[2] = 0.0f;
    comMarker.rgba[3] = 1.0f;
}

void MujocoSim::setMujocoState(VectorXd pinState) {
    for (int i = 0; i < mujModel->nq; i++) {
        mujData->qpos[i] = pinState[i];
    }

    for (int i = 0; i < mujModel->nv; i++) {
        mujData->qvel[i] = 0.0;
        mujData->qacc[i] = 0.0;
    }
}

void MujocoSim::setMujocoTorque(VectorXd pinTorque) {
    int skipFloatingBase = 6;
    for (int i = 0; i < mujModel->nu; i++) {
        mujData->ctrl[i] = pinTorque[i+skipFloatingBase];
    }
}

void MujocoSim::mouseButtonCallback(GLFWwindow* window, int button, int act, int mods) {
    auto* sim = static_cast<MujocoSim*>(glfwGetWindowUserPointer(window));
    if (!sim) return;

    if (button == GLFW_MOUSE_BUTTON_LEFT)
        sim->buttonLeft = (act == GLFW_PRESS);
    if (button == GLFW_MOUSE_BUTTON_RIGHT)
        sim->buttonRight = (act == GLFW_PRESS);
}

void MujocoSim::mouseMoveCallback(GLFWwindow* window, int xPos, int yPos) {
    auto* sim = static_cast<MujocoSim*>(glfwGetWindowUserPointer(window));
    if (!sim) return;

    if (!sim->buttonLeft && !sim->buttonRight)
        return;
    int width, height;
    glfwGetWindowSize(window, &width, &height);

    double dx = (xPos - sim->lastX) / height;
    double dy = (yPos - sim->lastY) / height;
    sim->lastX = xPos;
    sim->lastY = yPos;

    // MuJoCo camera manipulation: left = rotate, right = move
    int action = sim->buttonRight ? mjMOUSE_MOVE_H : mjMOUSE_ROTATE_H;
    mjv_moveCamera(sim->mujModel, action, dx, dy, &sim->scene, &sim->camera);
}

void MujocoSim::scrollCallback(GLFWwindow* window, double xOffset, double yOffset) {
    auto* sim = static_cast<MujocoSim*>(glfwGetWindowUserPointer(window));
    if (!sim) return;

    mjv_moveCamera(sim->mujModel, mjMOUSE_ZOOM, 0, -0.05*yOffset, &sim->scene, &sim->camera);
}

void MujocoSim::cursorPositionCallback(GLFWwindow* window, double xPos, double yPos) {
    auto* sim = static_cast<MujocoSim*>(glfwGetWindowUserPointer(window));
    if (!sim) return;

    if (sim->buttonLeft || sim->buttonRight)
        mouseMoveCallback(window, xPos, yPos);
    else {
        sim->lastX = xPos;
        sim->lastY = yPos;
    }
}

void MujocoSim::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    auto* sim = static_cast<MujocoSim*>(glfwGetWindowUserPointer(window));
    if (!sim) return;

    if (action == GLFW_PRESS) {
        if (key == GLFW_KEY_SPACE) {
            sim->pause = !sim->pause;
        }
    }
}
