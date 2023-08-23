#pragma once
#include <vector>
#include <memory>
#include "engine/TextureManager.h"
#include "engine/ShaderManager.h"
#include "layer.h"

class World{

    public:
        /**
         * @brief Construct a new World object
         * 
         * The world contains all the individual simulation layers.
         * 
         * @param width Width of the world 
         * @param height Height of the world
         * @param shaderManager The shader manager to use
         * @param textureManager The texture manager to use
         */
        World(unsigned int width, 
              unsigned int height, 
              std::shared_ptr<ShaderManager> shaderManager, 
              std::shared_ptr<TextureManager> textureManager);
        
        /**
         * @brief Update the world in the order in which layers were added to the layers vector 
         * 
         */
        void update();
        
        /**
         * @brief Render the world in the order in which layers were added to the layers vector 
         * 
         */
        void render();

    private:
        unsigned int width;
        unsigned int height;

        std::vector<std::shared_ptr<Layer>> layers;

        std::shared_ptr<TextureManager> textureManager;
        std::shared_ptr<ShaderManager> shaderManager;

        std::shared_ptr<QuadRenderer> quadRenderer;

};