#ifndef SPACE_H
#define SPACE_H

#include <string>
#include "Shader.h"

class Space {
private:
    unsigned int textureID;
    unsigned int quadVAO, quadVBO;
    Shader* shader;  // Pointer to shader program

public:
    Space(const std::string& imagePath, const std::string& vertexPath, const std::string& fragmentPath);
    ~Space();
    void draw();
};

#endif
