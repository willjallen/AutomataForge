#include <memory>

#include "world/World.h"
#include "world/AutomataLayer.h"
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
    this->layers.push_back(
        std::make_shared<AutomataLayer>(this->width, 
                                        this->height, 
                                        (unsigned int)1,
                                        std::string(""), 
                                        this->shaderManager, 
                                        this->textureManager, 
                                        this->quadRenderer));

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





