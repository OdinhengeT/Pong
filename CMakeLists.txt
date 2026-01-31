cmake_minimum_required(VERSION 3.16)
project(pong)

# Set the output directory for built objects.
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/$<CONFIGURATION>")

# Export compile commands for language servers
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# This assumes the SDL source is available in vendored/SDL
add_subdirectory(vendored/SDL EXCLUDE_FROM_ALL)

# Create your game executable target as usual
add_executable(pong
    src/main.cpp
    src/entity/entity.cpp
    src/graphics/fonts/bitFont.cpp
    src/graphics/frame.cpp
)

# Link to the actual SDL3 library.
target_link_libraries(pong PRIVATE SDL3::SDL3)
