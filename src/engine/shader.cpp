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

// General templated method (catch-all for unsupported types)
template <typename T>
void Shader::setUniform(const std::string &name, T value) {
    throw std::runtime_error("Uniform type not supported.");
}

// Specialization for float
template <>
void Shader::setUniform<float>(const std::string &name, float value) {
    glUniform1f(getUniformLocation(name), value);
}

// Specialization for int
template <>
void Shader::setUniform<int>(const std::string &name, int value) {
    glUniform1i(getUniformLocation(name), value);
}

// Specialization for glm::vec2
template <>
void Shader::setUniform<glm::vec2>(const std::string &name, glm::vec2 value) {
    glUniform2fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

// Specialization for glm::vec3
template <>
void Shader::setUniform<glm::vec3>(const std::string &name, glm::vec3 value) {
    glUniform3fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

// Specialization for glm::vec4
template <>
void Shader::setUniform<glm::vec4>(const std::string &name, glm::vec4 value) {
    glUniform4fv(getUniformLocation(name), 1, glm::value_ptr(value));
}

// Specialization for glm::mat2
template <>
void Shader::setUniform<glm::mat2>(const std::string &name, glm::mat2 value) {
    glUniformMatrix2fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

// Specialization for glm::mat3
template <>
void Shader::setUniform<glm::mat3>(const std::string &name, glm::mat3 value) {
    glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
}

// Specialization for glm::mat4
template <>
void Shader::setUniform<glm::mat4>(const std::string &name, glm::mat4 value) {
    glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, glm::value_ptr(value));
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
