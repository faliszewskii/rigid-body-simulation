//
// Created by faliszewskii on 18.04.24.
//

#include <vector>
#include "Shader.h"

const std::map<int, std::string> Shader::shaderNames = {
        // Traditional pipeline
        {GL_VERTEX_SHADER, "VERTEX"},
        {GL_TESS_CONTROL_SHADER, "TESS_CTRL"},
        {GL_TESS_EVALUATION_SHADER, "TESS_EVAL"},
        {GL_GEOMETRY_SHADER, "GEOMETRY"},
        {GL_FRAGMENT_SHADER, "FRAGMENT"},
        // Compute
        {GL_COMPUTE_SHADER, "COMPUTE"},
};

Shader::Shader(unsigned int ID) : ID(ID) {}


Shader Shader::createTraditionalShader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) {
    std::map<int, std::string> shaderStages;
    shaderStages.emplace(GL_VERTEX_SHADER, vertexShaderPath);
    shaderStages.emplace(GL_FRAGMENT_SHADER, fragmentShaderPath);
    unsigned int ID = initShader(shaderStages);
    return Shader(ID);
}

Shader Shader::createTraditionalShader(const std::string& vertexShaderPath, const std::string& geometryShaderPath,
                                       const std::string& fragmentShaderPath) {
    std::map<int, std::string> shaderStages;
    shaderStages.emplace(GL_VERTEX_SHADER, vertexShaderPath);
    shaderStages.emplace(GL_GEOMETRY_SHADER, geometryShaderPath);
    shaderStages.emplace(GL_FRAGMENT_SHADER, fragmentShaderPath);
    unsigned int ID = initShader(shaderStages);
    return Shader(ID);
}

Shader Shader::createTraditionalShader(const std::string& vertexShaderPath, const std::string& tesselationControlPath,
                                       const std::string& tesselationEvaluationPath, const std::string& fragmentShaderPath) {
    std::map<int, std::string> shaderStages;
    shaderStages.emplace(GL_VERTEX_SHADER, vertexShaderPath);
    shaderStages.emplace(GL_TESS_CONTROL_SHADER, tesselationControlPath);
    shaderStages.emplace(GL_TESS_EVALUATION_SHADER, tesselationEvaluationPath);
    shaderStages.emplace(GL_FRAGMENT_SHADER, fragmentShaderPath);
    unsigned int ID = initShader(shaderStages);
    return Shader(ID);
}

Shader Shader::createComputeShader(const std::string& computeShaderPath) {
    std::map<int, std::string> shaderStages;
    shaderStages.emplace(GL_COMPUTE_SHADER, computeShaderPath);
    unsigned int ID = initShader(shaderStages);
    return Shader(ID);
}

unsigned int Shader::loadShader(int type, const std::string &path) {
    unsigned int shader;
    std::string code;
    std::ifstream file;
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try{
        file.open(path);
        std::stringstream stream;
        stream << file.rdbuf();
        file.close();
        code = stream.str();
    }
    catch (std::ifstream::failure &e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char *pCode = code.c_str();
    shader = glCreateShader(type);
    glShaderSource(shader, 1, &pCode, nullptr);
    glCompileShader(shader);
    checkCompileErrors(shader, shaderNames.at(type));

    return shader;
}

unsigned int Shader::initShader(const std::map<int, std::string>& shaderSrcs) {
    std::vector<unsigned int> shaders;

    shaders.reserve(shaderSrcs.size());
    for(auto &src : shaderSrcs)
        shaders.push_back(loadShader(src.first, src.second));

    unsigned int ID = glCreateProgram();

    for(auto &shader : shaders)
        glAttachShader(ID, shader);

    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");

    for(auto &shader : shaders)
        glDeleteShader(shader);

    return ID;
}

void Shader::hotReload() const {
    glDeleteProgram(ID);
    initShader(std::map<int, std::string>());
}

void Shader::use() const {
    glUseProgram(ID);
}

void Shader::checkCompileErrors(unsigned int shader, std::string type) {
    int success;
    char infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog
                      << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
