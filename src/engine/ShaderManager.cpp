#include "engine/ShaderManager.h"

ShaderManager::~ShaderManager() {
    for (auto &pair : shaders) {
        delete pair.second;
    }
}

bool ShaderManager::loadShader(const std::string &name, const std::string &vertexPath, const std::string &fragmentPath) {
    Shader* shader = new Shader(vertexPath, fragmentPath);
    this->shaders[name] = shader;
    return true; //TODO Error checking
}

Shader* ShaderManager::getShader(const std::string &name) {
    auto it = this->shaders.find(name);
    if (it != this->shaders.end()) {
        return (it->second);
    }
    return nullptr;
}

void ShaderManager::useShader(const std::string &name) {
    Shader* shader = getShader(name);
    if (shader) {
        shader->use();
    }
}
