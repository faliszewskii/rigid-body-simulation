//
// Created by faliszewskii on 07.01.24.
//

#include "CameraAnchor.h"
#include <cmath>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/detail/type_quat.hpp>
#include <glm/gtc/quaternion.hpp>


CameraAnchor::CameraAnchor(int screenWidth, int screenHeight, glm::vec3 position, glm::vec3 anchor, glm::quat orientation) :
    orientation(orientation), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoomSensitivity(ZOOM_SENSITIVITY),
    position(position), anchor(anchor), nearPlane(0.01), farPlane(100), fov(45.f)
{
    BaseCamera::screenWidth = screenWidth;
    BaseCamera::screenHeight = screenHeight;
    // TODO Set orientation to anchor
    updateDirections();
}

static glm::vec3 getFront(glm::quat q) {
    return glm::normalize(q * glm::vec3(0, 0, -1.0f));
}

static glm::vec3 getUp(glm::quat q) {
    return glm::normalize(q * glm::vec3(0, 1.0f, 0));
}

static glm::vec3 getRight(glm::quat q) {
    return glm::normalize(q * glm::vec3(1.0f, 0, 0));
}

void CameraAnchor::updateDirections() {
    front = getFront(orientation);
    up = getUp(orientation);
    right = getRight(orientation);
}

glm::mat4 myLookAt(glm::vec3 eye, glm::vec3 center, glm::vec3 up) {
    auto f(normalize(center - eye));
    auto s(normalize(cross(f, up)));
    auto u(cross(s, f));

    return {
            {s.x, u.x, -f.x, 0},
            {s.y, u.y, -f.y, 0},
            {s.z, u.z, -f.z, 0},
            {-dot(s, eye), -dot(u, eye), dot(f, eye), 1},
    };
}

glm::mat4 CameraAnchor::getViewMatrix() const {
    return myLookAt(position, position + front, up);
}

glm::mat4 myProjection(float yFov, float aspectRatio, float zNear, float zFar) {
    float tanHalfYFov = std::tan(yFov / 2);

    return {
            {1 / aspectRatio / tanHalfYFov, 0, 0, 0},
            {0, 1 / tanHalfYFov, 0, 0},
            {0, 0, - (zFar + zNear) / (zFar - zNear), -1},
            {0, 0, -(2 * zFar * zNear) / (zFar - zNear),0},
    };
}

glm::mat4 CameraAnchor::getProjectionMatrix() const {
    return myProjection(glm::radians(fov), (float) screenWidth / (float) screenHeight, nearPlane, farPlane);
}

glm::vec3 CameraAnchor::getViewPosition() const {
    return position;
}

void CameraAnchor::processKeyboard(CameraMovement direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    if (direction == FORWARD)
        anchor += front * velocity;
    if (direction == BACKWARD)
        anchor -= front * velocity;
    if (direction == LEFT)
        anchor -= right * velocity;
    if (direction == RIGHT)
        anchor += right * velocity;

    //TODO move orientation
    updateDirections();
}

void CameraAnchor::processMouseMovement(float xoffset, float yoffset) {
    xoffset *= mouseSensitivity;
    yoffset *= mouseSensitivity;

    glm::quat rotationX = glm::angleAxis(xoffset, up);
    glm::quat rotationY = glm::angleAxis(yoffset, right);

    position = glm::vec3(rotationX * rotationY * glm::vec4(position - anchor, 1)) + anchor;
    orientation = glm::normalize(rotationX * rotationY * orientation);
    updateDirections();
}
void CameraAnchor::processMouseScroll(float yoffset) {
    yoffset *= zoomSensitivity;
    position += front * yoffset;
}

void CameraAnchor::resize (int screenWidth, int screenHeight)
{
    this->screenWidth = screenWidth;
    this->screenHeight = screenHeight;
}

glm::mat4 CameraAnchor::getNoTranslationViewMatrix() const {
    return glm::mat3(myLookAt(position, position + front, up));
}

glm::mat4 CameraAnchor::getMirrorViewMatrix () const
{
    glm::mat4 mirrorModel(1.0f);

    mirrorModel = glm::rotate(mirrorModel, (float)M_PI/6, glm::vec3(0,0,1));
    mirrorModel = glm::translate(mirrorModel, glm::vec3{-1.23f, 1.0f, 0.0f});
    mirrorModel = glm::scale(mirrorModel, glm::vec3(2,2,2));
    return  myLookAt(position, position + front, up) * mirrorModel * glm::scale(glm::mat4(1), glm::vec3(-1, 1, 1)) * glm::inverse(mirrorModel);
}

float CameraAnchor::getFarPlane() const {
    return farPlane;
};
float CameraAnchor::getNearPlane() const {
    return nearPlane;
};