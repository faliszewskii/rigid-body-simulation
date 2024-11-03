//
// Created by faliszewskii on 16.06.24.
//

#include <glm/gtc/type_ptr.hpp>
#include "Gui.h"
#include "imgui.h"

Gui::Gui(AppContext &appContext) : appContext(appContext) {}

void Gui::render() {
    ImGui::Begin("Scene##d");
    if(ImGui::CollapsingHeader("Light"))
        renderLightUI(*appContext.light);
    if(ImGui::CollapsingHeader("Display")) {
        ImGui::Checkbox("Draw Cube", &appContext.drawCube);
        ImGui::Checkbox("Draw Diagonal", &appContext.drawDiagonal);
        ImGui::Checkbox("Draw Cube Trace", &appContext.drawTrace);
        ImGui::Checkbox("Draw Plane", &appContext.drawPlane);
        ImGui::Checkbox("Draw Gravity Vector", &appContext.drawGravity);
    }
    if(ImGui::CollapsingHeader("Rigid Body")) {
        ImGui::DragFloat("Cube Size", &appContext.rigidBody->cubeSize, 0.1f, 0.1f, 5.0f);
        ImGui::DragFloat("Cube Density", &appContext.rigidBody->cubeDensity, 0.1f, 0.1f, 5.0f);
        ImGui::DragFloat("Cube Tilt", &appContext.rigidBody->cubeTilt, 0.05f, -std::numbers::pi, std::numbers::pi);
        ImGui::DragFloat("Cube Angular Velocity", &appContext.rigidBody->cubeAngleVelocity, 0.1f, -std::numbers::pi, std::numbers::pi);
        ImGui::DragInt("Trace Point Count", &appContext.rigidBody->traceSize, 1, 100, 2000);
    }
    if(ImGui::CollapsingHeader("Simulation")) {
        ImGui::Checkbox("Gravity On", &appContext.rigidBody->gravityOn);
        ImGui::DragFloat("Simulation Step (ms)", &appContext.rigidBody->timeStepMs, 0.1f, 1.f, 100.f);
    }
    ImGui::End();
}

void Gui::renderLightUI(PointLight &light) {
    ImGui::ColorPicker3("Light Color", glm::value_ptr(light.color));
    ImGui::DragFloat3("Light Position", glm::value_ptr(light.position), 0.001f);
}
