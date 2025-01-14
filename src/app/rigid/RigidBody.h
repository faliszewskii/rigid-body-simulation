//
// Created by USER on 03/11/2024.
//

#ifndef RIGIDBODY_H
#define RIGIDBODY_H

#include <glm/detail/type_quat.hpp>

#include "../entity/cube/Cube.h"
#include "../entity/line/Line.h"
#include "../../opengl/shader/Shader.h"
#include "btBulletDynamicsCommon.h"

class RigidBody {
    Cube cube;
    Line line;
    Line gravityVector;

    Line trace;
    std::vector<glm::vec3> tracePoints;

    glm::quat orientation;
    glm::vec3 angleVelocity;
    glm::mat3 inertiaTensor;
    glm::mat3 inertiaTensorInverse;

    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

    btBoxShape* boxShape;
    btRigidBody* cubeRigidBody;

public:
    float cubeSize;
    float cubeDensity;
    float cubeTilt;

    float cubeAngleVelocity;

    float timeStepMs;

    bool gravityOn;

    int traceSize;

    RigidBody();
    void reset();
    void advanceByStep();

    void updateTrace();
    void renderCube(Shader &shader);
    void renderDiagonal(Shader &shader);
    void renderTrace(Shader &shader);

    void renderGravityVector(Shader &shader);
    glm::vec3 getGravitationalTorque(glm::quat theta);

    glm::vec3 getAngularAcceleration(glm::vec3 F, glm::vec3 w);

    glm::quat getOrientationChange(glm::quat theta, glm::vec3 w);

    void advanceByStepBullet();
};



#endif //RIGIDBODY_H
