#include "engine/shader.h"
#include <iostream>
#include <fstream>
#include <sstream>

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) {
    GLuint vertexShader = LoadShader(vertexPath, GL_VERTEX_SHADER);
    GLuint fragmentShader = LoadShader(fragmentPath, GL_FRAGMENT_SHADER);

    programID = glCreateProgram();
    glAttachShader(programID, vertexShader);
    glAttachShader(programID, fragmentShader);
    glLinkProgram(programID);

    CheckCompileErrors(programID, "PROGRAM");

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    glDeleteProgram(programID);
}

void Shader::Use() {
    glUseProgram(programID);
}

void Shader::SetUniform(const std::string &name, float value) {
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void Shader::SetUniform(const std::string &name, int value) {
    glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

GLuint Shader::LoadShader(const std::string &path, GLenum shaderType) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string shaderCode = buffer.str();

    GLuint shaderID = glCreateShader(shaderType);
    const char *codePtr = shaderCode.c_str();
    glShaderSource(shaderID, 1, &codePtr, nullptr);
    glCompileShader(shaderID);

    CheckCompileErrors(shaderID, shaderType == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT");

    return shaderID;
}

void Shader::CheckCompileErrors(GLuint shader, const std::string &type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    } else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n";
        }
    }
}
