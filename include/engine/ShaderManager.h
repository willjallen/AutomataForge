#pragma once

#include <string>
#include <unordered_map>

#include "engine/Shader.h"

class ShaderManager {
public:

    /**
     * @brief Construct a new Shader Manager object. Performs checks of GL capabilities for compute shaders.
     * 
     */
    ShaderManager();

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
     * @brief Loads and stores a shader program.
     * @param name Name or identifier for the shader program.
     * @param computePath Path to the compute shader file.
     * @return True if the shader was loaded successfully, false otherwise.
     */
    bool loadShader(const std::string &name, const std::string &computePath);
    
    /**
     * @brief Retrieves a shader program by name.
     * @param name Name or identifier for the shader program.
     * @return Pointer to the shader program, or nullptr if not found.
     */
    Shader* getShader(const std::string &name);

private:
    std::unordered_map<std::string, Shader*> shaders;
};
