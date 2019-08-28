#include "system_gui.h"
#include "inputSystem.h"

void SystemGUI::Init(GLFWwindow* window) {
    const char* glsl_version = "#version 150";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io; // инициализация ввода/вывода (настройка колбэков)

    ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version); 

    console = std::make_unique<Console>(); // создание консоли
}

void SystemGUI::Update() {
    ImGui_ImplOpenGL3_NewFrame(); // настройка нового кадра для ImGui (нужно вызывать в начале каждого кадра)
    ImGui_ImplGlfw_NewFrame(); // настройка нового кадра для ImGui (нужно вызывать в начале каждого кадра)
    ImGui::NewFrame(); // настройка нового кадра для ImGui (нужно вызывать в начале каждого кадра)
    if (InputSystem::draw_gui) {    
        console->Render();
    }
    ImGui::Render(); // рендер инфы для GetDrawData
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // отрисовка
}

SystemGUI::~SystemGUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown(); // освобождаем ресурсы, используемые ImGui
    ImGui::DestroyContext();
}