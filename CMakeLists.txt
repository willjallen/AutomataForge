# /CMakeLists.txt
cmake_minimum_required(VERSION 3.15)

project(AutomataForge)

set(CMAKE_CXX_STANDARD 17)

# Define source and include directories
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

# Include the lib/CMakeLists.txt to handle dependencies
add_subdirectory(${LIB_DIR})

# Define the source files
file(GLOB_RECURSE SOURCES "${SOURCE_DIR}/*.cpp")


# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Define includes
# SDL
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR} ${SDL2_INCLUDE_DIR})
# GLEW
target_include_directories(${PROJECT_NAME} PRIVATE ${GLEW_INCLUDE_DIR})
# GLM
find_package(glm REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLM_INCLUDE_DIR})

target_link_directories(${PROJECT_NAME} PRIVATE "${SDL2_BUILD_DIR}/$<CONFIG>")

# message(STATUS "SDL2 Include Directory: ${SDL2_INCLUDE_DIR}")

# Platform-specific logic
if(WIN32)
  message(STATUS "Configuring for Windows")
  set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
  target_link_libraries(${PROJECT_NAME} SDL2 SDL2main)
  target_link_libraries(${PROJECT_NAME} ${GLEW_LIBRARY})
  target_link_libraries(${PROJECT_NAME} opengl32)
  target_link_libraries(${PROJECT_NAME} glm::glm)
else()
  target_link_libraries(${PROJECT_NAME} SDL2)
endif()



# Define the output directory
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/out)

# Delete and recreate the output directory
file(REMOVE_RECURSE ${OUTPUT_DIR})
file(MAKE_DIRECTORY ${OUTPUT_DIR})
# Copy project executable
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_BINARY_DIR}/$<CONFIG>
                    ${OUTPUT_DIR})

# Copy SDL2 libraries
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${SDL2_BUILD_DIR}/$<CONFIG> 
                    ${OUTPUT_DIR})


# Copy GLEW libraries
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${GLEW_LIB_DIR} 
                    ${OUTPUT_DIR})
# Copy GLEW binaries
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                    COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${GLEW_BIN_DIR} 
                    ${OUTPUT_DIR})

# Copy resources from the res/ folder
file(COPY ${CMAKE_SOURCE_DIR}/res/ DESTINATION ${OUTPUT_DIR}/res)
# Copy shaders from the shaders/ folder
file(COPY ${CMAKE_SOURCE_DIR}/shaders/ DESTINATION ${OUTPUT_DIR}/shaders)

