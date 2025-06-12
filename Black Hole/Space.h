#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "Camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Space {
public:
    Space();
    ~Space();
    void render();
    void processInput(GLFWwindow* window, float deltaTime);
    void processMouseMovement(float xoffset, float yoffset);
    void processMouseScroll(float yoffset);
    Camera camera;

private:
    unsigned int VAO, VBO, EBO;
    unsigned int texture;
    Shader shader;
    void setupBuffers();
    void loadTexture();
};
