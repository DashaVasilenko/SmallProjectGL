#ifndef __GUI__
#define __GUI__

#include "console.h"
#include "GLFW/glfw3.h"
#include <memory>

class DirectionalLightMenu {
public:
    void Render();
private:
};

class SystemGUI {
public:
    void Init(GLFWwindow* window);
    void Update();
    std::unique_ptr<Console>& GetConsole() { return console; }
    ~SystemGUI();
private:
    std::unique_ptr<Console> console;
    std::unique_ptr<DirectionalLightMenu> lightMenu;
 };

#endif /* End of GUI */