include(FetchContent)

##############################################
#                   SDL2                     #
##############################################
set(SDL2_SOURCE_DIR "${CMAKE_SOURCE_DIR}/lib/SDL2")
set(SDL2_BUILD_DIR "${SDL2_SOURCE_DIR}/build")
set(SDL2_BUILD_DIR "${SDL2_SOURCE_DIR}/build" PARENT_SCOPE)
set(SDL2_INCLUDE_DIR "${SDL2_SOURCE_DIR}/include" PARENT_SCOPE)

if(NOT EXISTS "${SDL2_BUILD_DIR}") # Adjust the condition as needed for your platform
  FetchContent_Declare(SDL2
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
    GIT_TAG release-2.28.2
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

##############################################
#                   GLEW                     #
##############################################
set(GLEW_DIR "${CMAKE_SOURCE_DIR}/lib/glew")
set(GLEW_INCLUDE_DIR "${GLEW_DIR}/include")
set(GLEW_LIB_DIR "${GLEW_DIR}/lib/Release/x64")

# TODO: Adjust this line based on platform
set(GLEW_LIBRARY "${GLEW_LIB_DIR}/glew32.lib")

# Export the include directory and lib
set(GLEW_INCLUDE_DIR ${GLEW_INCLUDE_DIR} PARENT_SCOPE)
set(GLEW_LIB_DIR ${GLEW_LIB_DIR} PARENT_SCOPE)
set(GLEW_LIBRARY ${GLEW_LIBRARY} PARENT_SCOPE)