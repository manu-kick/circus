#include "Core/Application.h"
#include "Core/Simulator.h"
#include "Core/Renderer.h"
#include <mujoco/mujoco.h>

namespace spqr {

Application::Application() : window(nullptr) {}
Application::~Application() { cleanup(); }

void Application::run() {
    initWindow();
    initMuJoCo();
    mainLoop();
    cleanup();
}

void Application::initWindow() {
    glfwInit();
    // TODO: What should we set for width and height? A standard value? Full screen? TBD  
    window = glfwCreateWindow(1280, 720, "Circus Simulator", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
}

void Application::initMuJoCo() {
    // TODO: Hardcoded for now, let the user select it with a simple file dialog. 
    Simulator::init(std::string(PROJECT_ROOT) + "/Resources/scene.xml");
}

void Application::mainLoop() {
    while (!glfwWindowShouldClose(window)) {
        Simulator::step();
        Renderer::render(window);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Application::cleanup() {
    Simulator::shutdown();
    Renderer::shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}

}
