#include <iostream>

#include <Input.hpp>
#include <Window.hpp>

Window::Window (int sw, int sh) : screenWidth (sw), screenHeight (sh)
{
    glfwInit ();
    glfwWindowHint (GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint (GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint (GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint (GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint (GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);   // This is vital on MacOSX
    
    glWindow = glfwCreateWindow (screenWidth, screenHeight, "Breakout", nullptr, nullptr);
    glfwMakeContextCurrent (glWindow);
    
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
	}

    glfwGetFramebufferSize (glWindow, &framebufferWidth, &framebufferHeight);
    glViewport (0, 0, framebufferWidth, framebufferHeight);

	glfwSetInputMode(glWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwSetKeyCallback (glWindow, KeyCallback);
    glfwSetCursorPosCallback (glWindow, MouseCallback);
    glfwSetScrollCallback (glWindow, ScrollCallback);
    
    glClearColor (0.2, 0.2, 0.2, 1.0);
}

Window::~Window ()
{
    glfwTerminate ();
}

bool Window::IsClose ()
{
    return glfwWindowShouldClose (glWindow);
}

void Window::PullEvents ()
{
    glfwPollEvents ();
}

void Window::Clear (GLbitfield mask)
{
    glClear (mask);
}

void Window::SwapBuffer ()
{
    glfwSwapBuffers (glWindow);
}

void Window::KeyCallback (GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true, closing the application
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose (window, GL_TRUE);
    }
    
    if (key >= 0 && key < 1024)
    {
        if (action == GLFW_PRESS)
        {
            Input::keys[key] = GL_TRUE;
        }
        else if (action == GLFW_RELEASE)
        {
            Input::keys[key] = GL_FALSE;
            Input::keysProcessed[key] = GL_FALSE;
        }
    }
}

void Window::MouseCallback (GLFWwindow* window, double xpos, double ypos)
{
    if (Input::isFirstMove)
    {
        Input::lastMouseX = xpos;
        Input::lastMouseY = ypos;
        Input::isFirstMove = false;
    }
    
    Input::mouseMoveOffsetX = xpos - Input::lastMouseX;
    Input::mouseMoveOffsetY = Input::lastMouseY - ypos;
    
    Input::lastMouseX = xpos;
    Input::lastMouseY = ypos;
}

void Window::ScrollCallback (GLFWwindow* window, double xoffset, double yoffset)
{
    Input::mouseScrollOffsetY = yoffset;
}

void Window::CalcDeltaTime ()
{
    GLfloat currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastTime;
    lastTime = currentFrame;
}

float Window::GetDeltaTime ()
{
    return deltaTime;
}