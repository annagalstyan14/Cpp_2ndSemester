#include "BlackHole.h"
#include "Space.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cmath>
#include "stb_image.h"
#include <filesystem>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Global variables
float lastX = 400, lastY = 300;
bool firstMouse = true;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
Space* space;

// Camera parameters
float yaw = -90.0f, pitch = 0.0f;
float fov = 45.0f;

float cameraRadius = 5.0f;
float cameraAngle = 0.0f;

// Callbacks
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    space->processMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset) {
    space->processMouseScroll(yoffset);
}

void processInput(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main() {
    // Init GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Black Hole Simulation", NULL, NULL);
    if (window == NULL) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // Tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGL((GLADloadfunc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set viewport
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Black hole setup
    BlackHole bh(1.989e30, 0, 0, 0);
    std::vector<Particle> particles = {
        {1e9, 0, 0, 0, 1e6, 0} // Example particle
    };
    int step = 0;
    const int maxSteps = 1000;

    // Get the absolute paths for shader files
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string vertexPath = (currentPath.parent_path() / "space.vert").string();
    std::string fragmentPath = (currentPath.parent_path() / "space.frag").string();

    std::cout << "Using shader paths:\n" << vertexPath << "\n" << fragmentPath << std::endl;

    // Space background
    space = new Space("stars.png", vertexPath.c_str(), fragmentPath.c_str());

    // Enable transparency blending once
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput(window);

        // Simulation
        if (step < maxSteps) {
            for (auto& p : particles) {
                updateParticle(p, bh, 0.01);
            }
            saveTrajectory(particles, "traj_" + std::to_string(step) + ".csv");
            step++;
        }

        // Rendering
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        space->draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete space;
    glfwTerminate();
    return 0;
}
