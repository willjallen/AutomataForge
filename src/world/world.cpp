#include <memory>

#include "world/World.h"
#include "engine/TextureManager.h"
#include "engine/ShaderManager.h"

World::World(std::shared_ptr<ShaderManager> shaderManager, std::shared_ptr<TextureManager> textureManager){
    this->textureManager = textureManager;
    this->shaderManager = shaderManager;
}





