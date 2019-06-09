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
    int GetWidth();
    int GetHeight();
    void SetWidth(int);
    void SetHeight(int);
    void SetName(const std::string&);
    static void OnKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void OnMouseMove(GLFWwindow* window, double xpos, double ypos);

private:
    GLFWwindow* window; 
    int width = 640;
    int height = 480;
    std::string name = "Window";
};

#endif /* End of __WINDOW__ */