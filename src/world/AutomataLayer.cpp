#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <glm\glm.hpp>

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
							 std::shared_ptr<QuadRenderer> quadRenderer){

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

	// TODO: FIX THIS	
	shaderManager->setUniform("game_of_life_compute", "textureSize", glm::ivec2(this->width, this->height));	
	
	// Set up texture
	// Create a buffer for the initial state
	std::vector<float> initialState(this->width * this->height * 4, 0.0f);

	// Populate the buffer with the initial state
	for (unsigned int y = 0; y < this->height; y++) { // Use height here
		for (unsigned int x = 0; x < this->width; x++) { // Use width here
			int index = (y * this->width + x) * 4;
			float value = (rand() % 100 < 50) ? 1.0f : 0.0f; // 50% chance
			// float value = 1.0f;
			initialState[index] = value; // Red channel (use this for the state)
			initialState[index + 1] = value; // Green channel
			initialState[index + 2] = value; // Blue channel
			initialState[index + 3] = 1.0f; // Alpha channel (always 1)
		}
	}

	// Create texture one (input/output)
	// Generate a name
	std::string textureOneName = textureManager->generateTextureName(AUTOMATA_TEXTURE_PREFIX);
	
	// Create texture
	textureManager->createTexture(textureOneName, this->width, this->height, initialState);
	
	// Bind texture to an image unit
	textureManager->bindImageUnit(textureOneName);

	// Bind texture to a texture unit
	textureManager->bindTextureUnit(textureOneName);

	// Create texture two (input/output)
	// Generate a name
	std::string textureTwoName = textureManager->generateTextureName(AUTOMATA_TEXTURE_PREFIX);
	
	// Create texture
	textureManager->createTexture(textureTwoName, this->width, this->height, initialState);
	
	// Bind texture to an image unit
	textureManager->bindImageUnit(textureTwoName);

	// Bind texture to a texture unit
	textureManager->bindTextureUnit(textureTwoName);

	// Binds for texture one	
	// Bind image unit to compute shader imgOutput
	textureManager->bindImageUnitToComputeShader(textureOneName, "game_of_life_compute", "imgInput");

	// Binds for texture two	
	// Bind image unit to compute shader imgOutput
	textureManager->bindImageUnitToComputeShader(textureTwoName, "game_of_life_compute", "imgOutput");

	this->inputTexture = textureOneName;
	this->outputTexture = textureTwoName;

	// Bind texture unit to quad renderer shader uniform sampler2D tex
	// textureManager->bindTextureUnitToGeneralShader(textureTwoName, "screen_quad", "tex");


}

AutomataLayer::~AutomataLayer(){

}

void AutomataLayer::reset(){

}

void AutomataLayer::update(){

	textureManager->bindImageUnitToComputeShader(this->inputTexture, "game_of_life_compute", "imgInput");
	textureManager->bindImageUnitToComputeShader(this->outputTexture, "game_of_life_compute", "imgOutput");

	shaderManager->useShader("game_of_life_compute");
	
	// Divide the dispatch by local workgroup size (10 in this case)
	glDispatchCompute(this->width / 8, this->height / 8, 1);

	// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void AutomataLayer::render() {

	// Bind output texture
	// TODO: I might be skipping frames here	
	textureManager->bindTextureUnitToGeneralShader(this->outputTexture, "screen_quad", "tex");

	// Render image to quad
	this->quadRenderer->render();

	// Swap input and output texture
	std::string swap = this->inputTexture;
	this->inputTexture = outputTexture;
	this->outputTexture = swap;
	
}

