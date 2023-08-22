include(FetchContent)

cmake_policy(SET CMP0135 NEW)

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
# TODO: Support other platforms
set(GLEW_SOURCE_DIR "${CMAKE_SOURCE_DIR}/lib/glew")
set(GLEW_BUILD_DIR "${GLEW_SOURCE_DIR}/build/cmake")
set(GLEW_INCLUDE_DIR "${GLEW_SOURCE_DIR}/include") # Fixed this line
set(GLEW_LIB_DIR "${GLEW_SOURCE_DIR}/lib/Release/x64")
set(GLEW_BIN_DIR "${GLEW_SOURCE_DIR}/bin/Release/x64")

# TODO: Adjust this line based on platform
set(GLEW_LIBRARY "${GLEW_LIB_DIR}/glew32.lib")

if(NOT EXISTS "${GLEW_SOURCE_DIR}")
  FetchContent_Declare(GLEW
    URL https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0-win32.zip
    SOURCE_DIR ${GLEW_SOURCE_DIR}
  )
  FetchContent_GetProperties(GLEW)
  if(NOT glew_POPULATED)
    FetchContent_Populate(GLEW)
  endif()
endif()
# Export the include directory and lib
set(GLEW_INCLUDE_DIR ${GLEW_INCLUDE_DIR} PARENT_SCOPE)
set(GLEW_LIB_DIR ${GLEW_LIB_DIR} PARENT_SCOPE)
set(GLEW_BIN_DIR ${GLEW_BIN_DIR} PARENT_SCOPE)
set(GLEW_LIBRARY ${GLEW_LIBRARY} PARENT_SCOPE)



# TODO: Try to build from source at some point
# if(NOT EXISTS "${GLEW_SOURCE_DIR}")
#   FetchContent_Declare(GLEW
#     URL https://github.com/nigels-com/glew/releases/download/glew-2.2.0/glew-2.2.0.zip
#     SOURCE_DIR ${GLEW_SOURCE_DIR}
#   )
#   FetchContent_GetProperties(GLEW)
#   if(NOT glew_POPULATED)
#     FetchContent_Populate(GLEW)
#     if(EXISTS "${GLEW_BUILD_DIR}/glew.sln") # Target the solution file
#       # Invoke MSBuild to build the GLEW solution
#       execute_process(
#         COMMAND "MSBuild" "${GLEW_BUILD_DIR}/glew.sln" # Target the solution file
#         RESULT_VARIABLE result
#       )
#       if(result)
#         message(FATAL_ERROR "Build failed with error code: ${result}")
#       endif()
#     else()
#       message(WARNING "GLEW solution file not found")
#     endif()
#   endif()
# endif()

##############################################
#                    GLM                     #
##############################################
set(GLM_SOURCE_DIR "${CMAKE_SOURCE_DIR}/lib/GLM")
set(GLM_INCLUDE_DIR "${GLM_SOURCE_DIR}" PARENT_SCOPE)

if(NOT EXISTS "${GLM_SOURCE_DIR}")
  FetchContent_Declare(GLM
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG 0.9.9.8
    SOURCE_DIR ${GLM_SOURCE_DIR}
  )
  FetchContent_MakeAvailable(glm)
endif()
set(glm_DIR ${CMAKE_SOURCE_DIR}/lib/GLM/cmake/glm PARENT_SCOPE) 