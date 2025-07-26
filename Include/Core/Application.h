#pragma once
#include <GLFW/glfw3.h>

namespace spqr {

class Application {
public:
    Application();
    ~Application();
    void run();
private:
    void initWindow();
    void initMuJoCo();
    void mainLoop();
    void cleanup();
    GLFWwindow* window;
};

}
