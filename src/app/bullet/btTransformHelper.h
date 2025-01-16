//
// Created by USER on 15/01/2025.
//

#ifndef BTTRANSFORMHELPER_H
#define BTTRANSFORMHELPER_H

#include <glm/fwd.hpp>

#include "btBulletDynamicsCommon.h"

inline glm::mat4 btTransformToMat4(const btTransform& transform)
{
    // Extract the translation and rotation components
    btVector3 translation = transform.getOrigin();          // Get the position (translation)
    btMatrix3x3 rotation = transform.getBasis();             // Get the rotation (3x3 matrix)

    // Convert btVector3 to glm::vec3
    glm::vec3 glmTranslation(translation.x(), translation.y(), translation.z());

    // Convert btMatrix3x3 to glm::mat3
    glm::mat3 glmRotation(
            rotation[0][0], rotation[0][1], rotation[0][2],
            rotation[1][0], rotation[1][1], rotation[1][2],
            rotation[2][0], rotation[2][1], rotation[2][2]
    );

    // Construct the glm::mat4 model matrix
    glm::mat4 modelMatrix = glm::mat4(1.0f);  // Start with an identity matrix

    // Set the upper-left 3x3 rotation matrix
    modelMatrix = glm::mat4(glm::transpose(glmRotation));

    // Set the translation component (position) in the model matrix
    modelMatrix[3] = glm::vec4(glmTranslation, 1.0f);

    return modelMatrix;
}

inline btTransform mat4ToBtTransform(const glm::mat4& modelMatrix)
{
    // Extract the translation component from glm::mat4
    glm::vec3 glmTranslation(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);

    // Extract the upper-left 3x3 matrix for rotation
    glm::mat3 glmRotation = glm::transpose(glm::mat3(modelMatrix));

    // Convert glm::vec3 to btVector3 for translation
    btVector3 translation(glmTranslation.x, glmTranslation.y, glmTranslation.z);

    // Convert glm::mat3 to btMatrix3x3 for rotation
    btMatrix3x3 rotation(
            glmRotation[0][0], glmRotation[0][1], glmRotation[0][2],
            glmRotation[1][0], glmRotation[1][1], glmRotation[1][2],
            glmRotation[2][0], glmRotation[2][1], glmRotation[2][2]
    );

    // Construct the btTransform
    btTransform transform;
    transform.setOrigin(translation); // Set translation
    transform.setBasis(rotation);    // Set rotation

    return transform;
}

#endif //BTTRANSFORMHELPER_H
