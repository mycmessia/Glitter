//
//  Input.cpp
//  OGLEngine
//
//  Created by 梅宇宸 on 17/1/3.
//  Copyright © 2017年 梅宇宸. All rights reserved.
//

#include "Input.hpp"

GLboolean Input::keysProcessed[1024] = {false};
GLboolean Input::keys[1024] = {false};

GLboolean Input::isFirstMove = true;
double Input::lastMouseX = 0.0;
double Input::lastMouseY = 0.0;
GLfloat Input::mouseMoveOffsetX = 0.0F;
GLfloat Input::mouseMoveOffsetY = 0.0f;

double Input::mouseScrollOffsetY = 0.0;

GLboolean Input::IsKeyDown (int key)
{
    return keys[key];
}

GLboolean Input::IsKeyPress (int key)
{
    if (keys[key] && !keysProcessed[key])
    {
        keysProcessed[key] = true;
        return true;
    }
    
    return false;
}

void Input::SetDefalutInputs ()
{
    mouseMoveOffsetX = 0.0f;
    mouseMoveOffsetY = 0.0f;
    
    mouseScrollOffsetY = 0.0;
}