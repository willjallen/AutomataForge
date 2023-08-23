#include "engine/QuadRenderer.h"
#include <GL/glew.h>


QuadRenderer::QuadRenderer(std::shared_ptr<ShaderManager> shaderManager) {

    // Set up variables
    this->shaderManager = shaderManager;

    // Set up shader
	shaderManager->loadShader("screen_quad", "shaders/screen_quad_vertex.glsl", "shaders/screen_quad_fragment.glsl");
    
    // Quad vertices representing positions and texture coordinates
    float quadVertices[] = {
        // positions        // texture Coords
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    };

    // Generate the Vertex Array Object (VAO)
    glGenVertexArrays(1, &this->quadVAO);
    glBindVertexArray(this->quadVAO); // Bind the VAO to store the subsequent state

    // Generate the Vertex Buffer Object (VBO)
    glGenBuffers(1, &this->quadVBO);
    glBindBuffer(GL_ARRAY_BUFFER, this->quadVBO); // Bind the VBO

    // Copy the vertex data into the buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

    // Define the layout of the vertex data (positions and texture coordinates)
    glEnableVertexAttribArray(0); // Enable the first attribute (positions)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0); // Positions

    glEnableVertexAttribArray(1); // Enable the second attribute (texture coordinates)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float))); // Texture Coords

    glBindVertexArray(0); // Unbind the VAO
}

void QuadRenderer::render() {

    this->shaderManager->useShader("screen_quad");

    // Bind the VAO containing the quad's vertex attributes
    glBindVertexArray(this->quadVAO);

    // Draw the quad as a triangle strip
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Unbind the VAO
    glBindVertexArray(0);
}

// unsigned int quadVAO = 0;
// unsigned int quadVBO;
// void renderQuad()
// {
// 	if (quadVAO == 0)
// 	{

// 		// setup plane VAO
// 		glGenVertexArrays(1, &quadVAO);
// 		glGenBuffers(1, &quadVBO);
// 		glBindVertexArray(quadVAO);
// 		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
// 		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
// 		glEnableVertexAttribArray(0);
// 		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
// 		glEnableVertexAttribArray(1);
// 		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
// 	}
// 	glBindVertexArray(quadVAO);
// 	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
// 	glBindVertexArray(0);
// }