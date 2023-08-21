#pragma once

#include <string>
#include <unordered_map>

#include "engine/Shader.h"

class ShaderManager {
public:

    ShaderManager() {};

    /**
     * @brief Frees all shaders stored in shaders map
     * 
     */
    ~ShaderManager();

    /**
     * @brief Loads and stores a shader program.
     * @param name Name or identifier for the shader program.
     * @param vertexPath Path to the vertex shader file.
     * @param fragmentPath Path to the fragment shader file.
     * @return True if the shader was loaded successfully, false otherwise.
     */
    bool loadShader(const std::string &name, const std::string &vertexPath, const std::string &fragmentPath);

    /**
     * @brief Retrieves a shader program by name.
     * @param name Name or identifier for the shader program.
     * @return Pointer to the shader program, or nullptr if not found.
     */
    Shader* getShader(const std::string &name);

    /**
     * @brief Sets the specified shader as the active shader for rendering.
     * @param name Name or identifier for the shader program.
     */
    void useShader(const std::string &name);

private:
    std::unordered_map<std::string, Shader*> shaders;
};
