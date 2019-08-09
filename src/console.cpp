#include "console.h"
#include <regex>

Console::Console() {
    legit_commands["clear"] = std::bind(&Console::Clear, this, std::placeholders::_1);
}

void Console::AddCommand(const std::string& name, const ConsoleFunction& function) {
    legit_commands[name] = function;
}

void Console::ExecuteCommand() {
    std::string command_name = command_line[0];
    command_line.erase(command_line.begin());
    auto it = legit_commands.find(command_name);
    if (it != legit_commands.end()) {
        auto func = it->second;
        bool success = func(command_line);
        if (!success) {
            text_buffer.push_back({"Wrong use of '" + command_name + "' !", ImVec4(1.0f, 0.0f, 0.0f, 1.0f)});
        }
    }
    else {
        text_buffer.push_back({"No such command '" + command_name + "' was found!", ImVec4(1.0f, 0.0f, 0.0f, 1.0f)});
    }
}

void Console::ParseString(const std::string& work_string) {
    command_line.clear();
    
    std::string current_string = std::regex_replace(work_string, std::regex("^ +|( ) +"), "$1");
    std::string::size_type pos = current_string.find(' ');
    std::string::size_type index = 0;

    while(pos != std::string::npos) {
        command_line.push_back(current_string.substr(index, pos - index));
        index = pos + 1;
        pos = current_string.find(' ', index + 1);
    }
    command_line.push_back(current_string.substr(index, std::min(pos, current_string.size()) - index + 1));
    ExecuteCommand();
}

void Console::EnterCommand(char* buffer) {
    std::string current_string = std::string(buffer);
    text_buffer.push_back({current_string});
    buffer[0] = '\0';
    
    ParseString(current_string);
}

void Console::Render() {
    ImGui::Begin("Console", NULL, ImGuiWindowFlags_NoResize);
    ImGui::BeginChild("TextWindow", ImVec2(0, 360));
        for (const auto& output_str: text_buffer) {
            ImGui::TextColored(output_str.color, "%s", output_str.data.c_str());
            ImGui::SetScrollHere(1.0f);
        }
    ImGui::EndChild();
    ImGui::Separator();

    if (ImGui::InputText("Enter", buffer, IM_ARRAYSIZE(buffer), ImGuiInputTextFlags_EnterReturnsTrue))
    {
        if (buffer[0]) {
            EnterCommand(buffer);
        }
    }

    //ImGui::SetItemDefaultFocus();
    //ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

	ImGui::End();

}


bool Console::Clear(const std::vector<std::string>& arguments) {
    if (arguments.size() == 0) {
        text_buffer.clear();
        return true;
    }
    return false;
}