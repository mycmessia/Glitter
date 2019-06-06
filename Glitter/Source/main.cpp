#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include <Input.hpp>
#include <Window.hpp>
#include <Shader.hpp>
#include <Model.hpp>
#include <Camera.hpp>
#include <Texture2D.hpp>

int main(int argc, const char * argv[])
{
    Window window(800, 600);

    Camera camera(glm::vec3 (0.0f, 0.0f, 3.0f));
    
    Shader modelShader("../Glitter/Shader/model_loading.vs", "../Glitter/Shader/model_loading.fs");
	Shader quadShader("../Glitter/Shader/quad.vs", "../Glitter/Shader/quad.fs");
	quadShader.use();
	quadShader.setInt("screenTexture", 0);
    
    Model nano("../Glitter/Resource/Object/nanosuit/nanosuit.obj");

	unsigned int fbo;
	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	Texture2D fboTexture;
	fboTexture.Generate(window.GetScreenWidth(), window.GetScreenHeight(), NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fboTexture.ID, 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window.GetScreenWidth(), window.GetScreenHeight());

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	float quadVertices[] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  1.0f,  1.0f, 1.0f
	};

	unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
    
    while (!window.IsClose())
    {
        window.CalcDeltaTime();
        
        Input::SetDefalutInputs();
        
        window.PullEvents();
        
        camera.HandleInput(window.GetDeltaTime());

		glEnable(GL_DEPTH_TEST);

		glBindFramebuffer(GL_FRAMEBUFFER, fbo);

		window.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		modelShader.use();

		glm::mat4 projection = glm::perspective(camera.Zoom, (float)window.GetScreenWidth() / (float)window.GetScreenHeight(), 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		modelShader.setMat4("projection", projection);
		modelShader.setMat4("view", view);
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		modelShader.setMat4("model", model);
		nano.Draw(modelShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		window.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		nano.Draw(modelShader);

		glDisable(GL_DEPTH_TEST);

		quadShader.use();
		glBindTexture(GL_TEXTURE_2D, fboTexture.ID);
		glActiveTexture(0);
		glBindVertexArray(quadVAO);
		
		glDrawArrays(GL_TRIANGLES, 0, 6);
        
        window.SwapBuffer ();
    }

	glDeleteFramebuffers(1, &fbo);
    
    return 0;
}
