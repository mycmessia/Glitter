#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Window
{
public:
    Window (int sw, int sh);
    ~Window ();
    
    inline GLFWwindow* GetGLWindow () { return glWindow; };
    
    bool IsClose ();
    
    void PullEvents ();
    
    void Clear (GLbitfield mask);
    
    void SwapBuffer ();
    
    static void KeyCallback (GLFWwindow* window, int key, int scancode, int action, int mode);
    static void MouseCallback (GLFWwindow* window, double xpos, double ypos);
    static void ScrollCallback (GLFWwindow* window, double xoffset, double yoffset);
    
    int GetScreenWidth () { return screenWidth; }
    int GetScreenHeight () { return screenHeight; }
    
    void CalcDeltaTime ();
    float GetDeltaTime ();
    
private:
    GLFWwindow* glWindow;
    
    int screenWidth;
    int screenHeight;
    int framebufferWidth;
    int framebufferHeight;
    
    float deltaTime;
    float lastTime;
};