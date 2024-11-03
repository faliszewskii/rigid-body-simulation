//
// Created by faliszewskii on 18.04.24.
//

#ifndef PUMAROBOT_CAMERAANCHORFREE_H
#define PUMAROBOT_CAMERAANCHORFREE_H

#include <glm/gtc/quaternion.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <map>
#include "BaseCamera.h"


class CameraAnchor : public BaseCamera {
private:
    glm::quat orientation;

    float movementSpeed;
    float mouseSensitivity;
    float zoomSensitivity; // TODO Maybe change based on how close anchor to
    glm::vec3 front{};
    glm::vec3 up{};
    glm::vec3 right{};

public:
    glm::vec3 position;
    glm::vec3 anchor;
    float nearPlane;
    float farPlane;
    float fov;

    explicit CameraAnchor(
            int screenWidth,
            int screenHeight,
            glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3 anchor = glm::vec3(0.0f, 0.0f, 0.0f),
            glm::quat orientation = glm::quat(1, 0, 0, 0)
    );

    [[nodiscard]] glm::mat4 getViewMatrix() const override;
    [[nodiscard]] glm::mat4 getNoTranslationViewMatrix() const override;
    [[nodiscard]] glm::mat4 getMirrorViewMatrix() const override;
    [[nodiscard]] glm::mat4 getProjectionMatrix() const override;
    [[nodiscard]] glm::vec3 getViewPosition() const override;
    [[nodiscard]] float getFarPlane() const override;
    [[nodiscard]] float getNearPlane() const override;

    void processKeyboard(CameraMovement direction, float deltaTime) override;
    void processMouseMovement(float xoffset, float yoffset) override;
    void processMouseScroll(float yoffset) override;

    void resize(int screenWidth, int screenHeight) override;

private:
    void updateDirections();
};

#endif //PUMAROBOT_CAMERAANCHORFREE_H
