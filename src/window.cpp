#include "window.h"
#include "inputSystem.h"

// (указатель на GLFWwindow, код нажатой клавиши, действие над клавишей,
// число описывающее модификаторы (shift, control, alt или super) ) 

bool Window::cursor_enabled = false;

void Window::OnKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        if (!cursor_enabled) {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback);
            glfwSetCursorPosCallback(window, nullptr);
            InputSystem::firstMouseMove = true;
        }
        else {
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            glfwSetCursorPosCallback(window, OnMouseMove);
            glfwSetKeyCallback(window, OnKeyPressed);            
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
    // инициализация GLFW
    if (!glfwInit()) {
        exit(EXIT_FAILURE);
    }

	// подсказка glfw окно какой версии создавать
	// Window creation + GL context
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); 
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE); // выключение возможности изменения размера окна

	// Mac OS build fix
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X

    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (!window) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);  // скрыть курсор мыши 
	// context
    glfwSetCursorPosCallback(window, OnMouseMove); // передача функции для курсора в GLFW
    glfwSetKeyCallback(window, OnKeyPressed); // передача функции для клавиатуры в GLFW
	glfwMakeContextCurrent(window);

	// Glew init (инициализация GLEW)
	glewExperimental = true; 
	if (glewInit() != GLEW_OK) { glfwTerminate(); return -1; }
    return 0;
}

void Window::Delete() {
    glfwDestroyWindow(window);
    glfwTerminate();
}