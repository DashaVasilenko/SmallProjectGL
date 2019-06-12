#ifndef __WINDOW__
#define __WINDOW__

#include <GL/glew.h>
#define GLEW_STATIC

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <string>

class Window {
public:
    GLFWwindow* GetPointer() { return this->window; }
    void SetWidth(int width) { this->width = width; }
    void SetHeight(int height) { this->height = height; }
    void SetName(const std::string& name) { this->name = name; }
    int GetWidth() { return this->width; }
    int GetHeight() { return this->height; }
    int Init();
    void Delete();
     
    static void OnKeyPressed(GLFWwindow* window, int key, int scancode, int action, int mode);
    static void OnMouseMove(GLFWwindow* window, double xpos, double ypos);

private:
    GLFWwindow* window; 
    int width = 640;
    int height = 480;
    std::string name = "Window";
};

#endif /* End of __WINDOW__ */