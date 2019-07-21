#include "console.h"

void Console::EnterCommand(char* buffer) {
    commands.push_back(buffer);
    buffer[0] = '\0';
}

void Console::Render() {
    ImGui::Begin("Console", NULL, ImGuiWindowFlags_NoResize);
    ImGui::BeginChild("TextWindow", ImVec2(0, 360));
        for (const auto& command: commands) {
            ImGui::Text("%s", command.c_str());
            ImGui::SetScrollHere(1.0f);
        }
    ImGui::EndChild();
    ImGui::Separator();
    //ImGui::SetKeyboardFocusHere();
    //bool reclaim_focus = false;
    if (ImGui::InputText("Enter", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue|ImGuiInputTextFlags_CallbackCompletion|ImGuiInputTextFlags_CallbackHistory, callback, (void*)this))
    {
        if (buffer[0]) {
            EnterCommand(buffer);
        }
        //reclaim_focus = true;
    }

    ImGui::SetItemDefaultFocus();
    ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

	ImGui::End();

}