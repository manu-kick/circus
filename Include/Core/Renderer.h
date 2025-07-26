#pragma once
#include <GLFW/glfw3.h>
#include <mujoco/mujoco.h>

namespace spqr {
class Renderer {
public:
    static void init(GLFWwindow* window);
    static void render(GLFWwindow* window);
    static void shutdown();

private:
    static void setupCallbacks(GLFWwindow* window);

    static mjvScene scene;
    static mjvCamera cam;
    static mjvOption opt;
    static mjrContext context;

    // Callbacks 
    static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    static void cursorPosCallback(GLFWwindow* window, double xpos, double ypos);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void resizeCallback(GLFWwindow* window, int width, int height);

    static bool initialized;
};
}
