#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include <memory>
#include <string>

#include "engine/TextureManager.h"
#include "engine/ShaderManager.h"
#include "engine/QuadRenderer.h"
#include "world/AutomataLayer.h"

AutomataLayer::AutomataLayer(unsigned int width, unsigned int height, unsigned int numStates, std::string rewriteRule, std::shared_ptr<TextureManager> textureManager, std::shared_ptr<ShaderManager> shaderManager, QuadRenderer &quadRenderer){

	// Set variables
	this->width = width;
	this->height = height;
	this->numStates = numStates;
	this->rewriteRule = rewriteRule;

	this->textureManager = textureManager;
	this->shaderManager = shaderManager;
	this->quadRenderer = quadRenderer;

	// Set up compute shader
	shaderManager->loadComputeShader("game_of_life_compute", "shaders/game_of_life_compute.glsl");

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

void AutomataLayer::render() {

}

	// shaderManager.loadShader("screen_quad", "shaders/screen_quad_vertex.glsl", "shaders/screen_quad_fragment.glsl");

	// Shader* computeShader = shaderManager.getShader("basic_compute");
	// computeShader->use();
	// computeShader->setUniformInt("textureSize", (int)TEXTURE_WIDTH);
	// Shader* screenQuad = shaderManager.getShader("screen_quad");
	// screenQuad->use();
	// screenQuad->setUniformInt("text", (int)0);

		// computeShader->use();
		
		// Divide the dispatch by local workgroup size (10 in this case)
		// glDispatchCompute(TEXTURE_WIDTH / 10, TEXTURE_HEIGHT / 10, 1);

		// make sure writing to image has finished before read
		// glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);

		// render image to quad
		// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// screenQuad->use();
		
		// renderQuad();