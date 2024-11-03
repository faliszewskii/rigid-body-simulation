//
// Created by faliszewskii on 18.04.24.
//

#ifndef OPENGL_TEMPLATE_INPUTHANDLER_H
#define OPENGL_TEMPLATE_INPUTHANDLER_H

#include <functional>
#include "../../app/AppContext.h"

class InputHandler {
    AppContext &appContext;

public:
    explicit InputHandler(AppContext &appContext) : appContext(appContext) {};

    void setupCallbacks(GLFWwindow *window);

private:
    bool guiFocus = true;

    void keyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) const;
    void mouseButtonCallback(GLFWwindow *window, int button, int action, int mods);
    void mouseCallback(GLFWwindow *window, double xposIn, double yposIn);
    void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
    void windowResize(GLFWwindow* window, int width, int height);
};




#endif //OPENGL_TEMPLATE_INPUTHANDLER_H
