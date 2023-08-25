#pragma once
#include <vector>
#include "Layer.h"

// TODO: This doesn't work for multiple diffusion layers
const std::string DIFFUSION_TEXTURE_PREFIX = "DIFFUSION";

class DiffusionLayer : public Layer{
    public: 
        /**
         * @brief Construct a new Diffusion Layer object.
         * TODO: Different diffusion methods
         * 
         * @param diffusionRate 
         */
        DiffusionLayer(unsigned int width,
                       unsigned int height,
                       float diffusionRate,
                       std::shared_ptr<ShaderManager> shaderManager,
                       std::shared_ptr<TextureManager> textureManager,
                       std::shared_ptr<QuadRenderer> quadRenderer);


        /**
         * @brief Destroy the Diffusion Layer object
         * 
         */
        ~DiffusionLayer();

        /**
         * @brief Reset the diffusion layer 
         * 
         */
        void reset();

        /**
         * @brief Update the diffusion layer 
         * 
         */
        void update();

        /**
         * @brief Render the diffusion layer 
         * 
         */
        void render();
    private:
        float diffusionRate;

        std::shared_ptr<ShaderManager> shaderManager;
        std::shared_ptr<TextureManager> textureManager;
        std::shared_ptr<QuadRenderer> quadRenderer;
        
        std::string inputTexture;
        std::string outputTexture;
        std::string sourcesTexture;
};