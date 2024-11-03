//
// Created by USER on 03/11/2024.
//

#ifndef LINE_H
#define LINE_H

#include <algorithm>
#include <memory>
#include "../../../opengl/mesh/Mesh.h"
#include "../../../opengl/mesh/PositionVertex.h"

class Line {
    std::unique_ptr<Mesh<PositionVertex>> mesh;

public:
    Line() {
        mesh = std::make_unique<Mesh<PositionVertex>>(std::vector<PositionVertex>{}, std::nullopt, GL_LINE_STRIP);
    }

    void updatePoints(std::vector<glm::vec3> points) {
        std::vector<PositionVertex> vertices;
        std::ranges::transform(points, std::back_inserter(vertices), [](glm::vec3 point) {return PositionVertex(point);});
        mesh->update(std::move(vertices));
    }

    void render() {
        mesh->render();
    }
};
#endif //LINE_H
