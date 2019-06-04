//
//  Renderer.cpp
//  GameEngine
//
//  Created by 梅宇宸 on 17/1/2.
//  Copyright © 2017年 梅宇宸. All rights reserved.
//

#include "Renderer.hpp"

void Renderer::render (RawModel model)
{
    glBindVertexArray (model.GetVaoID ());
        glEnableVertexAttribArray (0);          // positions is 0
//        glDrawArrays (GL_TRIANGLES, 0, model.GetVertexCount ());
        glDrawElements (GL_TRIANGLES, model.GetVertexCount (), GL_UNSIGNED_INT, 0);
        glDisableVertexAttribArray (0);
    glBindVertexArray (0);
}