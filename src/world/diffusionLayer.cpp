#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <glm\glm.hpp>

#include <memory>
#include <string>

#include "engine/TextureManager.h"
#include "engine/ShaderManager.h"
#include "engine/QuadRenderer.h"
#include "world/DiffusionLayer.h"

DiffusionLayer::DiffusionLayer(unsigned int width, 
							 unsigned int height, 
                             float diffusionRate,
							 std::shared_ptr<ShaderManager> shaderManager, 
							 std::shared_ptr<TextureManager> textureManager, 
							 std::shared_ptr<QuadRenderer> quadRenderer){

	// Set variables
	this->width = width;
	this->height = height;

    this->diffusionRate = diffusionRate;

	this->textureManager = textureManager;
	this->shaderManager = shaderManager;
	this->quadRenderer = quadRenderer;

	GLenum error = glGetError();
	// Set up compute shader
	shaderManager->loadComputeShader("diffusion_compute", "shaders/diffusion_compute.glsl");

	// Set up heatmap shader for rendering
	shaderManager->loadShader("heatmap_quad", "shaders/heatmap_quad_vertex.glsl", "shaders/heatmap_quad_fragment.glsl");
	shaderManager->setUniform("heatmap_quad", "minValue", 0.0f);
	shaderManager->setUniform("heatmap_quad", "maxValue", 40.0f);


	// alpha = 2.0
	// delta_x = 1

	// # Calculated params
	// delta_t = (delta_x ** 2)/(4 * alpha)
	// gamma = (alpha * delta_t) / (delta_x ** 2)
	
	// shaderManager->setUniform("diffusion_compute", "gridSize", glm::ivec2(this->width, this->height));	
	shaderManager->setUniform("diffusion_compute", "diffusionCoefficient", 0.2f);	
	// Set up texture
	// Create a buffer for the initial state
	std::vector<float> initialState(this->width * this->height * 4, 0.0f);
	// Populate the buffer with the initial state
	for (unsigned int y = 0; y < this->height; y++) { // Use height here
		for (unsigned int x = 0; x < this->width; x++) { // Use width here
			int index = (y * this->width + x) * 4;
			float sourceValue = 0.0f;
			float sourcePresent = (rand() % 1000 < 1) ? 1.0f : 0.0f; // 0.1% chance of being source or sink
			if(sourcePresent == 1.0f){
				sourceValue = (rand() % 100 < 50) ? 1.0f : 0.0f; //50% chance of being either a source or sink
			}
			initialState[index] = sourceValue * 0; // Red channel (use this for the state)
			initialState[index + 1] = 0.0f; // Green channel
			initialState[index + 2] = 0.0f; // Blue channel
			initialState[index + 3] = 1.0f; // Alpha channel (always 1)
		}
	}

	// Create texture one (input/output)
	// Generate a name
	std::string textureOneName = textureManager->generateTextureName(DIFFUSION_TEXTURE_PREFIX);
	
	// Create texture
	textureManager->createTexture(textureOneName, this->width, this->height, std::vector<float>(this->width * this->height * 4, 0.0f));
	
	// Bind texture to an image unit
	textureManager->bindImageUnit(textureOneName);

	// Bind texture to a texture unit
	textureManager->bindTextureUnit(textureOneName);

	// Create texture two (input/output)
	// Generate a name
	std::string textureTwoName = textureManager->generateTextureName(DIFFUSION_TEXTURE_PREFIX);
	
	// Create texture
	textureManager->createTexture(textureTwoName, this->width, this->height, std::vector<float>(this->width * this->height * 4, 0.0f));
	
	// Bind texture to an image unit
	textureManager->bindImageUnit(textureTwoName);

	// Bind texture to a texture unit
	textureManager->bindTextureUnit(textureTwoName);
	
	// Create sources(/sinks) texture

	// Generate a name
	std::string sourcesTextureName = textureManager->generateTextureName(DIFFUSION_TEXTURE_PREFIX);
	
	// // Create texture
	textureManager->createTexture(sourcesTextureName, this->width, this->height, initialState);
	
	// // Bind texture to an image unit
	textureManager->bindImageUnit(sourcesTextureName);

	// // Bind texture to a texture unit
	textureManager->bindTextureUnit(sourcesTextureName);

	// Binds for texture one	
	// Bind image unit to compute shader imgInput
	textureManager->bindImageUnitToComputeShader(textureOneName, "diffusion_compute", "inputGrid");

	// Binds for texture two	
	// Bind image unit to compute shader imgOutput
	textureManager->bindImageUnitToComputeShader(textureTwoName, "diffusion_compute", "outputGrid");

	// Binds for sources texture
	// Bind image unit to compute shader imgSources
	textureManager->bindImageUnitToComputeShader(sourcesTextureName, "diffusion_compute", "sourceGrid");

	this->inputTexture = textureOneName;
	this->outputTexture = textureTwoName;
	this->sourcesTexture = sourcesTextureName;
	textureManager->bindImageUnitToComputeShader(this->inputTexture, "diffusion_compute", "inputGrid");
	textureManager->bindImageUnitToComputeShader(this->outputTexture, "diffusion_compute", "outputGrid");

	// Bind texture unit to quad renderer shader uniform sampler2D tex
	// textureManager->bindTextureUnitToGeneralShader(textureTwoName, "screen_quad", "tex");

}

DiffusionLayer::~DiffusionLayer(){

}

std::string DiffusionLayer::getOutputTexture(){
	return this->outputTexture;
}

void DiffusionLayer::reset(){

}

void DiffusionLayer::update(){

	textureManager->bindImageUnitToComputeShader(this->inputTexture, "diffusion_compute", "inputGrid");
	textureManager->bindImageUnitToComputeShader(this->outputTexture, "diffusion_compute", "outputGrid");
	textureManager->bindImageUnitToComputeShader(this->sourcesTexture, "diffusion_compute", "sourceGrid");
	
	shaderManager->useShader("diffusion_compute");
	
	// Divide the dispatch by local workgroup size (10 in this case)
	glDispatchCompute(this->width / 8, this->height / 8, 1);

	// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void DiffusionLayer::render() {

		// Bind output texture
		textureManager->bindTextureUnitToGeneralShader(this->outputTexture, "heatmap_quad", "tex");

		shaderManager->useShader("heatmap_quad");

		// Render image to quad
		quadRenderer->render();

		// Swap input and output texture
		std::string swap = this->inputTexture;
		this->inputTexture = outputTexture;
		this->outputTexture = swap;
		
}

