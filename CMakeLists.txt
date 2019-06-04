cmake_minimum_required(VERSION 2.6)
project(Lesson1)

find_package (glfw3 REQUIRED)
find_package (OpenGL REQUIRED)


add_executable(main main.cpp)

target_link_libraries(main ${OPENGL_LIBRARIES} -lglfw)