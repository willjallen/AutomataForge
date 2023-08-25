#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Layer.h"
#include "world/DiffusionLayer.h"
#include "engine/TextureManager.h"
#include "engine/ShaderManager.h"
#include "engine/QuadRenderer.h"


const std::string AUTOMATA_TEXTURE_PREFIX = "AUTOMATA";

class AutomataLayer : public Layer{
    public: 
        /**
         * @brief Construct a new Automata Layer object. Initializes a GL texture object the layer will write to.
         * 
         * @param width Width of the layer
         * @param height Height of the layer
         * @param numStates Number of states each cell can be
         * @param rewriteRule TODO: The update rule for the automata
         */
        AutomataLayer(unsigned int width, 
                        unsigned int height,
                        unsigned int numStates,
                        std::string rewriteRule, 
                        std::shared_ptr<ShaderManager> shaderManager,
                        std::shared_ptr<TextureManager> textureManager,
                        std::shared_ptr<QuadRenderer> quadRenderer,
                        std::shared_ptr<DiffusionLayer> diffusionLayer);
        /**
         * @brief Destroy the Automata Layer object
         * 
         */
        ~AutomataLayer();
        
        /**
         * @brief Reset the automata layer 
         * TODO: To what?
         */
        void reset();

        /**
         * @brief Update the automata layer 
         * 
         */
        void update();

        /**
         * @brief Render the automata layer 
         * 
         */
        void render();

    private:
        unsigned int width;
        unsigned int height;
        unsigned int numStates;
        std::string rewriteRule;
        std::shared_ptr<TextureManager> textureManager;
        std::shared_ptr<ShaderManager> shaderManager;
        std::shared_ptr<QuadRenderer> quadRenderer;
        std::shared_ptr<DiffusionLayer> diffusionLayer;

        // TODO: Multiple input textures
        std::string inputTexture;
        std::string outputTexture;


};