cmake_minimum_required(VERSION 2.6)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -g")

project(Lesson1)


find_package (glfw3 REQUIRED)
find_package (OpenGL REQUIRED)
find_package (Assimp REQUIRED)
find_package (GLEW REQUIRED)

# assimp cmake fix https://github.com/ros-planning/geometric_shapes/issues/23
find_library(
    assimp_LIBRARIES
    NAMES assimp
)

file(GLOB sources 
	  "src/*.cpp"
	  "lib/imGUI/*.cpp"
	)

add_executable(main ${sources}) 
target_include_directories(main PUBLIC include lib/include lib/imGUI)

target_link_libraries(main PRIVATE GLEW::GLEW ${OPENGL_LIBRARIES} ${assimp_LIBRARIES} glfw)