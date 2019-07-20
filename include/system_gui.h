#ifndef __GUI__
#define __GUI__

#include "console.h"
#include "GLFW/glfw3.h"
#include <memory>

class SystemGUI {
public:
    void Init(GLFWwindow* window);
    void Update();
    ~SystemGUI();
private:
    std::unique_ptr<Console> console;
};

#endif /* End of GUI */