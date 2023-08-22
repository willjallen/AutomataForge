#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include <string>

#include "world/AutomataLayer.h"

AutomataLayer::AutomataLayer(unsigned int width, unsigned int height, unsigned int numStates, std::string rewriteRule){


	// // Create a buffer for the initial state
	// float* initialState = new float[TEXTURE_WIDTH * TEXTURE_HEIGHT * 4];

	// // Populate the buffer with the initial state
	// for (int y = 0; y < TEXTURE_HEIGHT; y++) {
	// 	for (int x = 0; x < TEXTURE_WIDTH; x++) {
	// 	int index = (y * TEXTURE_WIDTH + x) * 4;
	// 	float value = (rand() % 100 < 1) ? 1.0f : 0.0f; // 10% chance of being alive
	// 	// float value = 1.0f;
	// 	initialState[index] = value; // Red channel (use this for the state)
	// 	initialState[index + 1] = value; // Green channel
	// 	initialState[index + 2] = value; // Blue channel
	// 	initialState[index + 3] = 1.0f; // Alpha channel (always 1)
	// 	}
	// }

}

AutomataLayer::~AutomataLayer(){

}

void AutomataLayer::reset(){

}

void AutomataLayer::update(){

}

void AutomataLayer::render(){

}