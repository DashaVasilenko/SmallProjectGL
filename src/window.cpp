#include "window.h"

void Window::SetWidth(int width) {
    this->width = width;
}

void Window::SetHeight(int height) {
    this->height = height;
}

void Window::SetName(const std::string& name) {
    this->name = name;
}

GLFWwindow* Window::GetPointer() {
    return this->window;
}

int Window::Init() {
    // GLFW
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

    window = glfwCreateWindow(width, height, "Lesson 1", NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
	
	// context
	glfwMakeContextCurrent(window);

	// Glew init
	glewExperimental = true; 
	if (glewInit() != GLEW_OK) { glfwTerminate(); return -1; }
    return 0;
}

void Window::Destroy() {
    glfwDestroyWindow(window);
    glfwTerminate();
}