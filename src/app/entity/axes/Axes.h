//
// Created by USER on 03/11/2024.
//

#ifndef AXES_H
#define AXES_H

#include <memory>
#include "../../../opengl/mesh/Mesh.h"
#include "../../../opengl/mesh/PositionVertex.h"

class Axes {
    std::unique_ptr<Mesh<PositionVertex>> meshR;
    std::unique_ptr<Mesh<PositionVertex>> meshG;
    std::unique_ptr<Mesh<PositionVertex>> meshB;
    float size;

public:
    Axes() {
        meshR = std::make_unique<Mesh<PositionVertex>>(std::vector<PositionVertex>{{{-1, 0, 0}}, {{1, 0, 0}}}, std::nullopt, GL_LINES);
        meshG = std::make_unique<Mesh<PositionVertex>>(std::vector<PositionVertex>{{{0, -1, 0}}, {{0, 1, 0}}}, std::nullopt, GL_LINES);
        meshB = std::make_unique<Mesh<PositionVertex>>(std::vector<PositionVertex>{{{0, 0, -1}}, {{0, 0, 1}}}, std::nullopt, GL_LINES);
        size = 5;
    }

    void render(Shader &shader) {
        auto model = glm::scale(glm::identity<glm::mat4>(), glm::vec3(size));
        shader.setUniform("model", model);
        shader.setUniform("color", glm::vec4{1, 0, 0, 0.5});
        meshR->render();
        shader.setUniform("color", glm::vec4{0, 1, 0, 0.5});
        meshG->render();
        shader.setUniform("color", glm::vec4{0, 0, 1, 0.5});
        meshB->render();
    }
};

#endif //AXES_H
