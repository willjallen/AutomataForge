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

const unsigned int TEXTURE_WIDTH = 1000;
const unsigned int TEXTURE_HEIGHT = 1000;
const unsigned int TEXTURE_SIZE = 1000;

void renderQuad();

int main( int argc, char* args[] )
{

	// Create window & init GL
	Window window("AutomataForge", SCREEN_WIDTH, SCREEN_HEIGHT);

	// Load shader manager
	ShaderManager shaderManager;
	shaderManager.loadShader("screen_quad", "shaders/screen_quad_vertex.glsl", "shaders/screen_quad_fragment.glsl");
	shaderManager.loadShader("basic_compute", "shaders/game_of_life_compute.glsl");

	Shader* computeShader = shaderManager.getShader("basic_compute");
	computeShader->use();
	computeShader->setUniform("textureSize", (int)TEXTURE_SIZE);
	Shader* screenQuad = shaderManager.getShader("screen_quad");
	screenQuad->use();
	screenQuad->setUniform("text", (int)0);


	// Create texture for OpenGL operation
	// -----------------------------------

	// Declare a variable to hold the texture ID
	unsigned int texture;

	// Generate a new texture object, storing the ID in 'texture'
	glGenTextures(1, &texture);

	// Activate the texture unit 0 (GL_TEXTURE0) for subsequent texture commands
	glActiveTexture(GL_TEXTURE0);

	// Bind the texture object to the 2D texture target, making it the active texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// Set the texture wrapping mode for the S (horizontal) coordinate to CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);

	// Set the texture wrapping mode for the T (vertical) coordinate to CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// Set the magnification filter to linear interpolation (smoothing effect when scaling up)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set the minification filter to linear interpolation (smoothing effect when scaling down)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Allocate memory and specify the 2D texture image format (e.g., RGBA32F), size, and data type
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, TEXTURE_WIDTH, TEXTURE_HEIGHT, 0, GL_RGBA, GL_FLOAT, NULL);

	// Bind the texture object to an image unit for reading and writing within a shader
	glBindImageTexture(0, texture, 0, GL_FALSE, 0, GL_READ_WRITE, GL_RGBA32F);

	// (Optional) Re-activate the texture unit 0 and re-bind the texture object to the 2D target
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	// Create a buffer for the initial state
	float* initialState = new float[TEXTURE_WIDTH * TEXTURE_HEIGHT * 4];

	// Populate the buffer with the initial state
	for (int y = 0; y < TEXTURE_HEIGHT; y++) {
		for (int x = 0; x < TEXTURE_WIDTH; x++) {
		int index = (y * TEXTURE_WIDTH + x) * 4;
		float value = (rand() % 100 < 10) ? 1.0f : 0.0f; // 10% chance of being alive
		initialState[index] = value; // Red channel (use this for the state)
		initialState[index + 1] = value; // Green channel
		initialState[index + 2] = value; // Blue channel
		initialState[index + 3] = 1.0f; // Alpha channel (always 1)
		}
	}

	// Bind the texture object to the 2D texture target
	glBindTexture(GL_TEXTURE_2D, texture);

	// Upload the buffer to the texture
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, TEXTURE_WIDTH, TEXTURE_HEIGHT, GL_RGBA, GL_FLOAT, initialState);

	// Clean up the buffer
	delete[] initialState;


	bool running = true;
	
	//Enable text input
	SDL_StartTextInput();

	// Desired frame rate (set to 0 if no limit is desired)
	const int TARGET_FPS = 0;
	const int TARGET_FRAME_DURATION = TARGET_FPS > 0 ? 1000 / TARGET_FPS : 0;

	// Variables to keep track of the frame time and frame count
	Uint64 NOW = SDL_GetPerformanceCounter();
	Uint64 LAST = 0;
	double deltaTime = 0;
	Uint32 frameCount = 0;
	Uint32 timeElapsed = 0;

	// While the application is running
	while (running) {
		LAST = NOW;
		NOW = SDL_GetPerformanceCounter();
		
		// Calculate delta time in milliseconds
		deltaTime = (double)((NOW - LAST) * 1000) / (double)SDL_GetPerformanceFrequency();

		// Increment frame count and elapsed time
		frameCount++;
		timeElapsed += (Uint32)deltaTime;

		// If one second has passed, print the FPS and reset counters
		if (timeElapsed >= 1000) {
			printf("FPS: %d\n", frameCount);
			frameCount = 0;
			timeElapsed = 0;
		}

		computeShader->use();
		computeShader->setUniform("t", (float)SDL_GetTicks64());
		
		// Divide the dispatch by local workgroup size (10 in this case)
		glDispatchCompute(TEXTURE_SIZE / 10, TEXTURE_SIZE / 10, 1);

		// make sure writing to image has finished before read
		glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		// render image to quad
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		screenQuad->use();
		
		renderQuad();

		// Update screen
		window.swapBuffers();
		// Handle events
		running = window.handleEvents();

		// Frame rate limiter
		if (TARGET_FRAME_DURATION > 0) {
			Uint32 frameTime = (Uint32)deltaTime;
			if (frameTime < TARGET_FRAME_DURATION) {
				SDL_Delay(TARGET_FRAME_DURATION - frameTime);
			}
		}
		// Add a delay 
		// SDL_Delay(50);
	}
	
	//Disable text input
	SDL_StopTextInput();
	

	return 0;
}


unsigned int quadVAO = 0;
unsigned int quadVBO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

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
	// ShaderManager shaderManager;
	// shaderManager.loadShader("basic", "shaders/basic_vertex.glsl", "shaders/basic_fragment.glsl");
	// shaderManager.loadShader("basic_compute", "shaders/basic_compute.glsl");

	// // Create full screen quad
	// float vertices[] = {
	// 	0.5f,  0.5f, 0.0f,  // top right
	// 	0.5f, -0.5f, 0.0f,  // bottom right
	// 	-0.5f, -0.5f, 0.0f,  // bottom left
	// 	-0.5f,  0.5f, 0.0f   // top left 
	// };
	// unsigned int indices[] = {  // note that we start from 0!
	// 	0, 1, 3,   // first triangle
	// 	1, 2, 3    // second triangle
	// };  
	// GLuint VAO, VBO, EBO;
	
    // glGenVertexArrays(1, &VAO);
    // glGenBuffers(1, &VBO);
    // glGenBuffers(1, &EBO);
    
	// glBindVertexArray(VAO);

    // glBindBuffer(GL_ARRAY_BUFFER, VBO);
    // glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    // glEnableVertexAttribArray(0);

	// glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);