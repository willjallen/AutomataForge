#pragma once
#include <vector>
#include <memory>
#include "engine/TextureManager.h"
#include "engine/ShaderManager.h"
#include "layer.h"

class World{

    public:
        World(std::shared_ptr<ShaderManager> shaderManager, std::shared_ptr<TextureManager> textureManager);
        void update();

    private:
        std::vector<Layer> layers; 

        std::shared_ptr<TextureManager> textureManager;
        std::shared_ptr<ShaderManager> shaderManager;

};