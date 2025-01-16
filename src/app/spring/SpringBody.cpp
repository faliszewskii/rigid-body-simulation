//
// Created by USER on 15/01/2025.
//

#include "SpringBody.h"
#include "../bullet/btTransformHelper.h"

void SpringBody::growleaf(btRigidBody* branch, glm::vec3 origin, glm::vec3 direction, bool left) {
    float mass = 0.1f;
    auto* boxShape = new btBoxShape(btVector3(0.01,0.01,0.01));
    btVector3 inertia1(0, 0, 0);
    boxShape->calculateLocalInertia(mass, inertia1);

    auto v = glm::cross(direction, glm::vec3(0.01,0.99,0));
    v = glm::normalize(v);
    glm::vec3 center = origin + direction * 0.5f + (left? v: -v);
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(center.x,center.y,center.z));

    auto* motionState1 = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI1(mass, motionState1, boxShape, inertia1);
    auto* rigidBody = new btRigidBody(rigidBodyCI1);
    leafs.push_back(rigidBody);

    rigidBody->setActivationState(DISABLE_DEACTIVATION);

    dynamicsWorld->addRigidBody(rigidBody);

    auto* constraint2 = new btHingeConstraint(*rigidBody, *branch,
        btVector3((left?1:-1) * 0.5, 0.0, 0), btVector3(0.0, 0.5, 0),
        btVector3(1,0,0), btVector3(1,0, 0));
    constraint2->setLimit(std::numbers::pi/2-std::numbers::pi/4, std::numbers::pi/2+std::numbers::pi/4);

    dynamicsWorld->addConstraint(constraint2);
}

btRigidBody* SpringBody::growTree(glm::vec3 origin, glm::vec3 direction, int height) {
    float mass = 1.f * exp(height);
    auto* boxShape = new btBoxShape(btVector3(0.1,0.1,0.1));
    btVector3 inertia1(0, 0, 0);
    boxShape->calculateLocalInertia(mass, inertia1);

    glm::vec3 center = origin + direction * 0.5f;
    btTransform transform;
    transform.setIdentity();
    transform.setOrigin(btVector3(center.x,center.y,center.z));

    auto* motionState1 = new btDefaultMotionState(transform);
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI1(mass, motionState1, boxShape, inertia1);
    auto* rigidBody = new btRigidBody(rigidBodyCI1);
    rigidBodies.emplace_back(rigidBody, height);

    rigidBody->setActivationState(DISABLE_DEACTIVATION);

    dynamicsWorld->addRigidBody(rigidBody);

    if(height < 3) growleaf(rigidBody, origin, direction, false);
    if(height < 3) growleaf(rigidBody, origin, direction, true);
    // if(height == 1) rigidBody->setLinearVelocity(btVector3(1,0,1));
    if(height == 1) return rigidBody;


    auto* childRigidBody = growTree(origin + direction, direction, height - 1);

    btTransform transformSpring1;
    btTransform transformSpring2;
    transformSpring1.setIdentity();
    transformSpring1.setOrigin(btVector3(0, 0.5, 0));
    transformSpring2.setIdentity();
    transformSpring2.setOrigin(btVector3(0, -0.5, 0));
    auto* constraint2 = new btGeneric6DofSpring2Constraint(*rigidBody, *childRigidBody, transformSpring1, transformSpring2);
    // constraint2->setLimit(0, 0, 0);
    // constraint2->setLimit(1, 0, 0);
    // constraint2->setLimit(2, 0, 0);
    constraint2->setLimit(3, 0, 0);
    constraint2->setLimit(4, 0, 0);
    constraint2->setLimit(5, 0, 0);
    // constraint2->enableSpring(3, false);
    // constraint2->enableSpring(4, false);
    // constraint2->enableSpring(5, false);
    // constraint2->setStiffness(3, 100);
    // constraint2->setDamping(3, 100*height);
    // constraint2->setStiffness(4, 100);
    // constraint2->setDamping(4, 100*height);
    // constraint2->setStiffness(5, 100);
    // constraint2->setDamping(5, 100*height);
    // constraint2->setEquilibriumPoint(0, 0);
    // constraint2->setDbgDrawSize(btScalar(2.f));

    dynamicsWorld->addConstraint(constraint2);

    float rand = dist(gen);
    if(rand > 0.3) {

        float angleX = angleDist(gen);
        if(angleX < 0) angleX -= std::numbers::pi/16;
        if(angleX > 0) angleX += std::numbers::pi/16;
        float angleZ = angleDist(gen);
        if(angleZ < 0) angleZ -= std::numbers::pi/16;
        if(angleZ > 0) angleZ += std::numbers::pi/16;

        auto m = glm::identity<glm::mat4>();
        m = glm::rotate(m, angleZ, glm::vec3(0, 0, 1));
        m = glm::rotate(m, angleX, glm::vec3(1, 0, 0));
        direction = glm::vec4(direction, 0) * m;

        auto* childRigidBody = growTree(origin + direction, direction, height /2);

        btTransform transformSpring1;
        btTransform transformSpring2;
        transformSpring1.setIdentity();
        transformSpring1.setOrigin(btVector3(0, 0.5, 0));
        transformSpring2.setIdentity();
        transformSpring2.setOrigin(btVector3(0, -0.5, 0));
        auto* constraint2 = new btGeneric6DofSpring2Constraint(*rigidBody, *childRigidBody, transformSpring1, transformSpring2);
        // constraint2->setLimit(0, 0, 0);
        // constraint2->setLimit(1, 0, 0);
        // constraint2->setLimit(2, 0, 0);
        constraint2->setLimit(3, angleX, angleX);
        constraint2->setLimit(4, 0, 0);
        constraint2->setLimit(5, angleZ, angleZ);
        // constraint2->enableSpring(3, false);
        // constraint2->enableSpring(4, false);
        // constraint2->enableSpring(5, false);
        // constraint2->setStiffness(3, 100);
        // constraint2->setDamping(3, 100*height);
        // constraint2->setStiffness(4, 100);
        // constraint2->setDamping(4, 100*height);
        // constraint2->setStiffness(5, 100);
        // constraint2->setDamping(5, 100*height);
        // constraint2->setEquilibriumPoint(0, 0);
        // constraint2->setDbgDrawSize(btScalar(2.f));

        dynamicsWorld->addConstraint(constraint2);
    }

    return rigidBody;
}

