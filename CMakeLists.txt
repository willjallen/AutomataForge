cmake_minimum_required(VERSION 3.15)

project(AutomataForge)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)

# Define source and include directories
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/lib)

# Include the lib/CMakeLists.txt to handle dependencies
add_subdirectory(${LIB_DIR})

# Add the include directory
include_directories(${INCLUDE_DIR})

# Define the source files
file(GLOB SOURCES "${SOURCE_DIR}/*.cpp")

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES})

# Link libraries (SDL2-static target should be available after including lib/CMakeLists.txt)
target_link_libraries(${PROJECT_NAME} SDL2-static)