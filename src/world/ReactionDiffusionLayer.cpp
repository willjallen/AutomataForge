#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <glm\glm.hpp>

#include <memory>
#include <string>

#include "engine/TextureManager.h"
#include "engine/ShaderManager.h"
#include "engine/QuadRenderer.h"
#include "world/ReactionDiffusionLayer.h"

ReactionDiffusionLayer::ReactionDiffusionLayer(unsigned int width, 
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
	shaderManager->loadComputeShader("diffusion_compute", "shaders/reaction_diffusion/compute.glsl");

    float diffusionRateA = 1.0f;
    float diffusionRateB = 0.5f;
    float feedRate = 0.01899f;
    float killRate = 0.04933f;
    float timestep = 1.0f;
    float centerKernelWeight = -1.0f;
    float adjKernelWeight = 0.2f;
    float diagonalKernelWeight = 0.05f;

    // Set uniforms
    shaderManager->setUniform("diffusion_compute", "diffusionRateA", diffusionRateA);
    shaderManager->setUniform("diffusion_compute", "diffusionRateB", diffusionRateB);
    shaderManager->setUniform("diffusion_compute", "feedRate", feedRate);
    shaderManager->setUniform("diffusion_compute", "killRate", killRate);
    shaderManager->setUniform("diffusion_compute", "timestep", timestep);
    shaderManager->setUniform("diffusion_compute", "centerKernelWeight", centerKernelWeight);
    shaderManager->setUniform("diffusion_compute", "adjKernelWeight", adjKernelWeight);
    shaderManager->setUniform("diffusion_compute", "diagKernelWeight", diagonalKernelWeight);

	// Set up heatmap shader for rendering
	shaderManager->loadShader("passthrough_quad", "shaders/reaction_diffusion/quad_vertex.glsl", "shaders/reaction_diffusion/quad_fragment.glsl");

	// Create a buffer for the initial state
	std::vector<float> initialState(this->width * this->height * 4, 0.0f);

	// Populate the buffer with the initial state
    // Set A to 1, B to 0. Disperse B with 1
	for (unsigned int y = 0; y < this->height; y++) { 
		for (unsigned int x = 0; x < this->width; x++) { 
			int index = (y * this->width + x) * 4;
			float sourcePresent = (rand() % 100 < 1) ? 1.0f : 0.0f; // 0.1% chance of being source or sink
			initialState[index] = 1.0f; // Red channel 
			initialState[index + 1] = sourcePresent; // Green channel
			initialState[index + 2] = 0.0f; // Blue channel
			initialState[index + 3] = 1.0f; // Alpha channel (always 1)
		}
	}

	// Create texture one (input/output)
	// Generate a name
	std::string textureOneName = textureManager->generateTextureName(REACTION_DIFFUSION_TEXTURE_PREFIX);
	
	// Create texture
	textureManager->createTexture(textureOneName, this->width, this->height, initialState);
	
	// Bind texture to an image unit
	textureManager->bindImageUnit(textureOneName);

	// Bind texture to a texture unit
	textureManager->bindTextureUnit(textureOneName);

	// Create texture two (input/output)
	// Generate a name
	std::string textureTwoName = textureManager->generateTextureName(REACTION_DIFFUSION_TEXTURE_PREFIX);
	
	// Create texture
	textureManager->createTexture(textureTwoName, this->width, this->height, initialState);
	
	// Bind texture to an image unit
	textureManager->bindImageUnit(textureTwoName);

	// Bind texture to a texture unit
	textureManager->bindTextureUnit(textureTwoName);
	
	// Binds for texture one	
	// Bind image unit to compute shader imgInput
	textureManager->bindImageUnitToComputeShader(textureOneName, "diffusion_compute", "inputGridAB");

	// Binds for texture two	
	// Bind image unit to compute shader imgOutput
	textureManager->bindImageUnitToComputeShader(textureTwoName, "diffusion_compute", "outputGridAB");

	this->inputTexture = textureOneName;
	this->outputTexture = textureTwoName;
	textureManager->bindImageUnitToComputeShader(this->inputTexture, "diffusion_compute", "inputGridAB");
	textureManager->bindImageUnitToComputeShader(this->outputTexture, "diffusion_compute", "outputGridAB");

	// Bind texture unit to quad renderer shader uniform sampler2D tex
	// textureManager->bindTextureUnitToGeneralShader(textureTwoName, "screen_quad", "tex");

}

ReactionDiffusionLayer::~ReactionDiffusionLayer(){

}

std::string ReactionDiffusionLayer::getOutputTexture(){
	return this->outputTexture;
}

void ReactionDiffusionLayer::reset(){

}

void ReactionDiffusionLayer::update(){

	textureManager->bindImageUnitToComputeShader(this->inputTexture, "diffusion_compute", "inputGridAB");
	textureManager->bindImageUnitToComputeShader(this->outputTexture, "diffusion_compute", "outputGridAB");
	
	shaderManager->useShader("diffusion_compute");
	
	// Divide the dispatch by local workgroup size
	glDispatchCompute(this->width / 8, this->height / 8, 1);

	// make sure writing to image has finished before read
	glMemoryBarrier(GL_SHADER_IMAGE_ACCESS_BARRIER_BIT);
}

void ReactionDiffusionLayer::render() {


		shaderManager->useShader("passthrough_quad");
		
        // Bind output texture
		textureManager->bindTextureUnitToGeneralShader(this->outputTexture, "passthrough_quad", "tex");

		// Render image to quad
		quadRenderer->render();

		// Swap input and output texture
		std::string swap = this->inputTexture;
		this->inputTexture = outputTexture;
		this->outputTexture = swap;
		
}


