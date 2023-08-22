#include <memory>

#include "world/World.h"
#include "engine/TextureManager.h"
#include "engine/ShaderManager.h"

World::World(std::shared_ptr<ShaderManager> shaderManager, std::shared_ptr<TextureManager> textureManager){
    this->textureManager = textureManager;
    this->shaderManager = shaderManager;
}


	// shaderManager.loadShader("screen_quad", "shaders/screen_quad_vertex.glsl", "shaders/screen_quad_fragment.glsl");
	// shaderManager.loadShader("basic_compute", "shaders/game_of_life_compute.glsl");

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