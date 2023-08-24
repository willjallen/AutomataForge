#pragma once

#include <string>
#include <unordered_map>
#include "engine/ShaderManager.h"   
#include "engine/Texture.h"

/**
 * @class TextureManager
 * @brief Manages the creation, binding, and deletion of OpenGL textures.
 * 
 * The TextureManager class provides a centralized way to manage textures within an OpenGL application.
 * It helps to efficiently load, unload textures, and ensures that duplicate textures are not created unnecessarily.
 */
class TextureManager {
public:

    /**
     * @brief Construct a new Texture Manager object.
     */
    TextureManager(std::shared_ptr<ShaderManager> shaderManager);

    /**
     * @brief Frees all textures stored in textures map.
     */
    ~TextureManager();


    /**
     * @brief Generates a unique texture name. 
     * @param std::string prefix The prefix for the texture name. Useful for debugging. 
     * @return std::string The texture name.
     */
    std::string generateTextureName(const std::string &prefix);

    /**
     * @brief Creates and stores a new texture with given specifications.
     * @param name Name or identifier for the texture.
     * @param width The width of the texture.
     * @param height The height of the texture.
     * @param data Pointer to the initial data (optional).
     * @return True if the texture was created successfully, false otherwise.
     */
    bool createTexture(const std::string &name, GLsizei width, GLsizei height, std::vector<float>& data);
    
    /**
     * @brief Unloads a texture by name.
     * @param name Name or identifier for the texture.
     */
    void unloadTexture(const std::string &name);


    /**
     * @brief Retrieves a texture by name.
     * @param name Name or identifier for the texture.
     * @return Pointer to the texture
     * @throws std::runtime_error if not found
     */
    Texture* getTexture(const std::string &name);

    /**
     * @brief Gets the texture unit for a texture
     * 
     * @param name The name of the texture
     * @return GLuint The texture unit 
     * @throws std::runtime_error if not found
     */
    GLuint getTextureUnit(const std::string &name);

    /**
     * @brief Gets the image unit for a texture
     * 
     * @param name The name of the texture
     * @return GLuint The image unit
     * @throws std::runtime_error if not found
     */
    GLuint getImageUnit(const std::string &name);


    /**
     * @brief Gets the next available texture unit
     * 
     * @return GLuint The texture unit
     * @throws std::runtime_error if there are no more texture units available 
     */
    GLuint getNextAvailableTextureUnit();

    /**
     * @brief Gets the next available image unit 
     * 
     * @return GLuint The image unit
     * @throws std::runtime_error if there are no more image units available 
     */
    GLuint getNextAvailableImageUnit();


    /**
     * @brief Dynamically binds an existing texture to a texture unit.
     *  A texture unit is a slot where you can bind a texture, making it accessible to shaders. 
     * @param name The name of the texture to bind.
     * @throws std::runtime_error if the texture is not found  
     */
    void bindTextureUnit(const std::string& name);

    /**
     * @brief Dynamically binds an existing texture to an image unit 
     * An image unit is used for reading from and writing to textures in a more flexible way. Particularly useful in compute shaders.
     * @param name The name of the texture to bind
     * @throws std::runtime_error if the texture is not found  
     */
    void bindImageUnit(const std::string& name);

    /**
     * @brief Binds an existing texture unit to a general(vertex/fragment/compute) shader 
     * 
     * @param textureName The texture unit to be bound 
     * @param shaderProgramID The shader to bind the texture unit to
     * @param uniformName The name of the uniform variable in the shader to bind the texture unit to
     * @throws std::runtime_error if the uniform location is not found  
     */
    void TextureManager::bindTextureUnitToGeneralShader(const std::string &textureName, const std::string &shaderProgramName, const std::string &uniformName);

    /**
     * @brief Binds an existing image unit to a compute shader 
     * 
     * @param textureName The texture unit to be bound 
     * @param shaderProgramID The compute shader to bind the texture unit to
     * @param uniformName The name of the uniform variable in the shader to bind the texture unit to
     * @throws std::runtime_error if the uniform location is not found  
     */
    void TextureManager::bindImageUnitToComputeShader(const std::string &textureName, const std::string &shaderProgramName, const std::string &uniformName);
private:

    std::shared_ptr<ShaderManager> shaderManager; 

    GLint maxTextureImageUnits;
    GLint maxImageUnits;

    std::unordered_map<std::string, Texture*> textures;
    std::unordered_map<std::string, GLuint> textureUnits;
    std::unordered_map<std::string, GLuint> imageUnits;
};
