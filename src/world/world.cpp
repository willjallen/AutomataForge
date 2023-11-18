#include <memory>

#include "world/World.h"
#include "world/AutomataLayer.h"
#include "world/ReactionDiffusionLayer.h"
#include "world/DiffusionLayer.h"
#include "engine/TextureManager.h"
#include "engine/ShaderManager.h"

World::World(unsigned int width, 
             unsigned int height, 
            std::shared_ptr<ShaderManager> shaderManager, 
            std::shared_ptr<TextureManager> textureManager){
    // Set variables
    this->width = width;
    this->height = height;
    this->textureManager = textureManager;
    this->shaderManager = shaderManager;

    // Set up quad renderer for layers
    this->quadRenderer = std::make_shared<QuadRenderer>(this->shaderManager);

    // Set up layers

    std::shared_ptr<ReactionDiffusionLayer> reactionDiffusionLayer = 
        std::make_shared<ReactionDiffusionLayer>(this->width, 
                                        this->height, 
                                        1.0f,
                                        this->shaderManager, 
                                        this->textureManager, 
                                        this->quadRenderer);



    // std::shared_ptr<DiffusionLayer> diffusionLayer = 
    //     std::make_shared<DiffusionLayer>(this->width, 
    //                                     this->height, 
    //                                     1.0f,
    //                                     this->shaderManager, 
    //                                     this->textureManager, 
    //                                     this->quadRenderer);

    // std::shared_ptr<AutomataLayer> automataLayer = 
    //     std::make_shared<AutomataLayer>(this->width, 
    //                                     this->height, 
    //                                     (unsigned int)1,
    //                                     std::string(""), 
    //                                     this->shaderManager, 
    //                                     this->textureManager, 
    //                                     this->quadRenderer,
    //                                     diffusionLayer);

    this->layers.push_back(reactionDiffusionLayer);
    // this->layers.push_back(diffusionLayer);
    // this->layers.push_back(automataLayer);



}

void World::update(){
    for(auto &layer : layers){
        layer->update();
    }
}

void World::render(){
    for(auto &layer : layers){
        layer->render();
    }
}





