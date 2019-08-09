#include "system_gui.h"
#include "inputSystem.h"

void DirectionalLightMenu::Render() {
     ImGui::Begin("Light Menu", NULL);
     ImGui::End();
}

void SystemGUI::Init(GLFWwindow* window) {
    const char* glsl_version = "#version 150";

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;


    ImGui::StyleColorsClassic();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    console = std::make_unique<Console>();
    lightMenu = std::make_unique<DirectionalLightMenu>();
}

void SystemGUI::Update() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    if (InputSystem::draw_gui) {    
        console->Render();
        lightMenu->Render();   
    }
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

SystemGUI::~SystemGUI() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}