#ifndef SPACE_H
#define SPACE_H

#include <string>

class Space {
private:
    unsigned int textureID;
    unsigned int quadVAO, quadVBO;

public:
    Space(const std::string& imagePath);
    ~Space();
    void draw();
};

#endif