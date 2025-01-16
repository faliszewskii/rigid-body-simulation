//
// Created by USER on 15/01/2025.
//

#ifndef SPRINGBODY_H
#define SPRINGBODY_H

#include <random>

#include "../entity/cube/Cube.h"
#include "../physics/PhysicsBody.h"
#include "btBulletDynamicsCommon.h"
#include "TreePart.h"

class SpringBody {

    Cube cube;

    btDefaultCollisionConfiguration* collisionConfiguration;
    btCollisionDispatcher* dispatcher;
    btBroadphaseInterface* overlappingPairCache;
    btSequentialImpulseConstraintSolver* solver;
    btDiscreteDynamicsWorld* dynamicsWorld;

    // btBoxShape* boxShape;
    // btRigidBody* cubeRigidBody;
    // btRigidBody* cubeRigidBody2;

    std::vector<TreePart> rigidBodies;
    std::vector<btRigidBody*> leafs;

    std::random_device rd;  // Will be used to obtain a seed for the random number engine
    std::mt19937 gen; // Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dist;
    std::uniform_real_distribution<> angleDist;

public:
    float timeStepMs;
    float windStrength;

    btRigidBody* growTree(glm::vec3 origin, glm::vec3 direction, int height);
    void growleaf(btRigidBody* branch, glm::vec3 origin, glm::vec3 direction, bool left);

    SpringBody();
    void reset();
    void advanceByStep(float timeS);

    void render(Shader &shader);
};



#endif //SPRINGBODY_H
