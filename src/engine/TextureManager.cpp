#include <string>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <stdexcept>
#include <set>
#include <memory>
#include <cstdio>
#include "engine/Texture.h"
#include "engine/TextureManager.h"

TextureManager::TextureManager(std::shared_ptr<ShaderManager> shaderManager) {
    // Set variables
    this->shaderManager = shaderManager;

    // Query hardware limits
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &this->maxTextureImageUnits);
    glGetIntegerv(GL_MAX_IMAGE_UNITS, &this->maxImageUnits);
    printf("Max texture image units:%d", this->maxTextureImageUnits);
    printf("Max texture image units:%d", this->maxImageUnits);
}

TextureManager::~TextureManager() {
    for (auto &pair : textures) {
        try {
            unloadTexture(pair.first);
        } catch (const std::exception& e) {
            printf(e.what());
           // TODO: Handle exception 
        }
    }
}

bool TextureManager::createTexture(const std::string &name, GLsizei width, GLsizei height, std::vector<float>& data) {
    // Check if texture already exists
    auto it = this->textureUnits.find(name);
    if (it != this->textureUnits.end()) {
        throw new std::runtime_error("Texture " + name + " already exists.");
    }
    
    Texture* texture = new Texture(width, height, data);
    this->textures[name] = texture;
    return true; //TODO: Error checking
}

void TextureManager::unloadTexture(const std::string &name){
    Texture* texture = getTexture(name);

    if(texture == nullptr){
        throw new std::runtime_error("Error: Attempted to unload texture " + name + " but it cannot be found");
    }

    // Remove the texture from the all the maps 
    textures.erase(name);
    textureUnits.erase(name);
    imageUnits.erase(name);

    // Delete the Texture object
    delete texture;

}


Texture* TextureManager::getTexture(const std::string &name) {
    auto it = this->textures.find(name);
    if (it != this->textures.end()) {
        return (it->second);
    }

    throw new std::runtime_error("Attempted to get texture " + name + " but texture was not found.");
}

GLuint TextureManager::getTextureUnit(const std::string &name) {
    auto it = this->textureUnits.find(name);
    if (it != this->textureUnits.end()) {
        return (it->second);
    }

    throw new std::runtime_error("Attempted to get texture unit for texture" + name + " but texture unit was not found.");
}


GLuint TextureManager::getImageUnit(const std::string &name) {
    auto it = this->imageUnits.find(name);
    if (it != this->imageUnits.end()) {
        return (it->second);
    }

    throw new std::runtime_error("Attempted to get image unit for texture" + name + " but image unit was not found.");
}




GLuint TextureManager::getNextAvailableTextureUnit() {
    // Create a set of all used texture units
    std::set<GLuint> usedTextureUnits;
    for (const auto &pair : textureUnits) {
        usedTextureUnits.insert(pair.second);
    }

    // Find the smallest available texture unit
    for (GLint i = 0; i < this->maxTextureImageUnits; ++i) {
        if (usedTextureUnits.find(i) == usedTextureUnits.end()) {
            return i; // Return the first unused texture unit
        }
    }

    // If we reach here, we've exhausted all texture units
    throw std::runtime_error("Reached the hardware limit for texture units");
}


GLuint TextureManager::getNextAvailableImageUnit() {
    // Create a set of all used image units
    std::set<GLuint> usedImageUnits;
    for (const auto &pair : imageUnits) {
        usedImageUnits.insert(pair.second);
    }

    // Find the smallest available image unit
    for (GLint i = 0; i < this->maxImageUnits; ++i) {
        if (usedImageUnits.find(i) == usedImageUnits.end()) {
            return i; // Return the first unused image unit
        }
    }

    // If we reach here, we've exhausted all image units
    throw std::runtime_error("Reached the hardware limit for image units");
}


void TextureManager::bindTextureUnit(const std::string &name) {
    // Ensure the texture unit is not already bound
    if (textureUnits.find(name) != textureUnits.end()) {
        throw std::runtime_error("Texture unit for " + name + " is already bound");
    }

    Texture *texture = getTexture(name);

    // Get the next available texture unit
    GLuint textureUnit = getNextAvailableTextureUnit();

    // Store the texture unit in the map
    textureUnits[name] = textureUnit;

    // Activate the selected texture unit
    glActiveTexture(GL_TEXTURE0 + textureUnit);

    // Bind the texture to the active texture unit
    glBindTexture(GL_TEXTURE_2D, texture->getTextureID());
}

void TextureManager::bindImageUnit(const std::string &name) {
    // Ensure the image unit is not already bound
    if (imageUnits.find(name) != imageUnits.end()) {
        throw std::runtime_error("Image unit for " + name + " is already bound");
    }

    Texture *texture = getTexture(name);

    // Get the next available image unit
    GLuint imageUnit = getNextAvailableImageUnit();

    // Store the image unit in the map
    imageUnits[name] = imageUnit;
    GLenum error = glGetError();
    // Bind the texture to the selected image unit
    glBindImageTexture(imageUnit, texture->getTextureID(), 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);
    error = glGetError();
    error = glGetError();

}
// https://computergraphics.stackexchange.com/questions/10066/get-binding-point-of-sampler-image-in-shader

/**
 * NOTE: 
 * glUniform1i and glUniform1iv are the only two functions that may be used to load uniform variables defined as sampler types. 
 * Loading samplers with any other function will result in a GL_INVALID_OPERATION error. 
 * https://docs.gl/gl4/glUniform
 */

void TextureManager::bindTextureUnitToGeneralShader(const std::string &textureName, const std::string &shaderProgramName, const std::string &uniformName) {
    GLuint textureUnit = getTextureUnit(textureName);
    shaderManager->setUniform(shaderProgramName, uniformName, (GLint)textureUnit);
}

void TextureManager::bindImageUnitToComputeShader(const std::string &textureName, const std::string &shaderProgramName, const std::string &uniformName) {
    // Retrieve the image unit associated with the texture name
    GLuint imageUnit = getImageUnit(textureName);
    shaderManager->setUniform(shaderProgramName, uniformName, (GLint)imageUnit);
}
