//
// Created by USER on 03/11/2024.
//

#include "RigidBody.h"

#include <glm/gtc/quaternion.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "glm/gtx/string_cast.hpp"
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "../bullet/btTransformHelper.h"


RigidBody::RigidBody() {
    cubeSize = 1;
    cubeDensity = 1;
    cubeTilt = 0;
    cubeAngleVelocity = 0;
    gravityOn = true;
    traceSize = 1000;
    timeStepMs = 0.1;

    glm::vec3 a = glm::normalize(glm::vec3({1,0, 1}));

    line.updatePoints({ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f) });
    gravityVector.updatePoints({ glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f, -1.0f, -1.0f) });

    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    glm::vec3 g = glm::normalize(glm::vec3(1));
    g = g * -9.81f;
    dynamicsWorld->setGravity(btVector3(g.x,g.y,g.z));

    btScalar mass = cubeDensity * cubeSize * cubeSize * cubeSize;
    auto d = cubeSize / 2;
    boxShape = new btBoxShape(btVector3(btScalar(d), btScalar(d), btScalar(d)));
    btVector3 inertia(0, 0, 0);
    boxShape->calculateLocalInertia(mass, inertia);

    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(d,d,d));
    auto* motionState = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, boxShape, inertia);
    cubeRigidBody = new btRigidBody(rigidBodyCI);

    glm::vec3 cornerPosition = glm::vec3(-d,-d,-d);
    btVector3 pivot = btVector3(cornerPosition.x,cornerPosition.y,cornerPosition.z);

    auto* constraint = new btPoint2PointConstraint(*cubeRigidBody, pivot);


    cubeRigidBody->setActivationState(DISABLE_DEACTIVATION);
    cubeRigidBody->setFriction(.0);
    cubeRigidBody->setRollingFriction(.0);
    cubeRigidBody->setSpinningFriction(.0);
    cubeRigidBody->setAnisotropicFriction(boxShape->getAnisotropicRollingFrictionDirection(), btCollisionObject::CF_ANISOTROPIC_FRICTION_DISABLED);

    dynamicsWorld->addConstraint(constraint);
    reset();
}

void RigidBody::reset() {
    dynamicsWorld->removeRigidBody(cubeRigidBody);

    float cube5 = cubeSize*cubeSize*cubeSize*cubeSize*cubeSize;
    float itOff = - cube5 * cubeDensity / 4.f;
    float itDiag = 2.f * cube5 * cubeDensity / 3.f;

    inertiaTensor = {
            itDiag, itOff, itOff,
            itOff, itDiag, itOff,
            itOff, itOff, itDiag
    };
    inertiaTensorInverse = glm::inverse(inertiaTensor);

    orientation = glm::identity<glm::quat>();
    glm::vec3 axis = glm::normalize(glm::vec3(0.0f, 0.0f, 1.0f));
    glm::quat rotation = glm::angleAxis(glm::radians(cubeTilt), axis);
    orientation = rotation * orientation;

    angleVelocity = glm::vec3(1);
    angleVelocity = glm::normalize(angleVelocity)* cubeAngleVelocity;

    tracePoints.clear();
    trace.updatePoints({});

    auto model = glm::identity<glm::mat4>();
    model = model * glm::toMat4(orientation);
    model = glm::translate(model, glm::vec3(1.f/2));

//    btQuaternion bulletQuat(orientation.x, orientation.y, orientation.z, orientation.w);
//    btTransform transform(bulletQuat, btVector3(cubeSize/2,cubeSize/2,cubeSize/2));
//    cubeRigidBody->setCenterOfMassTransform(transform);  // Set center of mass to match reset transform
    cubeRigidBody->setWorldTransform(mat4ToBtTransform(model));  // Apply the reset transform to the rigid body

    // Reset angular velocity
    btVector3 bulletAngularVelocity(angleVelocity.x, angleVelocity.y, angleVelocity.z);
    cubeRigidBody->setAngularVelocity(bulletAngularVelocity);
    cubeRigidBody->setLinearVelocity(btVector3(0,0,0));

    // Reset the cube's mass and inertia tensor
    btVector3 inertia;
    boxShape->calculateLocalInertia(cubeDensity * cubeSize * cubeSize * cubeSize, inertia);
    cubeRigidBody->setMassProps(cubeDensity * cubeSize * cubeSize * cubeSize, inertia);  // Update Bullet rigid body with new mass and inertia tensor

    dynamicsWorld->addRigidBody(cubeRigidBody);
}

void RigidBody::advanceByStep() {
    float h = timeStepMs / 1000.f;

    glm::vec3 k1_v = getAngularAcceleration(getGravitationalTorque(orientation), angleVelocity);
    glm::vec3 k1_w = angleVelocity;

    glm::vec3 k2_v = getAngularAcceleration(getGravitationalTorque(orientation), angleVelocity + 0.5f * h * k1_v);
    glm::vec3 k2_w = angleVelocity + 0.5f * h * k1_v;

    glm::vec3 k3_v = getAngularAcceleration(getGravitationalTorque(orientation), angleVelocity + 0.5f * h * k2_v);
    glm::vec3 k3_w = angleVelocity + 0.5f * h * k2_v;

    glm::vec3 k4_v = getAngularAcceleration(getGravitationalTorque(orientation), angleVelocity + h * k3_v);
    glm::vec3 k4_w = angleVelocity + h * k3_v;

    angleVelocity += (h / 6.0f) * (k1_v + 2.0f * k2_v + 2.0f * k3_v + k4_v);

    glm::quat q1 = getOrientationChange(orientation, k1_w);
    glm::quat q2 = getOrientationChange(orientation + 0.5f * h * q1, k2_w);
    glm::quat q3 = getOrientationChange(orientation + 0.5f * h * q2, k3_w);
    glm::quat q4 = getOrientationChange(orientation + h * q3, k4_w);

    orientation += (q1 + 2.0f * q2 + 2.0f * q3 + q4) * (h / 6.0f);
    orientation = glm::normalize(orientation);
}

