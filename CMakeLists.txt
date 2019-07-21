cmake_minimum_required(VERSION 2.6)
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall -g")

project(Lesson1)


find_package (glfw3 REQUIRED)
find_package (OpenGL REQUIRED)
find_package (Assimp REQUIRED)
find_package (GLEW REQUIRED STATIC)

file(GLOB sources 
	  "src/*.cpp"
	  "lib/imGUI/*.cpp"
	)

add_executable(main ${sources}) 
target_include_directories(main PUBLIC include lib/include lib/imGUI)

target_link_libraries(main ${GLEW_LIBRARY} ${OPENGL_LIBRARIES} ${ASSIMP_LIBRARIES} -lglfw)