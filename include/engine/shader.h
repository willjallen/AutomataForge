#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/glm.hpp> 
#include <stdexcept>
#include <glm/gtc/type_ptr.hpp>

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
     * @brief Activates the shader program.
     */
    void use();

    /**
     * @brief Get the binding point of a uniform within the shader program.
     * @param uniformName The name of the uniform variable. 
     * @return GLint The location of the uniform variable, or -1 if the uniform variable does not exist
     * @throws std::runtime_error if the uniform variable is not found.
     */
    GLint getUniformBindingPoint(const std::string& uniformName);
    
    /**
     * @brief Retrieves the location of a uniform variable within the shader program.
     * @param name The name of the uniform variable.
     * @return GLint The location of the uniform variable, or -1 if the uniform variable does not exist.
     * @throws std::runtime_error if the uniform variable is not found.
     */
    GLint getUniformLocation(const std::string &uniformName) const;
    
    /** @brief General template method for setting uniform values.
     *  @details This function is a catch-all for unsupported types and will throw a runtime error.
     *  @param uniformName The name of the uniform variable.
     *  @param value The value to be set.
     *  @throws std::runtime_error if the uniform type is not supported.
     */
    template <typename T>
    void setUniform(const std::string &uniformName, T value) {
        throw std::runtime_error("Uniform type not supported.");
    }

    /** @brief Specialization for float.
     *  @param uniformName The name of the uniform variable.
     *  @param value The float value to be set.
     */
    template <>
    void setUniform<float>(const std::string &uniformName, float value) {
        glUniform1f(getUniformLocation(uniformName), value);
    }

    /** @brief Specialization for int.
     *  @param uniformName The name of the uniform variable.
     *  @param value The int value to be set.
     */
    template <>
    void setUniform<int>(const std::string &uniformName, int value) {
        glUniform1i(getUniformLocation(uniformName), value);
    }

    /** @brief Specialization for unsigned int.
     *  @param uniformName The name of the uniform variable.
     *  @param value The unsigned int value to be set.
     */
    template <>
    void setUniform<unsigned int>(const std::string &uniformName, unsigned int value) {
        glUniform1ui(getUniformLocation(uniformName), value);
    }

    /** @brief Specialization for glm::vec2.
     *  @param uniformName The name of the uniform variable.
     *  @param value The glm::vec2 value to be set.
     */
    template <>
    void setUniform<glm::vec2>(const std::string &uniformName, glm::vec2 value) {
        glUniform2fv(getUniformLocation(uniformName), 1, glm::value_ptr(value));
    }

    /** @brief Specialization for glm::vec3.
     *  @param uniformName The name of the uniform variable.
     *  @param value The glm::vec3 value to be set.
     */
    template <>
    void setUniform<glm::vec3>(const std::string &uniformName, glm::vec3 value) {
        glUniform3fv(getUniformLocation(uniformName), 1, glm::value_ptr(value));
    }

    /** @brief Specialization for glm::vec4.
     *  @param uniformName The name of the uniform variable.
     *  @param value The glm::vec4 value to be set.
     */
    template <>
    void setUniform<glm::vec4>(const std::string &uniformName, glm::vec4 value) {
        glUniform4fv(getUniformLocation(uniformName), 1, glm::value_ptr(value));
    }

    /** @brief Specialization for glm::mat2.
     *  @param uniformName The name of the uniform variable.
     *  @param value The glm::mat2 value to be set.
     */
    template <>
    void setUniform<glm::mat2>(const std::string &uniformName, glm::mat2 value) {
        glUniformMatrix2fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(value));
    }

    /** @brief Specialization for glm::mat3.
     *  @param uniformName The name of the uniform variable.
     *  @param value The glm::mat3 value to be set.
     */
    template <>
    void setUniform<glm::mat3>(const std::string &uniformName, glm::mat3 value) {
        glUniformMatrix3fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(value));
    }

    /** @brief Specialization for glm::mat4.
     *  @param uniformName The name of the uniform variable.
     *  @param value The glm::mat4 value to be set.
     */
    template <>
    void setUniform<glm::mat4>(const std::string &uniformName, glm::mat4 value) {
        glUniformMatrix4fv(getUniformLocation(uniformName), 1, GL_FALSE, glm::value_ptr(value));
    }



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
