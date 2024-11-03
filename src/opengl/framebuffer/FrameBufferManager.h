//
// Created by Bartek Jadczak on 12/03/2024.
//

#ifndef OPENGLMODELEDITOR_FRAMEBUFFERMANAGER_H
#define OPENGLMODELEDITOR_FRAMEBUFFERMANAGER_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#include "../../../dep/glad/glad_glfw.h"
#else
    #include "../../glew_glfw.h"
#endif

class FrameBufferManager
{
public:
    FrameBufferManager() : mFBO { 0 }, mTexId { 0 }, mDepthId { 0 }
    {}

    void create_buffers(unsigned int width, unsigned int height);

    void delete_buffers();

    void bind();

    void unbind();

    GLuint get_texture();

private:
    GLuint mFBO = 0;
    GLuint mTexId = 0;
    GLuint mDepthId = 0;
    unsigned int mWidth = 0;
    unsigned int mHeight = 0;
};

#endif //OPENGLMODELEDITOR_FRAMEBUFFERMANAGER_H
