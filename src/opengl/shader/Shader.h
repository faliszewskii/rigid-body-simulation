//
// Created by faliszewskii on 18.04.24.
//

#ifndef PUMAROBOT_SHADER_H
#define PUMAROBOT_SHADER_H

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #include "../../../dep/glad/glad_glfw.h"
#else
    #include "../../glew_glfw.h"
#endif
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <utility>
#include <variant>
#include <stack>
#include <map>
#include <memory>
#include "../../utils/VariantUtil.h"

using ShaderType = std::variant<bool, int, float, glm::vec3, glm::vec4, glm::mat4, glm::vec3*>;

class Shader {
public:
    static Shader createTraditionalShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath);
    static Shader createTraditionalShader(const std::string& vertexShaderPath, const std::string& geometryShaderPath, const std::string& fragmentShaderPath);
    static Shader createTraditionalShader(const std::string& vertexShaderPath, const std::string& tesselationControlPath, const std::string& tesselationEvaluationPath, const std::string& fragmentShaderPath);

    static Shader createComputeShader(const std::string& computeShaderPath);

    // Activate the shader
    void use() const;
    // Hot-reload the shader
    void hotReload() const;

    // TODO Reroll and refactor to use templates maybe. How to make sure that user will not assign int to a float by accident?
    // Make sure you pass the correct type to the function! For example value "42" will be treated as int even if the uniform is float. Use "42.f" instead.
    void setUniform(const std::string &name, ShaderType &&value, unsigned long size = 1) const {
        std::visit(overloaded{
                [&](bool &v) { glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) v); },
                [&](int &v) { glUniform1i(glGetUniformLocation(ID, name.c_str()), v); },
                [&](float &v) { glUniform1f(glGetUniformLocation(ID, name.c_str()), v); },
                [&](glm::vec3 &v) { glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &v[0]); },
                [&](glm::vec4 &v) { glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &v[0]); },
                [&](glm::mat4 &v) { glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &v[0][0]); },
                [&](glm::vec3* &v) {
                    auto *buffer = new float[size*3];
                    for(int i = 0; i < size; i++) {
                        buffer[i*3+0] = v[i].x;
                        buffer[i*3+1] = v[i].y;
                        buffer[i*3+2] = v[i].z;
                    }
                    glUniform3fv(glGetUniformLocation(ID, name.c_str()), GLsizei(size), buffer);
                }
        }, value);
    };

private:
    explicit Shader(unsigned int ID);

    unsigned int ID;

    const static std::map<int, std::string> shaderNames;

    static unsigned int loadShader(int type, const std::string &path);

    static unsigned int initShader(const std::map<int, std::string>& shaderSrcs);

    static void checkCompileErrors(unsigned int shader, std::string type);
};


#endif //PUMAROBOT_SHADER_H
