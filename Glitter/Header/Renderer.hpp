//
//  Renderer.hpp
//  GameEngine
//
//  Created by 梅宇宸 on 17/1/2.
//  Copyright © 2017年 梅宇宸. All rights reserved.
//

#ifndef Renderer_hpp
#define Renderer_hpp

#include <glad/glad.h>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include "RawModel.hpp"

class Renderer
{
public:
    void render (RawModel model);
};

#endif /* Renderer_hpp */
