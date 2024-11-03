//
// Created by faliszewskii on 28.04.24.
//

#ifndef PUMAROBOT_POSITIONVERTEX_H
#define PUMAROBOT_POSITIONVERTEX_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include "../../../dep/glad/glad_glfw.h"
#else
#include "../../../dep/glew/glew_glfw.h"
#endif
#include <vector>
#include "glm/vec3.hpp"

struct PositionVertex {
    glm::vec3 position;

    inline static std::vector<int> getSizes() { return {3}; }
    inline static std::vector<int> getTypes() { return {GL_FLOAT}; }
    inline static std::vector<int> getOffsets() { return {0}; }
};

#endif //PUMAROBOT_POSITIONVERTEX_H
