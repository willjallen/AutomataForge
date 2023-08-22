#include <memory>
#include <string>
#include "engine/Engine.h"


Engine::Engine(){

    // Construct the window
	this->window = std::make_unique<Window>("AutomataForge", WINDOW_WIDTH, WINDOW_HEIGHT);

	// Start shader manager
	this->shaderManager = std::make_shared<ShaderManager>();

    // Start texture manager
    this->textureManager = std::make_shared<TextureManager>();
    
    // Create the world
    this->world = std::make_unique<World>(this->shaderManager, this->textureManager);

}

Engine::~Engine(){

}

void Engine::start(){
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



		// Update screen
		this->window->swapBuffers();
		// Handle events
		running = this->window->handleEvents();

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
}

void Engine::stop(){

}

void Engine::render(){

}



	



// unsigned int quadVAO = 0;
// unsigned int quadVBO;
// void renderQuad()
// {
// 	if (quadVAO == 0)
// 	{
// 		float quadVertices[] = {
// 			// positions        // texture Coords
// 			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
// 			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
// 			 1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
// 			 1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
// 		};
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