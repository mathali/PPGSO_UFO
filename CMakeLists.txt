cmake_minimum_required(VERSION 3.14)
project(test)

set(CMAKE_CXX_STANDARD 14)

add_executable(test main.cpp)

#
# CONFIGURATION
#

# Basic CMake settings
set(CMAKE_INSTALL_PREFIX ${CMAKE_CURRENT_SOURCE_DIR}/_install)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/data)

# Use add_resources function to convert .glsl to a .cpp files which will be compiled into a separate static library
include(add_resources)

#
# DEPENDENCIES
#

set(CMAKE_INCLUDE_PATH ${CMAKE_INCLUDE_PATH} "${CMAKE_SOURCE_DIR}/dependencies/include/")
set(CMAKE_LIBRARY_PATH ${CMAKE_LIBRARY_PATH} "${CMAKE_SOURCE_DIR}/dependencies/lib/mingw")

# Warnings for Debug mode
option(USE_STRICT_COMPILE_WARNINGS "Use strict compilation warnings in debug mode." ON)
# These compile flags should apply for GCC and Clang compilers
set(STRICT_COMPILE_FLAGS "-Wpedantic -Wall -Wno-c++98-compat -Wextra -Wno-sign-conversion -Wno-unused-parameter")
if (USE_STRICT_COMPILE_WARNINGS)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} ${STRICT_COMPILE_FLAGS}")
endif ()

# Find required packages
find_package(GLFW3 REQUIRED)
find_package(GLEW REQUIRED)
find_package(GLM REQUIRED)
find_package(OpenGL REQUIRED)

# Optional packages
find_package(OpenMP)
if(OPENMP_FOUND)
    list(APPEND CMAKE_CXX_FLAGS ${OpenMP_CXX_FLAGS})
endif()

#
# Shaders / Materials
#

set(PPGSO_SHADER_SRC
        shader/color_vert.glsl shader/color_frag.glsl
        shader/convolution_vert.glsl shader/convolution_frag.glsl
        shader/diffuse_vert.glsl shader/diffuse_frag.glsl
        shader/texture_vert.glsl shader/texture_frag.glsl
        )
add_resources(shaders ${PPGSO_SHADER_SRC})

# PPGSO library
add_library(ppgso STATIC
        ppgso/mesh.cpp
        ppgso/tiny_obj_loader.cpp
        ppgso/shader.cpp
        ppgso/image.cpp
        ppgso/image_bmp.cpp
        ppgso/image_raw.cpp
        ppgso/texture.cpp
        ppgso/window.cpp
        )

# Make sure GLM uses radians and GLEW is a static library
target_compile_definitions(ppgso PUBLIC -DGLM_FORCE_RADIANS -DGLEW_STATIC)

# Link to GLFW, GLEW and OpenGL
target_link_libraries(ppgso PUBLIC ${GLFW_LIBRARIES} GLEW::GLEW ${OPENGL_LIBRARIES} ${OpenMP_libomp_LIBRARY})
# Pass on include directories
target_include_directories(ppgso PUBLIC
        ppgso
        ${GLFW_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR})

# Make sure GLM uses radians and GLEW is a static library
target_compile_definitions(test PUBLIC -DGLM_FORCE_RADIANS -DGLEW_STATIC)

# Link to GLFW, GLEW and OpenGL
target_link_libraries(test PUBLIC ${GLFW_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})

# Pass on include directories
target_include_directories(test PUBLIC
        ${GLFW_INCLUDE_DIRS}
        ${GLEW_INCLUDE_DIRS}
        ${GLM_INCLUDE_DIRS}
        ${OPENGL_INCLUDE_DIRS}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_CURRENT_BINARY_DIR})

add_executable(ufo_game
        src/game_scene.cpp
        src/object.cpp
        src/scene.cpp
        src/camera.cpp
        src/cow.cpp
        src/cow_shadow.cpp
        src/generator.cpp
        src/player.cpp
        src/beam.cpp
        src/explosion.cpp
        src/ground.cpp
        src/house.cpp
        src/house_shadow.cpp
        src/tree.cpp
        src/tree_shadow.cpp
        src/grass.cpp
        src/room_wall.cpp
        src/cage.cpp)
target_link_libraries(ufo_game ppgso shaders)
install(TARGETS ufo_game DESTINATION .)