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

        // /**
        //  * @brief Add a temporary source (or sink) to the diffusion layer. Lasts for one update only.
        //  * 
        //  * @param gridX x coordinate of the source/sink
        //  * @param gridY y coordinate of the source/sink
        //  * @param sourceValue The value of the source/sink
        //  */
        // void addTemporarySource(unsigned int gridX, unsigned int gridY, float sourceValue);


        /**
         * @brief Add a permanent source (or sink) to the diffusion layer. Lasts until removed.
         * 
         * @param gridX x coordinate of the source/sink
         * @param gridY y coordinate of the source/sink
         * @param sourceValue The value of the source/sink
         */
        void addPermanentSource(unsigned int gridX, unsigned int gridY, float sourceValue);

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
        std::string sourcesTexture;
};