#include "Core/Renderer.h"
#include "Core/Simulator.h"

namespace spqr {

bool Renderer::initialized = false;
mjvScene Renderer::scene;
mjvCamera Renderer::cam;
mjvOption Renderer::opt;
mjrContext Renderer::context;

void Renderer::init(GLFWwindow *window){
    mjv_defaultCamera(&cam);
    mjv_defaultOption(&opt);
    mjv_defaultScene(&scene);
    mjr_defaultContext(&context);
    mjv_makeScene(Simulator::getModel(), &scene, 2000);
    mjr_makeContext(Simulator::getModel(), &context, mjFONTSCALE_150);
    setupCallbacks(window);

    initialized = true;
}

void Renderer::setupCallbacks(GLFWwindow* window) {
    glfwSetKeyCallback(window, keyboardCallback);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPosCallback);
    glfwSetScrollCallback(window, scrollCallback);
    glfwSetFramebufferSizeCallback(window, resizeCallback);
}

void Renderer::render(GLFWwindow* window) {
    if (!initialized) init(window);

    mjv_updateScene(Simulator::getModel(), Simulator::getData(), &opt, nullptr, &cam, mjCAT_ALL, &scene);
    mjrRect viewport = {0, 0, 0, 0};
    glfwGetFramebufferSize(window, &viewport.width, &viewport.height);
    mjr_render(viewport, &scene, &context);
}

void Renderer::shutdown() {
    if (initialized) {
        mjv_freeScene(&scene);
        mjr_freeContext(&context);
        initialized = false;
    }
}

void Renderer::keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        switch (key) {
            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(window, GLFW_TRUE);
                break;
            // TODO: just a placeholder, add real shortcuts 
            default:
                break;
        }
    }
}

void Renderer::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) { }

void Renderer::cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    static double lastX = xpos;
    static double lastY = ypos;

    double dx = xpos - lastX;
    double dy = ypos - lastY;
    lastX = xpos;
    lastY = ypos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        int mods = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_SHIFT) == GLFW_PRESS;

        if (mods) {
            // Shift + drag: rotate camera
            // scale for sensitivity (TODO: move the scale factor somewhere else)
            double scale_x = 0.005;
            double scale_y = 0.005;
            mjv_moveCamera(Simulator::getModel(), mjMOUSE_ROTATE_H, dx * scale_x, 0, &scene, &cam);
            mjv_moveCamera(Simulator::getModel(), mjMOUSE_ROTATE_V, 0, dy * scale_y, &scene, &cam);
        } else {
            // Plain drag: translate camera (pan)
            // scale for sensitivity (TODO: move the scale factor somewhere else)
            double scale_x = 0.005;
            double scale_y = 0.005;
            mjv_moveCamera(Simulator::getModel(), mjMOUSE_MOVE_H, dx * scale_x, 0, &scene, &cam);
            mjv_moveCamera(Simulator::getModel(), mjMOUSE_MOVE_V, 0, dy * scale_y, &scene, &cam);
        }
    }
 }

void Renderer::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) { 
    // scale for sensitivity (TODO: move the scale factor somewhere else)
    double dy = -0.05 * yoffset;
    mjv_moveCamera(Simulator::getModel(), mjMOUSE_ZOOM, 0, dy, &scene, &cam);
}

void Renderer::resizeCallback(GLFWwindow* window, int width, int height) {
    mjrRect viewport = {0, 0, width, height};
    mjr_render(viewport, &scene, &context);
}

}
