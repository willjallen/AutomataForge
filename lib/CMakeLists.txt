include(FetchContent)

##########
## SDL2 ##
##########
set(SDL2_SOURCE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2")
set(SDL2_BUILD_DIR "${SDL2_SOURCE_DIR}/build")

set(SDL2_SOURCE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2" PARENT_SCOPE)
set(SDL2_BUILD_DIR "${SDL2_SOURCE_DIR}/build" PARENT_SCOPE)
# Specify the include directories for SDL2
set(SDL2_INCLUDE_DIR "${SDL2_SOURCE_DIR}/include" PARENT_SCOPE)

if(NOT EXISTS "${SDL2_BUILD_DIR}") # Adjust the condition as needed for your platform
  FetchContent_Declare(SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG SDL2
    SOURCE_DIR ${SDL2_SOURCE_DIR}
  )

  FetchContent_GetProperties(SDL2)

  if(NOT sdl2_POPULATED)
    FetchContent_Populate(SDL2)

    # Configure SDL2 build
    set(SDL_SHARED ON CACHE BOOL "" FORCE)
    set(SDL_STATIC OFF CACHE BOOL "" FORCE)

    add_subdirectory(${SDL2_SOURCE_DIR} ${SDL2_BUILD_DIR})
  endif()
endif()

