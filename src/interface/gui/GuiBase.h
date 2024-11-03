//
// Created by faliszewskii on 16.06.24.
//

#ifndef OPENGL_TEMPLATE_GUIBASE_H
#define OPENGL_TEMPLATE_GUIBASE_H


#include "imgui.h"
#include "../../app/AppContext.h"
#include "Gui.h"

class GuiBase {
    Gui gui;
    AppContext &appContext;

    void showScene();

public:
    explicit GuiBase(AppContext &appContext, GLFWwindow *window);

    void render();

    static void newFrame();

    ~GuiBase();
};

#endif //OPENGL_TEMPLATE_GUIBASE_H
