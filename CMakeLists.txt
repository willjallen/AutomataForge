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
file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

set(SDL2_INCLUDE_PATH "${LIB_DIR}/SDL2/include") # Adjust the path according to your project structure
target_include_directories(${PROJECT_NAME} PRIVATE ${INCLUDE_DIR} ${SDL2_INCLUDE_DIRS})


# Platform-specific logic
if(WIN32)
  message(STATUS "Configuring for Windows")
  set_target_properties(${PROJECT_NAME} PROPERTIES LINK_FLAGS "/SUBSYSTEM:CONSOLE")
  target_link_libraries(${PROJECT_NAME} SDL2 SDL2main)

else()
  target_link_libraries(${PROJECT_NAME} SDL2)
endif()

# Define the output directory
set(OUTPUT_DIR ${CMAKE_SOURCE_DIR}/out)

# Delete and recreate the output directory
add_custom_command(OUTPUT ${OUTPUT_DIR}
                   COMMAND ${CMAKE_COMMAND} -E remove_directory ${OUTPUT_DIR}
                   COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIR})

if(CMAKE_BUILD_TYPE MATCHES Debug)
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_directory
                     ${CMAKE_BINARY_DIR}/Debug
                     ${OUTPUT_DIR})

  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_directory
                     ${CMAKE_BINARY_DIR}/lib/SDL2/debug
                     ${OUTPUT_DIR})
else() # Assuming Release mode
  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_directory
                     ${CMAKE_BINARY_DIR}/Release
                     ${OUTPUT_DIR})

  add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                     COMMAND ${CMAKE_COMMAND} -E copy_directory
                     ${CMAKE_BINARY_DIR}/lib/SDL2/release
                     ${OUTPUT_DIR})
endif()

# Copy resources from the res/ folder
file(COPY ${CMAKE_SOURCE_DIR}/res/ DESTINATION ${OUTPUT_DIR}/res)

