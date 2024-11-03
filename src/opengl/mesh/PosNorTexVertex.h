//
// Created by faliszewskii on 05.05.24.
//

#ifndef PUMAROBOT_POSNORTEXVERTEX_H
#define PUMAROBOT_POSNORTEXVERTEX_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include "../../../dep/glad/glad_glfw.h"
#else
    #include "../../glew_glfw.h"
#endif
#include <vector>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

struct PosNorTexVertex{
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoords;

    inline static std::vector<int> getSizes() { return {3, 3, 2}; }
    inline static std::vector<int> getTypes() { return {GL_FLOAT, GL_FLOAT, GL_FLOAT}; }
    inline static std::vector<int> getOffsets() { return {0, offsetof(PosNorTexVertex, normal), offsetof(PosNorTexVertex, texCoords)}; }
};

#endif //PUMAROBOT_POSNORTEXVERTEX_H