SpringBody::SpringBody() :gen(rd()), dist(0.f, 1.f), angleDist(-std::numbers::pi/4, std::numbers::pi/4){
    timeStepMs = 0.1;
    windStrength = 1;

    collisionConfiguration = new btDefaultCollisionConfiguration();
    dispatcher = new btCollisionDispatcher(collisionConfiguration);
    overlappingPairCache = new btDbvtBroadphase();
    solver = new btSequentialImpulseConstraintSolver;
    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    glm::vec3 g = glm::normalize(glm::vec3(1));
    g = g * -9.81f;
    dynamicsWorld->setGravity(btVector3(0,g.y,0));

    growTree(glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 15);

    btTransform transformSpring;
    transformSpring.setIdentity();
    transformSpring.setOrigin(btVector3(0, -0.5, 0));
    auto* constraint1 = new btGeneric6DofSpring2Constraint(*rigidBodies.front().rigidBody, transformSpring);
    // constraint1->setLimit(0, 0, 0);
    // constraint1->setLimit(1, 0, 0);
    // constraint1->setLimit(2, 0, 0);
    constraint1->setLimit(3, 0.0, 0.0);
    constraint1->setLimit(4, 0.0, 0.0);
    constraint1->setLimit(5, 0.0, 0.0);
    // constraint1->enableSpring(3, false);
    // constraint1->enableSpring(4, false);
    // constraint1->enableSpring(5, false);
    // constraint1->setStiffness(3, 100);
    // constraint1->setDamping(3, 100);
    // constraint1->setStiffness(4, 100);
    // constraint1->setDamping(4, 100);
    // constraint1->setStiffness(5, 100);
    // constraint1->setDamping(5, 100);
    dynamicsWorld->addConstraint(constraint1);

}

void SpringBody::reset() {
    for(auto& treePart : rigidBodies) {
        treePart.rigidBody->setAngularVelocity(btVector3(0, 0, 0));
        treePart.rigidBody->setLinearVelocity(btVector3(0, 0, 0));
    }
    for(auto& leaf : leafs) {
        leaf->setAngularVelocity(btVector3(0, 0, 0));
        leaf->setLinearVelocity(btVector3(0, 0, 0));
    }
}

void SpringBody::advanceByStep(float timeMs) {
    float h = timeStepMs / 1000.f;
    float coeff = std::sin(timeMs/1000) * windStrength;
    for(auto& treePart : rigidBodies) {
        treePart.rigidBody->applyForce(btVector3(0, 0, coeff), btVector3(0,0,0));
    }
    dynamicsWorld->stepSimulation(h, 10); // Using 10 substeps for precision
}

void SpringBody::render(Shader &shader) {
    for(auto& treePart : rigidBodies) {
        auto model = glm::identity<glm::mat4>();
        model = glm::scale(model, glm::vec3(0.05));
        model = model * btTransformToMat4(treePart.rigidBody->getWorldTransform());
        float thickness = std::sqrt(treePart.height) * 0.1f;
        model = glm::scale(model, glm::vec3(thickness,1,thickness));
        shader.setUniform("material.albedo", glm::vec4(0.7, 0.6, 0.5, 1));
        shader.setUniform("model", model);
        cube.render();
    }

    for(auto& leaf : leafs) {
        auto model = glm::identity<glm::mat4>();
        model = glm::scale(model, glm::vec3(0.05));
        model = model * btTransformToMat4(leaf->getWorldTransform());
        model = glm::scale(model, glm::vec3(1,0.05,0.4));
        shader.setUniform("material.albedo", glm::vec4(0.2, 0.5, 0.2, 0.8));
        shader.setUniform("model", model);
        cube.render();
    }
}
