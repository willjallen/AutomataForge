#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp> 

class Shader {
public:

    /**
     * @brief Construct an empty Shader object
     * 
     */
    Shader();

    /**
     * @brief Constructs the Shader object and compiles compute shader.
     * @param computePath Path to the compute shader file.
     */
    Shader(const std::string &computePath);

    /**
     * @brief Constructs the Shader object and compiles the vertex and fragment shaders.
     * @param vertexPath Path to the vertex shader file.
     * @param fragmentPath Path to the fragment shader file.
     */
    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    /**
     * @brief Destroys the Shader object and deletes the shader program.
     */
    ~Shader();

    /**
     * @brief Activates the shader program for rendering.
     */
    void use();

    /**
     * @brief General templated method for setting uniform values (catch-all for unsupported types).
     * @param name Name of the uniform variable.
     * @param value Value to set.
     * @throw std::runtime_error Throws an error if the type is not supported.
     */
    template <typename T>
    void setUniform(const std::string &name, T value);

    /**
     * @brief Sets a float uniform value in the shader program.
     * @param name Name of the uniform variable.
     * @param value Float value to set.
     */
    template <>
    void setUniform<float>(const std::string &name, float value);

    /**
     * @brief Sets an int uniform value in the shader program.
     * @param name Name of the uniform variable.
     * @param value Int value to set.
     */
    template <>
    void setUniform<int>(const std::string &name, int value);

    /**
     * @brief Sets a glm::vec2 uniform value in the shader program.
     * @param name Name of the uniform variable.
     * @param value glm::vec2 value to set.
     */
    template <>
    void setUniform<glm::vec2>(const std::string &name, glm::vec2 value);

    /**
     * @brief Sets a glm::vec3 uniform value in the shader program.
     * @param name Name of the uniform variable.
     * @param value glm::vec3 value to set.
     */
    template <>
    void setUniform<glm::vec3>(const std::string &name, glm::vec3 value);

    /**
     * @brief Sets a glm::vec4 uniform value in the shader program.
     * @param name Name of the uniform variable.
     * @param value glm::vec4 value to set.
     */
    template <>
    void setUniform<glm::vec4>(const std::string &name, glm::vec4 value);

    /**
     * @brief Sets a glm::mat2 uniform value in the shader program.
     * @param name Name of the uniform variable.
     * @param value glm::mat2 value to set.
     */
    template <>
    void setUniform<glm::mat2>(const std::string &name, glm::mat2 value);

    /**
     * @brief Sets a glm::mat3 uniform value in the shader program.
     * @param name Name of the uniform variable.
     * @param value glm::mat3 value to set.
     */
    template <>
    void setUniform<glm::mat3>(const std::string &name, glm::mat3 value);

    /**
     * @brief Sets a glm::mat4 uniform value in the shader program.
     * @param name Name of the uniform variable.
     * @param value glm::mat4 value to set.
     */
    template <>
    void setUniform<glm::mat4>(const std::string &name, glm::mat4 value);


private:
    GLuint programID;
    std::string vertexPath;
    std::string fragmentPath;
    std::string computePath;

    /**
     * @brief Loads and compiles a shader from a file.
     * @param path Path to the shader file.
     * @param shaderType Type of the shader (e.g., GL_VERTEX_SHADER).
     * @return The shader ID.
     */
    GLuint loadShader(const std::string &path, GLenum shaderType);

    /**
     * @brief Checks for compilation or linking errors.
     * @param shader ID of the shader or program.
     * @param type Type of the error to check ("VERTEX", "FRAGMENT", or "PROGRAM").
     */
    void checkCompileErrors(GLuint shader, const std::string &type);
};
