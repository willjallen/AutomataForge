#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <vector>
#include <stdexcept>
#include <iostream>
#include "engine/Texture.h"



Texture::Texture(GLsizei width, GLsizei height, const std::vector<float> data){
    // Texture unit assigned to us by the TextureManager
    this->textureUnit = textureUnit;

    this->width = width;
    this->height = height;


    // Declare a variable to hold the texture ID
	GLuint textureID;

	GLenum error = glGetError();
	// Generate a new texture object, storing the ID in 'texture'
	glGenTextures(1, &textureID);
	error = glGetError();
	// Bind the texture object to the 2D texture target, making it the active texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	error = glGetError();
	// TODO: Allow precise specifications of these parameters upon object creation. Maybe passing a config struct
	
	// Set the texture wrapping mode for the S (horizontal) coordinate to CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	error = glGetError();
	// Set the texture wrapping mode for the T (vertical) coordinate to CLAMP_TO_EDGE
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	error = glGetError();
	// Set the magnification filter to linear interpolation (smoothing effect when scaling up)
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// Set the minification filter to linear interpolation (smoothing effect when scaling down)
	// glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	// Avoid any interpolation between pixels (black and white only)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	error = glGetError();
	// Allocate memory and specify the 2D texture image format (e.g., RGBA32F), size, and data type
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, this->width, this->height, 0, GL_RGBA, GL_FLOAT, NULL);
	error = glGetError();
	// Set texture ID
	this->textureID = textureID;
	
	if(data.size() > 0){
		setData(data);
	}

}

Texture::~Texture(){
    glDeleteTextures(1, &this->textureID);
}



// TODO: Allow for definable texture dimensions / more flexibility
void Texture::setData(const std::vector<float>& data){

	if (data.size() != this->width * this->height * 4) {
		throw std::runtime_error("Data size does not match texture dimensions");
	}

	// // Obtain a pointer to the underlying data
    const float* dataPtr = data.data();
	
	// Upload the buffer to the texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->width, this->height, GL_RGBA, GL_FLOAT, dataPtr);
	GLenum error = glGetError();
	if (error != GL_NO_ERROR) {
		printf("OpenGL Error: %d\n", error);
	}
	;;
}

GLuint Texture::getTextureID(){
	return this->textureID;
}



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
