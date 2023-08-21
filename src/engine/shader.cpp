#include <iostream>
#include <fstream>
#include <sstream>

#include "engine/Shader.h"

Shader::Shader(){}

Shader::Shader(const std::string &computePath){
    this->computePath = computePath;

    GLuint computeShader = loadShader(computePath, GL_COMPUTE_SHADER);

    this->programID = glCreateProgram();

    glAttachShader(this->programID, computeShader);
    glLinkProgram(this->programID);

    checkCompileErrors(this->programID, "PROGRAM");

    glDeleteShader(computeShader);

}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    this->vertexPath = vertexPath;
    this->fragmentPath = fragmentPath;
    
    GLuint vertexShader = loadShader(vertexPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = loadShader(fragmentPath, GL_FRAGMENT_SHADER);

    this->programID = glCreateProgram();
    glAttachShader(this->programID, vertexShader);
    glAttachShader(this->programID, fragmentShader);
    glLinkProgram(this->programID);

    checkCompileErrors(this->programID, "PROGRAM");


    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(this->programID);
}

void Shader::use() {
    glUseProgram(this->programID);
}

void Shader::setUniform(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(this->programID, name.c_str()), value);
}

void Shader::setUniform(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(this->programID, name.c_str()), value);
}

GLuint Shader::loadShader(const std::string &path, GLenum shaderType) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string shaderCode = buffer.str();
    printf("Shader code: %s", shaderCode.c_str());

    GLuint shaderID = glCreateShader(shaderType);
    const GLchar *source = (const GLchar *)shaderCode.c_str();
    glShaderSource(shaderID, 1, &source, 0);
    glCompileShader(shaderID);

    switch(shaderType){
        case GL_VERTEX_SHADER:
            checkCompileErrors(shaderID, "VERTEX");
            break;
        case GL_FRAGMENT_SHADER:
            checkCompileErrors(shaderID, "FRAGMENT");
            break;
        case GL_COMPUTE_SHADER:
            checkCompileErrors(shaderID, "COMPUTE");
            break;
    }

    return shaderID;
}

void Shader::checkCompileErrors(GLuint shader, const std::string &type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            printf("ERROR::SHADER_COMPILATION_ERROR of type: %s\n%s\nVertex Path: %s\nFragment Path: %s\n", type.c_str(), infoLog, vertexPath.c_str(), fragmentPath.c_str());
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            printf("ERROR::PROGRAM_LINKING_ERROR of type: %s\n%s\nVertex Path: %s\nFragment Path: %s\n", type.c_str(), infoLog, vertexPath.c_str(), fragmentPath.c_str());
        }
    }
}
