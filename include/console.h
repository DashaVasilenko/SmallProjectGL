#ifndef __CONSOLE__
#define __CONSOLE__


#include <iostream>
#include <vector>
#include <string>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define MAX_COMMAND_LENGTH 256

class Console {
public:
    void Render();
private:
    char buffer[MAX_COMMAND_LENGTH];
    std::vector<std::string> commands;
    void EnterCommand(char* buffer);




    static int callback(ImGuiTextEditCallbackData* data) {
        data->EventChar = *static_cast<const char*>(data->UserData);
        return 0;
    }
};

#endif /* End of Console */