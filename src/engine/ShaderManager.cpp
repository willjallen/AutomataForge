#include "engine/ShaderManager.h"

ShaderManager::ShaderManager(){
    int max_compute_work_group_count[3];
    int max_compute_work_group_size[3];
    int max_compute_work_group_invocations;

    for (int idx = 0; idx < 3; idx++) {
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_COUNT, idx, &max_compute_work_group_count[idx]);
        glGetIntegeri_v(GL_MAX_COMPUTE_WORK_GROUP_SIZE, idx, &max_compute_work_group_size[idx]);
    }
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &max_compute_work_group_invocations);

    printf("OpenGL Limitations: \n");
    printf("maximum number of work groups in X dimension %d\n", max_compute_work_group_count[0]);
    printf("maximum number of work groups in Y dimension %d\n", max_compute_work_group_count[1]);
    printf("maximum number of work groups in Z dimension %d\n", max_compute_work_group_count[2]);

    printf("maximum size of a work group in X dimension %d\n", max_compute_work_group_size[0]);
    printf("maximum size of a work group in Y dimension %d\n", max_compute_work_group_size[1]);
    printf("maximum size of a work group in Z dimension %d\n", max_compute_work_group_size[2]);

    printf("Number of invocations in a single local work group that may be dispatched to a compute shader %d\n", max_compute_work_group_invocations);
}

ShaderManager::~ShaderManager() {
    for (auto &pair : shaders) {
        delete pair.second;
    }
}

bool ShaderManager::loadShader(const std::string &name, const std::string &vertexPath, const std::string &fragmentPath) {
    Shader* shader = new Shader(vertexPath, fragmentPath);
    this->shaders[name] = shader;
    return true; //TODO: Error checking
}


bool ShaderManager::loadShader(const std::string &name, const std::string &computePath) {
    Shader* shader = new Shader(computePath);
    this->shaders[name] = shader;
    return true; //TODO: Error checking
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
