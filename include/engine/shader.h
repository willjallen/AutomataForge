#pragma once

#include <string>
#include <GL/glew.h>

class Shader {
public:
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
    void Use();

    /**
     * @brief Sets a float uniform variable in the shader.
     * @param name Name of the uniform variable.
     * @param value Value to be set.
     */
    void SetUniform(const std::string &name, float value);

    /**
     * @brief Sets an integer uniform variable in the shader.
     * @param name Name of the uniform variable.
     * @param value Value to be set.
     */
    void SetUniform(const std::string &name, int value);

private:
    GLuint programID;

    /**
     * @brief Loads and compiles a shader from a file.
     * @param path Path to the shader file.
     * @param shaderType Type of the shader (e.g., GL_VERTEX_SHADER).
     * @return The shader ID.
     */
    GLuint LoadShader(const std::string &path, GLenum shaderType);

    /**
     * @brief Checks for compilation or linking errors.
     * @param shader ID of the shader or program.
     * @param type Type of the error to check ("VERTEX", "FRAGMENT", or "PROGRAM").
     */
    void CheckCompileErrors(GLuint shader, const std::string &type);
};
