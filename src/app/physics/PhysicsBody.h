//
// Created by USER on 15/01/2025.
//

#ifndef PHYSICSBODY_H
#define PHYSICSBODY_H

#include "../../opengl/shader/Shader.h"

class PhysicsBody {
public:
    virtual ~PhysicsBody() = default;

    virtual void reset();
    virtual void advanceByStep();
};

#endif //PHYSICSBODY_H
