//
// Created by faliszewskii on 16.06.24.
//

#ifndef OPENGL_TEMPLATE_POINT_H
#define OPENGL_TEMPLATE_POINT_H

#include <memory>
#include <glm/vec4.hpp>
#include "../../../opengl/mesh/EmptyVertex.h"
#include "../../../opengl/shader/Shader.h"
#include "../../../opengl/mesh/Mesh.h"

class Point {
    std::unique_ptr<Mesh<EmptyVertex>> mesh;
public:
    glm::vec3 position{};
    glm::vec4 color{1};

    Point() {
        mesh = std::make_unique<Mesh<EmptyVertex>>(Mesh<EmptyVertex>({EmptyVertex()}, {}, GL_POINTS));
    }

    void render(Shader &shader) {
        shader.setUniform("position", position);
        shader.setUniform("color", color);
        mesh->render();
    }
};

#endif //OPENGL_TEMPLATE_POINT_H
