#pragma once

#include <string>
#include <SDL.h>

class Window {
public:
    /**
     * @brief Constructs the Window object and creates an SDL window with an OpenGL context.
     * @param title The title of the window.
     * @param width The width of the window.
     * @param height The height of the window.
     */
    Window(const std::string &title, int width, int height);

    /**
     * @brief Destroys the Window object and cleans up the SDL window and OpenGL context.
     */
    ~Window();

    /**
     * @brief Processes window events and returns whether the window should continue running.
     * @return True if the window should continue running, false otherwise.
     */
    bool handleEvents();

    /**
     * @brief Swaps the OpenGL buffers, presenting the rendered scene.
     */
    void swapBuffers();

    /**
     * @brief Renders openGL context 
     * 
     */
    void render();

private:
    SDL_Window* window;
    SDL_GLContext glContext;

    /**
     * @brief Initializes SDL and creates the window with an OpenGL context.
     */
    void initialize(const std::string &title, int width, int height);

    /**
     * @brief Initializes GL and loads shaders
     */
    void initializeGL();

    /**
     * @brief Cleans up the SDL window and OpenGL context.
     */
    void cleanUp();
};
