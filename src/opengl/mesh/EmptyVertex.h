//
// Created by faliszewskii on 28.04.24.
//

#ifndef PUMAROBOT_EMPTYVERTEX_H
#define PUMAROBOT_EMPTYVERTEX_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include "../../../dep/glad/glad_glfw.h"
#else
#include "../../../dep/glew/glew_glfw.h"
#endif
#include <vector>
#include "glm/vec3.hpp"

struct EmptyVertex {
    inline static std::vector<int> getSizes() { return {}; }
    inline static std::vector<int> getTypes() { return {}; }
    inline static std::vector<int> getOffsets() { return {}; }
};

#endif //PUMAROBOT_EMPTYVERTEX_H
