#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <stdio.h>
#include <string>

#include "engine/Window.h"
#include "engine/Shader.h"
#include "engine/ShaderManager.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main( int argc, char* args[] )
{

	// Create window & init GL
	Window window("AutomataForge", SCREEN_WIDTH, SCREEN_HEIGHT);

	// // Init GOL state
	// const int gridSize = 64;
	// float initialData[gridSize * gridSize];
	// for (int i = 0; i < gridSize * gridSize; i++) {
	// 	initialData[i] = (rand() % 2) ? 1.0f : 0.0f;
	// }

	// GLuint textureID;
	// glGenTextures(1, &textureID);
	// glBindTexture(GL_TEXTURE_2D, textureID);
	// glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, gridSize, gridSize, 0, GL_RED, GL_FLOAT, initialData);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


	// Load GOL shader
	ShaderManager shaderManager;
	shaderManager.loadShader("basic", "shaders/basic_vertex.glsl", "shaders/basic_fragment.glsl");


	// Create full screen quad
	float vertices[] = {
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	unsigned int indices[] = {  // note that we start from 0!
		0, 1, 3,   // first triangle
		1, 2, 3    // second triangle
	};  
	GLuint VAO, VBO, EBO;
	
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    
	glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

	// glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);


	bool running = true;
	
	//Enable text input
	SDL_StartTextInput();

	//While application is running
	while(running)
	{
		// Handle events
		running = window.handleEvents();
		
		// Clear the screen
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// Activate the Game of Life shader
		shaderManager.useShader("basic");

		// Bind the texture representing the current game state
		// glActiveTexture(GL_TEXTURE0);
		// glBindTexture(GL_TEXTURE_2D, textureID);
		// shaderManager.getShader("game_of_life")->setUniform("uTexture", 0);

		// Render the full-screen quad
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		// Update screen
		window.swapBuffers();

		// Add a delay 
		SDL_Delay(50);
	}
	
	//Disable text input
	SDL_StopTextInput();
	

	return 0;
}

