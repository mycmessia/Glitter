//
//  Input.hpp
//  OGLEngine
//
//  Created by 梅宇宸 on 17/1/3.
//  Copyright © 2017年 梅宇宸. All rights reserved.
//

#ifndef Input_hpp
#define Input_hpp

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class Window;

class Input
{
friend class Window;
friend class Camera;
    
public:
    static GLboolean IsKeyDown (int key);
    static GLboolean IsKeyPress (int key);
    static void SetDefalutInputs ();
    
private:
    static GLboolean keysProcessed[1024];
    static GLboolean keys[1024];
    
    static double lastMouseX;
    static double lastMouseY;
    static GLboolean isFirstMove;
    static GLfloat mouseMoveOffsetX;
    static GLfloat mouseMoveOffsetY;
    
    static double mouseScrollOffsetY;
};

#endif /* Input_hpp */
