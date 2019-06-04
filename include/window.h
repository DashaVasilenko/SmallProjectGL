#ifndef __WINDOW__
#define __WINDOW__

#include <GL/glew.h>
#define GLEW_STATIC

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string>

class Window {
public:
    GLFWwindow* GetPointer();
    int Init();
    void Destroy();

    void SetWidth(int width);
    void SetHeight(int height);
    void SetName(const std::string& name);
private:
    GLFWwindow* window; 
    int width = 640;
    int height = 480;
    std::string name = "Window";
};


#endif /* End of __WINDOW__ */