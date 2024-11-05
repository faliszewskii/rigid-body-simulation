//
// Created by faliszewskii on 16.06.24.
//

#include <glm/gtc/type_ptr.hpp>
#include "Gui.h"
#include "imgui.h"

Gui::Gui(AppContext &appContext) : appContext(appContext) {}

void Gui::render() {
    ImGui::Begin("Scene##d");

    if(ImGui::Button(appContext.running? "Stop": "Start")) {
        if(!appContext.running) appContext.parametersBlocked = true;
        appContext.running = !appContext.running;
    }
    ImGui::SameLine();
    if(ImGui::Button("Reset")) {
        appContext.parametersBlocked = false;
        appContext.running = false;
        appContext.rigidBody->reset();
        appContext.lastFrameTimeMs = glfwGetTime() * 1000;
    }

    if(ImGui::CollapsingHeader("Simulation")) {
        ImGui::Checkbox("Gravity On", &appContext.rigidBody->gravityOn);
        ImGui::DragFloat("Simulation Step (ms)", &appContext.rigidBody->timeStepMs, 0.1f, 1.f, 100.f);
    }

    if(ImGui::CollapsingHeader("Rigid Body")) {
        ImGui::BeginDisabled(appContext.parametersBlocked);
        {
            bool changed = false;
            changed |= ImGui::DragFloat("Size", &appContext.rigidBody->cubeSize, 0.01f, 0.1f, 5.0f);
            changed |= ImGui::DragFloat("Density", &appContext.rigidBody->cubeDensity, 0.01f, 0.1f, 5.0f);
            changed |= ImGui::DragFloat("Tilt (°)", &appContext.rigidBody->cubeTilt, 0.01f, -180, 180);
            changed |= ImGui::DragFloat("Angular Velocity", &appContext.rigidBody->cubeAngleVelocity, 0.1f, 0, 100);
            if(changed) appContext.rigidBody->reset();
            ImGui::EndDisabled();
        }
        ImGui::DragInt("Trace Point Count", &appContext.rigidBody->traceSize, 1, 100, 2000);
    }

    if(ImGui::CollapsingHeader("Display")) {
        ImGui::Checkbox("Draw Cube", &appContext.drawCube);
        ImGui::Checkbox("Draw Diagonal", &appContext.drawDiagonal);
        ImGui::Checkbox("Draw Cube Trace", &appContext.drawTrace);
        ImGui::Checkbox("Draw Plane", &appContext.drawPlane);
        ImGui::Checkbox("Draw Gravity Vector", &appContext.drawGravity);
    }

    if(ImGui::CollapsingHeader("Light"))
        renderLightUI(*appContext.light);

    ImGui::End();
}

void Gui::renderLightUI(PointLight &light) {
    ImGui::ColorPicker3("Light Color", glm::value_ptr(light.color));
    ImGui::DragFloat3("Light Position", glm::value_ptr(light.position), 0.001f);
}
