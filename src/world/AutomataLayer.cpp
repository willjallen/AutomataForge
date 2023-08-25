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
							 std::shared_ptr<QuadRenderer> quadRenderer,
							 std::shared_ptr<DiffusionLayer> diffusionLayer){

	// Set variables
	this->width = width;
	this->height = height;
	this->numStates = numStates;
	this->rewriteRule = rewriteRule;

	this->textureManager = textureManager;
	this->shaderManager = shaderManager;
	this->quadRenderer = quadRenderer;
	this->diffusionLayer = diffusionLayer;

	GLenum error = glGetError();
	// Set up compute shader
	shaderManager->loadComputeShader("diffusion_coupled_game_of_life", "shaders/diffusion_coupled_game_of_life_compute.glsl");

    // Set up render shader (passthrough)
	shaderManager->loadShader("passthrough_quad", "shaders/passthrough_quad_vertex.glsl", "shaders/passthrough_quad_fragment.glsl");
    
	shaderManager->setUniform("diffusion_coupled_game_of_life", "textureSize", glm::ivec2(this->width, this->height));	

	// Diffusion layer coupling
	shaderManager->setUniform("diffusion_coupled_game_of_life", "energyFlux", 20.0f);	

	// Set up texture
	// Create a buffer for the initial state
	std::vector<float> initialState(this->width * this->height * 4, 0.0f);

	// Populate the buffer with the initial state
	for (unsigned int y = 0; y < this->height; y++) { // Use height here
		for (unsigned int x = 0; x < this->width; x++) { // Use width here
			int index = (y * this->width + x) * 4;
			float value = (rand() % 100 < 40) ? 1.0f : 0.0f; // 0.1% chance
			// float value = 1.0f;
			initialState[index] = value; // Red channel (use this for the state)
			initialState[index + 1] = value; // Green channel
			initialState[index + 2] = value; // Blue channel
			initialState[index + 3] = 0.5f; // Alpha channel (always 1)
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
	textureManager->bindImageUnitToComputeShader(textureOneName, "diffusion_coupled_game_of_life", "imgInput");

	// Binds for texture two	
	// Bind image unit to compute shader imgOutput
	textureManager->bindImageUnitToComputeShader(textureTwoName, "diffusion_coupled_game_of_life", "imgOutput");

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

	textureManager->bindImageUnitToComputeShader(this->inputTexture, "diffusion_coupled_game_of_life", "imgInput");
	textureManager->bindImageUnitToComputeShader(this->outputTexture, "diffusion_coupled_game_of_life", "imgOutput");
	
	textureManager->bindImageUnitToComputeShader(this->diffusionLayer->getOutputTexture(), "diffusion_coupled_game_of_life", "imgEnergy");

	shaderManager->useShader("diffusion_coupled_game_of_life");
	
	// Divide the dispatch by local workgroup size 
	glDispatchCompute(this->width / 8, this->height / 8, 1);

	// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void AutomataLayer::render() {

    this->shaderManager->useShader("passthrough_quad");
	
	// Bind output texture
	textureManager->bindTextureUnitToGeneralShader(this->inputTexture, "passthrough_quad", "tex");

	// Render image to quad
	this->quadRenderer->render();

	// Swap input and output texture
	std::string swap = this->inputTexture;
	this->inputTexture = outputTexture;
	this->outputTexture = swap;
	
}

