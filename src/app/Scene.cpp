//
// Created by faliszewskii on 16.06.24.
//

#include "Scene.h"
#include "../interface/camera/CameraAnchor.h"

Scene::Scene(AppContext &appContext) : appContext(appContext) {
    appContext.camera = std::make_unique<CameraAnchor>(1920, 1080, glm::vec3(0.0f, 3.0f, 3.0f), glm::vec3(0.f), glm::vec3(-M_PI / 4, 0, 0));
    appContext.frameBufferManager = std::make_unique<FrameBufferManager>();
    appContext.frameBufferManager->create_buffers(appContext.camera->screenWidth, appContext.camera->screenHeight);

    // TODO --- Initialization of the app state goes here.

    appContext.phongShader = std::make_unique<Shader>(Shader::createTraditionalShader(
            "../res/shaders/phong/phong.vert", "../res/shaders/phong/phong.frag"));
    appContext.pointShader = std::make_unique<Shader>(Shader::createTraditionalShader(
    "../res/shaders/point/point.vert", "../res/shaders/point/point.frag"));
    appContext.colorShader = std::make_unique<Shader>(Shader::createTraditionalShader(
            "../res/shaders/basic/position.vert", "../res/shaders/basic/color.frag"));

    appContext.quad = std::make_unique<Quad>();
    appContext.light = std::make_unique<PointLight>();
    appContext.lightBulb = std::make_unique<Point>();

    appContext.axes = std::make_unique<Axes>();
    appContext.rigidBody = std::make_unique<RigidBody>();

    appContext.drawCube = true;
    appContext.drawDiagonal = true;
    appContext.drawTrace = true;
    appContext.drawGravity = true;
    appContext.drawPlane = true;

    appContext.running = false;
    appContext.parametersBlocked = false;
    appContext.lastFrameTimeMs = glfwGetTime();
}

void Scene::update() {
    appContext.lightBulb->position = appContext.light->position;
    appContext.lightBulb->color = glm::vec4(appContext.light->color, 1);

    if(appContext.running) {
        float timeMs = glfwGetTime() * 1000;
        int loopsToDo = static_cast<int>((timeMs - appContext.lastFrameTimeMs) / appContext.rigidBody->timeStepMs);
        appContext.lastFrameTimeMs += loopsToDo * appContext.rigidBody->timeStepMs;
        for (int i = 0; i < loopsToDo; i++) {
            appContext.rigidBody->advanceByStep();
        }
        appContext.rigidBody->updateTrace();
    } else {
        appContext.lastFrameTimeMs = glfwGetTime() * 1000.f;
    }

}

void Scene::render() {
    appContext.frameBufferManager->bind();

    appContext.pointShader->use();
    appContext.pointShader->setUniform("view", appContext.camera->getViewMatrix());
    appContext.pointShader->setUniform("projection", appContext.camera->getProjectionMatrix());
    appContext.lightBulb->render(*appContext.pointShader);

    appContext.colorShader->use();
    appContext.colorShader->setUniform("view", appContext.camera->getViewMatrix());
    appContext.colorShader->setUniform("projection", appContext.camera->getProjectionMatrix());
    appContext.axes->render(*appContext.colorShader);

    appContext.colorShader->setUniform("model", glm::rotate(glm::identity<glm::mat4>(), glm::radians(-90.0f), glm::vec3(1, 0, 0)));
    appContext.colorShader->setUniform("color", glm::vec4{0.5, 0.5, 0.5, 0.6});
    if(appContext.drawPlane) appContext.quad->render();
    if(appContext.drawDiagonal) appContext.rigidBody->renderDiagonal(*appContext.colorShader);
    if(appContext.drawTrace) appContext.rigidBody->renderTrace(*appContext.colorShader);
    if(appContext.drawGravity) appContext.rigidBody->renderGravityVector(*appContext.colorShader);

    appContext.phongShader->use();
    appContext.phongShader->setUniform("viewPos", appContext.camera->getViewPosition());
    appContext.phongShader->setUniform("view", appContext.camera->getViewMatrix());
    appContext.phongShader->setUniform("projection", appContext.camera->getProjectionMatrix());
    appContext.phongShader->setUniform("material.hasTexture", false);
    appContext.phongShader->setUniform("material.albedo", glm::vec4(0.5, 0.5, 0.5, 0.6));
    appContext.phongShader->setUniform("material.shininess", 256.f);
    appContext.light->setupPointLight(*appContext.phongShader);
    if(appContext.drawCube) appContext.rigidBody->renderCube(*appContext.phongShader);

    appContext.frameBufferManager->unbind();
}
