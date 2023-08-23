#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>

#include <memory>
#include <string>

#include "engine/TextureManager.h"
#include "engine/ShaderManager.h"
#include "engine/QuadRenderer.h"
#include "world/AutomataLayer.h"

AutomataLayer::AutomataLayer(unsigned int width, 
							 unsigned int height, 
							 unsigned int numStates, 
							 std::string rewriteRule, 
							 std::shared_ptr<ShaderManager> shaderManager, 
							 std::shared_ptr<TextureManager> textureManager, 
							 std::shared_ptr<QuadRenderer> quadRenderer) : quadRenderer(quadRenderer){

	// Set variables
	this->width = width;
	this->height = height;
	this->numStates = numStates;
	this->rewriteRule = rewriteRule;

	this->textureManager = textureManager;
	this->shaderManager = shaderManager;
	this->quadRenderer = quadRenderer;

	GLenum error = glGetError();
	// Set up compute shader
	shaderManager->loadComputeShader("game_of_life_compute", "shaders/game_of_life_compute.glsl");
	
	shaderManager->setUniform("game_of_life_compute", "textureSize", 600);	
	
	// Set up texture
	// Create a buffer for the initial state
	std::vector<float> initialState(this->width * this->height * 4, 0.0f);

	// Populate the buffer with the initial state
	for (unsigned int y = 0; y < 600; y++) { // Use height here
		for (unsigned int x = 0; x < 600; x++) { // Use width here
			int index = (y * 600 + x) * 4;
			// float value = (rand() % 100 < 1) ? 1.0f : 0.0f; // You mentioned a 10% chance, but this line gives a 1% chance
			float value = 1.0f;
			initialState[index] = value; // Red channel (use this for the state)
			initialState[index + 1] = value; // Green channel
			initialState[index + 2] = value; // Blue channel
			initialState[index + 3] = 1.0f; // Alpha channel (always 1)
		}
	}

	// Create texture
	textureManager->createTexture(TEXTURE_NAME, this->width, this->height, initialState);
	
	// Bind texture to an image unit
	textureManager->bindImageUnit(TEXTURE_NAME);
	
	// Bind image unit to compute shader imgOutput
	textureManager->bindImageUnitToComputeShader(TEXTURE_NAME, "game_of_life_compute", "imgOutput");

	// Bind texture to a texture unit
	textureManager->bindTextureUnit(TEXTURE_NAME);

	// Bind texture unit to quad renderer shader uniform sampler2D tex
	textureManager->bindTextureUnitToGeneralShader(TEXTURE_NAME, "screen_quad", "tex");

}

AutomataLayer::~AutomataLayer(){

}

void AutomataLayer::reset(){

}

void AutomataLayer::update(){
	shaderManager->useShader("game_of_life_compute");
	
	// Divide the dispatch by local workgroup size (10 in this case)
	glDispatchCompute(this->width / 10, this->height / 10, 1);

	// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void AutomataLayer::render() {

		// render image to quad
		
		this->quadRenderer->render();
		
}