void RigidBody::advanceByStepBullet() {
    float h = timeStepMs / 1000.f;

    // Step the simulation forward (this handles all physics calculations)
    dynamicsWorld->stepSimulation(h, 10); // Using 10 substeps for precision

    // Get the updated cube orientation and angular velocity from Bullet
    btTransform transform;
    cubeRigidBody->getMotionState()->getWorldTransform(transform);
    btQuaternion updatedOrientation = transform.getRotation();

    btVector3 updatedAngularVelocity = cubeRigidBody->getAngularVelocity();

    // Update your local variables (e.g., angleVelocity) if needed
    angleVelocity = glm::vec3(updatedAngularVelocity.x(), updatedAngularVelocity.y(), updatedAngularVelocity.z());
    orientation = glm::quat(updatedOrientation.w(), updatedOrientation.x(), updatedOrientation.y(), updatedOrientation.z());
}

glm::vec3 RigidBody::getGravitationalTorque(glm::quat theta) {
    if(!gravityOn) return glm::vec3(0.0f);

    auto q = theta * glm::vec3(cubeSize) / 2.f;
    float gg = 9.81;
    glm::vec3 g = glm::vec3(-1, -1, -1);
    g = glm::normalize(g) * gg;
    g = glm::cross(q, g);
    glm::quat qN = glm::conjugate(theta) * glm::quat(0, g) * theta;
    glm::vec3 N = {qN.x, qN.y, qN.z};
    return N;
}

glm::vec3 RigidBody::getAngularAcceleration(glm::vec3 F, glm::vec3 w) {
    return inertiaTensorInverse * (F + glm::cross(inertiaTensor * w, w) );
}

glm::quat RigidBody::getOrientationChange(glm::quat theta, glm::vec3 w) {
    return theta * glm::quat(0, w / 2.f);
}


void RigidBody::updateTrace() {
    auto q = orientation * glm::vec3(cubeSize);
    glm::mat4 model = glm::identity<glm::mat4>();
    model = glm::rotate(model, -(float)(glm::asin(1.f/std::sqrt(3))), glm::vec3(1,0,0));
    model = glm::rotate(model, (float)(std::numbers::pi/4.f), glm::vec3(0,0,1));
    q = glm::vec3(model * glm::vec4(q, 1));
    tracePoints.push_back(q);
    while(tracePoints.size() > traceSize)
        tracePoints.erase(tracePoints.begin());
    trace.updatePoints(tracePoints);
}

void RigidBody::renderCube(Shader &shader) {
    auto model = glm::identity<glm::mat4>();
    model = glm::rotate(model, -(float)(glm::asin(1.f/std::sqrt(3))), glm::vec3(1,0,0));
    model = glm::rotate(model, (float)(std::numbers::pi/4.f), glm::vec3(0,0,1));
    model = glm::scale(model, glm::vec3(cubeSize));
    model = model * btTransformToMat4(cubeRigidBody->getWorldTransform());

    shader.setUniform("model", model);
    cube.render();
}

void RigidBody::renderDiagonal(Shader &shader) {
    auto model = glm::identity<glm::mat4>();
    model = model * glm::scale(model, glm::vec3(cubeSize));
    model = glm::rotate(model, -(float)(glm::asin(1.f/std::sqrt(3))), glm::vec3(1,0,0));
    model = glm::rotate(model, (float)(std::numbers::pi/4.f), glm::vec3(0,0,1));
    model = model * glm::mat4_cast(orientation);
    shader.setUniform("model", model);
    shader.setUniform("color", glm::vec4{0.5, 0.5, 0, 1});
    line.render();
}

void RigidBody::renderTrace(Shader &shader) {
    shader.setUniform("model", glm::identity<glm::mat4>());
    shader.setUniform("color", glm::vec4{1, 1, 1, 1});
    trace.render();
}

void RigidBody::renderGravityVector(Shader &shader) {
    auto q = orientation * glm::vec3(cubeSize);

    auto model = glm::identity<glm::mat4>();
    model = glm::rotate(model, -(float)(glm::asin(1.f/std::sqrt(3))), glm::vec3(1,0,0));
    model = glm::rotate(model, (float)(std::numbers::pi/4.f), glm::vec3(0,0,1));
    model = glm::translate(model, q / 2.f);

    shader.setUniform("model", model);
    if(gravityOn) {
        shader.setUniform("color", glm::vec4{0, 0.5, 1, 1});
    } else {
        shader.setUniform("color", glm::vec4{0, 0.5, 1, 0.2});
    }

    gravityVector.render();
}
