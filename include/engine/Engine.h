#pragma once
#include <memory>
#include "engine/Window.h"
#include "engine/ShaderManager.h"
#include "engine/TextureManager.h"
#include "world/World.h"

const unsigned int WINDOW_WIDTH = 1920;
const unsigned int WINDOW_HEIGHT = 1080;

const std::string WINDOW_NAME = "AutomataForge";

class Engine{
    public:
        /**
         * @brief Construct a new Engine object
         * 
         */
        Engine();

        /**
         * @brief Destroy the Engine object
         * 
         */
        ~Engine();

        /**
         * @brief Starts the engine 
         * 
         */
        void start();

        /**
         * @brief Stops the engine 
         * 
         */
        void stop();

        /**
         * @brief Updates the world 
         * 
         */
        void update();

        /**
         * @brief Renders the world 
         * 
         */
        void render();
    
        std::unique_ptr<Window> window;
        std::shared_ptr<ShaderManager> shaderManager;
        std::shared_ptr<TextureManager> textureManager;

        std::unique_ptr<World> world;
    
    private:
        int targetFPS = 60;
};
