//
// Created by faliszewskii on 16.06.24.
//

#ifndef OPENGL_TEMPLATE_APPCONTEXT_H
#define OPENGL_TEMPLATE_APPCONTEXT_H

#include <memory>
#include "../opengl/framebuffer/FrameBufferManager.h"
#include "../interface/camera/BaseCamera.h"
#include "entity/quad/Quad.h"
#include "../opengl/shader/Shader.h"
#include "entity/axes/Axes.h"
#include "entity/light/PointLight.h"
#include "entity/point/Point.h"
#include "rigid/RigidBody.h"

struct AppContext {
    AppContext() = default;

    std::unique_ptr<BaseCamera> camera;
    std::unique_ptr<FrameBufferManager> frameBufferManager;

    // Shaders
    std::unique_ptr<Shader> phongShader;
    std::unique_ptr<Shader> pointShader;
    std::unique_ptr<Shader> colorShader;

    // TODO --- App data goes here
    std::unique_ptr<PointLight> light;
    std::unique_ptr<Point> lightBulb;
    std::unique_ptr<Quad> quad;

    std::unique_ptr<Axes> axes;
    std::unique_ptr<RigidBody> rigidBody;

};

#endif //OPENGL_TEMPLATE_APPCONTEXT_H
