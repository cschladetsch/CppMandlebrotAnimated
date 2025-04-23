cmake_minimum_required(VERSION 3.16)
project(CppMandelbrotGPU)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find required packages
find_package(SDL2 REQUIRED)
find_package(Boost REQUIRED COMPONENTS program_options)
find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)

# Include directories
include_directories(${SDL2_INCLUDE_DIRS})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/src)

# Define source files
set(SOURCES
    src/main.cpp
    src/app.cpp
    src/shader.cpp
    src/config.cpp
)

# Create executable
add_executable(mandelbrot_gpu ${SOURCES})

# Link libraries
target_link_libraries(mandelbrot_gpu
    ${SDL2_LIBRARIES}
    ${Boost_LIBRARIES}
    ${OPENGL_LIBRARIES}
    ${GLEW_LIBRARIES}
    GL GLEW
)

# Copy shader files to the build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# Install target
install(TARGETS mandelbrot_gpu DESTINATION bin)
install(DIRECTORY shaders/ DESTINATION bin/shaders)
