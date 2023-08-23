#pragma once

#include <string>
#include <unordered_map>
#include <stdexcept>
#include <iostream>
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
     * @brief Loads and stores a compute shader program.
     * @param name Name or identifier for the compute shader program.
     * @param computePath Path to the compute shader file.
     * @return True if the shader was loaded successfully, false otherwise.
     */
    bool loadComputeShader(const std::string &name, const std::string &computePath);
    
    /**
     * @brief Retrieves a shader program by name.
     * @param name Name or identifier for the shader program.
     * @return Pointer to the shader program, or nullptr if not found.
     */
    Shader* getShader(const std::string &name);
    
    /**
     * @brief Activates a shader program by name.
     * @param name Name or identifier for the shader program.
     */
    void useShader(const std::string &name);

    /**
     * @brief Get the the location of the binding point of a uniform within a shader program.
     * 
     * @param shaderProgramName The name of the shader program. 
     * @param uniformName The name of the uniform variable within the shader program.
     * @return GLint The binding location of the uniform variable.
     */
    GLint getUniformBindingPoint(const std::string& shaderProgramName, const std::string& uniformName);
    
    /**
     * @brief Templated method to set a uniform value in a specific shader.
     * @tparam T Type of the value to set.
     * @param shaderName Name of the shader where the uniform is located.
     * @param uniformName Name of the uniform variable.
     * @param value Value to set.
     * @return true if the uniform was successfully set, otherwise false.
     * @throw std::runtime_error Throws an error if the shader or uniform location does not exist, or if an OpenGL error occurs.
     */
    template <typename T>
    bool setUniform(const std::string& shaderName, const std::string& uniformName, T value) {
        Shader* shader = getShader(shaderName); // Exception will be thrown if shader does not exist
        GLenum error = glGetError();
        // Activate the shader
        shader->use();
        error = glGetError();
        // Use appropriate method in Shader class based on the type T
        shader->setUniform(uniformName, value);
        // Check for OpenGL errors
        error = glGetError();
        if (error != GL_NO_ERROR) {
            std::cout << std::to_string(error) << std::endl;
            throw std::runtime_error("OpenGL error setting uniform '" + uniformName + "' in shader '" + shaderName + "'. GL Error: " + std::to_string(error));
        }

        return true;
    }

private:
    std::unordered_map<std::string, Shader*> shaders;
};
