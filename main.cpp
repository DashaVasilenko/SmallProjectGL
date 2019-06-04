#include <iostream>
#include <stdlib.h>
#include <GLFW/glfw3.h>

int main() {
	// GLFW
	// Window creation

	GLFWwindow* window;
	
	if (!glfwInit())
        exit(EXIT_FAILURE);

	// Подсказка glfw окно какой версии создавать
	// Window creation + GL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Mac OS build fix
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X

    window = glfwCreateWindow(1080, 768, "Lesson 1", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

	// context
	glfwMakeContextCurrent(window);

	while (!glfwWindowShouldClose(window)) {
		glfwSwapBuffers(window);
    	glfwPollEvents();
	}

	glfwDestroyWindow(window);
    glfwTerminate();

	std::cout << "Hello World3!" << std::endl;
	return 0;
}