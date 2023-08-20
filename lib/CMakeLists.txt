# lib/CMakeLists.txt
set(SDL2_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/SDL2")
set(SDL2_BUILD_DIR "${SDL2_SOURCE_DIR}/build")


# Check if SDL2 source is present
if(NOT EXISTS "${SDL2_SOURCE_DIR}")
  find_package(Git REQUIRED)
  execute_process(
    COMMAND ${GIT_EXECUTABLE} clone https://github.com/libsdl-org/SDL.git -b SDL2 ${SDL2_SOURCE_DIR}
  )
endif()

# Platform-specific logic for building SDL2
if(WIN32)
  set(SDL2_SHARED_LIB "${SDL2_BUILD_DIR}/SDL2.dll")
else()
  set(SDL2_SHARED_LIB "${SDL2_BUILD_DIR}/libSDL2.so")
endif()

# Check if SDL2 has been built
if(NOT EXISTS ${SDL2_SHARED_LIB})
  execute_process(
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SDL2_BUILD_DIR}
    COMMAND ${CMAKE_COMMAND} -S ${SDL2_SOURCE_DIR} -B ${SDL2_BUILD_DIR} -DSDL_SHARED=ON -DSDL_STATIC=OFF
    COMMAND ${CMAKE_COMMAND} --build ${SDL2_BUILD_DIR}
  )
endif()

# Include SDL2 as a subdirectory 
add_subdirectory(${SDL2_SOURCE_DIR} EXCLUDE_FROM_ALL)
