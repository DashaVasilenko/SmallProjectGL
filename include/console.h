#ifndef __CONSOLE__
#define __CONSOLE__

#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <functional>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#define MAX_COMMAND_LENGTH 128

// тип функций, который возвращает бул,а на вход получает константную ссылку на вектор строк
using ConsoleFunction = std::function<bool (const std::vector<std::string>&)>;

class OutputStr {
public:
    OutputStr(const std::string& data, const ImVec4& color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f)) {
        this->data = data;
        this->color = color;
    }
    std::string data;
    ImVec4 color ;
};

class Console {
public:
    Console();
    void Render();
    void AddCommand(const std::string&, const ConsoleFunction&);

private:
    std::unordered_map<std::string, ConsoleFunction> legit_commands;
    
    char buffer[MAX_COMMAND_LENGTH];
    std::vector<std::string> command_line;
    std::vector<OutputStr> text_buffer; // текст, который выводится в консоли при наборе
    void EnterCommand(char* buffer);
    void ParseString(const std::string& );
    void ExecuteCommand();

    bool Clear(const std::vector<std::string>&);
};

#endif /* End of Console */