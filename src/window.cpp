#include "window.h"
#include "inputSystem.h"

bool Window::cursor_enabled = false;

// (указатель на GLFWwindow, код нажатой клавиши, действие над клавишей, число описывающее модификаторы (shift, control, alt или super) ) 
void Window::OnKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
    if (key == GLFW_KEY_TAB && action == GLFW_PRESS) {
        if (!cursor_enabled) { // когда окно гуи нужно включить
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); // включает курсор в окне гуи
            glfwSetKeyCallback(window, ImGui_ImplGlfw_KeyCallback); // чтобы гуи читала нажатие клавиш (колбэк на гуи)
            glfwSetCursorPosCallback(window, nullptr); // отключить колбэк на курсор. чтобы он не двигался в окне гуи (чтобы камера не двигалась)
            InputSystem::firstMouseMove = true;
        }
        else { // когда окно гуи нужно выключить
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // выключаем курсор в окне гуи
            glfwSetCursorPosCallback(window, OnMouseMove); // включить колбэк для курсора
            glfwSetKeyCallback(window, OnKeyPressed); // включить колбэк для клавиатуры        
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

    // создание окна
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

Window::~Window() {
    glfwDestroyWindow(window);
    glfwTerminate();
}