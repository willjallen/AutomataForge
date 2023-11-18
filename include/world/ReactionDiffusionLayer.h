#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Layer.h"
#include "engine/TextureManager.h"
#include "engine/ShaderManager.h"
#include "engine/QuadRenderer.h"

// TODO: This doesn't work for multiple diffusion layers
const std::string REACTION_DIFFUSION_TEXTURE_PREFIX = "REACTION_DIFFUSION";

class ReactionDiffusionLayer : public Layer{
    public: 
        /**
         * @brief Construct a new Diffusion Layer object.
         * TODO: Different diffusion methods
         * 
         * @param diffusionRate 
         */
        ReactionDiffusionLayer(unsigned int width,
                       unsigned int height,
                       float diffusionRate,
                       std::shared_ptr<ShaderManager> shaderManager,
                       std::shared_ptr<TextureManager> textureManager,
                       std::shared_ptr<QuadRenderer> quadRenderer);


        /**
         * @brief Destroy the Diffusion Layer object
         * 
         */
        ~ReactionDiffusionLayer();

        // /**
        //  * @brief Add a temporary source (or sink) to the diffusion layer. Lasts for one update only.
        //  * 
        //  * @param gridX x coordinate of the source/sink
        //  * @param gridY y coordinate of the source/sink
        //  * @param sourceValue The value of the source/sink
        //  */
        // void addTemporarySource(unsigned int gridX, unsigned int gridY, float sourceValue);


        std::string getOutputTexture();

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
};