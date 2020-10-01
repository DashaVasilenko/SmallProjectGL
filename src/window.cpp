#include "window.h"
#include "inputSystem.h"

bool Window::cursor_enabled = false; 

void Window::OnKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        if (!cursor_enabled) { 
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); 
            glfwSetCursorPosCallback(window, nullptr); 
            InputSystem::firstMouseMove = true;
            InputSystem::cameraMove = false;
        }
        else { 
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); 
            glfwSetCursorPosCallback(window, OnMouseMove);
            InputSystem::cameraMove = true;    
        }
        cursor_enabled = !cursor_enabled;
        InputSystem::draw_gui = !InputSystem::draw_gui;
    }
    if (action == GLFW_PRESS) 
        InputSystem::keys[key] = true;
    else if (action == GLFW_RELEASE)
        InputSystem::keys[key] = false;
}

void Window::OnMouseMove(GLFWwindow* window, double xpos, double ypos) {
    if (InputSystem::firstMouseMove) {
        InputSystem::lastCursPosX = xpos;
        InputSystem::lastCursPosY = ypos;
    }
    InputSystem::deltaCursPosX = xpos - InputSystem::lastCursPosX;
    InputSystem::deltaCursPosY = InputSystem::lastCursPosY - ypos;
    InputSystem::lastCursPosX = xpos;
    InputSystem::lastCursPosY = ypos;
    InputSystem::firstMouseMove = false;
}

int Window::Init() {
    if (!glfwInit()) {
        return -1;    
    }
    init_glfw = true;

	// Window creation + GL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Mac OS build fix
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X

    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    init_window = true; 

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  
	// context
    glfwSetCursorPosCallback(window, OnMouseMove);
    glfwSetKeyCallback(window, OnKeyPressed); 
	glfwMakeContextCurrent(window);

	// Glew init
	glewExperimental = true; 
	if (glewInit() != GLEW_OK) { glfwTerminate(); return -1; }
    return 0;
}

Window::~Window() {
    if(init_window) 
        glfwDestroyWindow(window);
    if(init_glfw)
        glfwTerminate();
}