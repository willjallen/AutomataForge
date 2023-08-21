#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <stdio.h>


#include "engine/Window.h"

Window::Window(const std::string &title, int width, int height) {
    initialize(title, width, height);
    initializeGL();
}

Window::~Window() {
    cleanUp();
}

bool Window::handleEvents() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT) {
            return false;
        }
    }
    return true;
}

void Window::swapBuffers() {
    SDL_GL_SwapWindow(window);
}

void Window::initialize(const std::string &title, int width, int height) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Failed to init SDL: %s\n", SDL_GetError());
        return;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    this->window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_OPENGL);
    if (!this->window) {
        printf("Failed to create window: %s\n", SDL_GetError());
        return;
    }

    this->glContext = SDL_GL_CreateContext(window);
    if (!this->glContext) {
        printf("Failed to create OpenGL context: %s\n", SDL_GetError());
        return;
    }
    //Initialize GLEW
    glewExperimental = GL_TRUE; 
    GLenum glewError = glewInit();

    if (glewInit() != GLEW_OK) {
        printf("Failed to initialize GLEW\n");
        return;
    }

    // Don't use V-sync
    SDL_GL_SetSwapInterval(0); // Disable V-Sync

    //Use Vsync
    // if( SDL_GL_SetSwapInterval(1) < 0) {
    //     printf( "Warning: Unable to set VSync, SDL Error: %s\n", SDL_GetError() );
    //     return;
    // }


}

void Window::initializeGL(){

	
}

void Window::cleanUp() {
	//Deallocate program
	// glDeleteProgram( gProgramID );

	//Destroy window	
	SDL_DestroyWindow(this->window);
	this->window = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

void Window::render(){

}

// void render()
// {
// 	//Clear color buffer
// 	glClear( GL_COLOR_BUFFER_BIT );
	
// 	//Render quad
// 	if( gRenderQuad )
// 	{
// 		//Bind program
// 		glUseProgram( gProgramID );

// 		//Enable vertex position
// 		glEnableVertexAttribArray( gVertexPos2DLocation );

// 		//Set vertex data
// 		glBindBuffer( GL_ARRAY_BUFFER, gVBO );
// 		glVertexAttribPointer( gVertexPos2DLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), NULL );

// 		//Set index data and render
// 		glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
// 		glDrawElements( GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, NULL );

// 		//Disable vertex position
// 		glDisableVertexAttribArray( gVertexPos2DLocation );

// 		//Unbind program
// 		glUseProgram( NULL );
// 	}
// }