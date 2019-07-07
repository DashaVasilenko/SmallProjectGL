cmake_minimum_required(VERSION 2.6)
project(Lesson1)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -g")


find_package (glfw3 REQUIRED)
find_package (OpenGL REQUIRED)
find_package (Assimp REQUIRED)
find_package (GLEW REQUIRED STATIC)

add_executable(main src/main.cpp src/window.cpp src/renderer.cpp src/shaderProgram.cpp src/camera.cpp src/inputSystem.cpp
        	src/buffers.cpp src/geometry.cpp src/texture.cpp) 
target_include_directories(main PUBLIC include lib/include)

target_link_libraries(main ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${ASSIMP_LIBRARIES} -lglfw)