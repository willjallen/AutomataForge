/*This source code copyrighted by Lazy Foo' Productions 2004-2023
and may not be redistributed without written permission.*/

//Using SDL, SDL OpenGL, GLEW, standard IO, and strings
#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <stdio.h>
#include <string>

#include "engine/window.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;




int main( int argc, char* args[] )
{

	// Create window
	Window window("AutomataForge", SCREEN_WIDTH, SCREEN_HEIGHT);
	
	bool running = true;
	
	//Enable text input
	SDL_StartTextInput();

	//While application is running
	while(running)
	{
		//Handle events on queue
		bool running = window.handleEvents();

		//Render quad
		window.render();
		
		//Update screen
		window.swapBuffers();
	}
	
	//Disable text input
	SDL_StopTextInput();
	

	return 0;
}
