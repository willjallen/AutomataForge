#pragma once
#include <memory>
#include <GL/glew.h>
#include "engine/ShaderManager.h"

/**
 * @class QuadRenderer
 * @brief Responsible for rendering a full-screen quad.
 *
 * This class encapsulates the functionality needed to create and render
 * a full-screen quad. It is commonly used for post-processing effects
 * or rendering textures to the entire screen.
 */
class QuadRenderer {
    public:
        /**
         * @brief Constructor that initializes the quad.
         *
         * Creates the Vertex Array Object and Vertex Buffer Object
         * and sets up the necessary OpenGL state to render the quad.
         * Also sets up the quad shader.
         * @param shaderManager The shader manager.
         */
        QuadRenderer(std::shared_ptr<ShaderManager> shaderManager);

        /**
         * @brief Renders the full-screen quad.
         *
         * Binds the Vertex Array Object and issues the draw call
         * to render the quad. Should be called with the appropriate
         * shader already bound.
         */
        void render();

    private:

        std::shared_ptr<ShaderManager> shaderManager;

        GLuint quadVAO; // Vertex Array Object for the quad.
        GLuint quadVBO; // Vertex Buffer Object for the quad vertices.
};