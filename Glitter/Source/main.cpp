//
//  main.cpp
//  GameEngine
//
//  Created by 梅宇宸 on 17/1/2.
//  Copyright © 2017年 梅宇宸. All rights reserved.
//

#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include "Input.hpp"
#include "Window.hpp"
#include "Shader.hpp"
#include "Model.hpp"
#include "Camera.hpp"

int main (int argc, const char * argv[])
{
    Window window (800, 600);

    Camera camera (glm::vec3 (0.0f, 0.0f, 3.0f));
    
    Shader ourShader ("../Glitter/Shader/model_loading.vs", "../Glitter/Shader/model_loading.fs");
    
    Model mountain ("../Glitter/Resource/Object/nanosuit/nanosuit.obj");
    
    glEnable (GL_DEPTH_TEST);
    
    while (!window.IsClose ())
    {
        window.CalcDeltaTime ();
        
        window.Clear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        Input::SetDefalutInputs ();
        
        window.PullEvents ();
        
        camera.HandleInput (window.GetDeltaTime ());
        
		// don't forget to enable shader before setting uniforms
		ourShader.use();

		// view/projection transformations
		glm::mat4 projection = glm::perspective(camera.Zoom, (float)window.GetScreenWidth() / (float)window.GetScreenHeight(), 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		ourShader.setMat4("projection", projection);
		ourShader.setMat4("view", view);

		// render the loaded model
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f)); // translate it down so it's at the center of the scene
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// it's a bit too big for our scene, so scale it down
		ourShader.setMat4("model", model);
		mountain.Draw(ourShader);
        
        window.SwapBuffer ();
    }
    
    return 0;
}
