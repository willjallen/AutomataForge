#pragma once

#include <GL/glew.h>
#include <vector>

class Texture {
public:

    /**
     * @brief Construction of default Texture objects are prohibited.
     */
    Texture() = delete;

    /**
     * @brief Construct and initialize a Texture object with given specifications.
     * @param textureUnit Automatically assigned texture unit from the TextureManager
     * @param width The width of the texture.
     * @param height The height of the texture.
     * @param data Pointer to the initial data (optional).
     */
    Texture(GLsizei width, GLsizei height, const std::vector<float> data);

    /**
     * @brief Destroys the Texture object and deletes the texture.
     */
    ~Texture();


    /**
     * @brief Update the content of the texture.
     * @param data Pointer to the new data.
     */
    void setData(const std::vector<float>& data);

    /**
     * @brief Get the Texture ID
     * 
     * @return GLuint 
     */
    GLuint getTextureID();

private:
    GLuint textureUnit;
    GLuint textureID;
    GLsizei width;
    GLsizei height;
};
