#include <iostream>

#define STB_IMAGE_IMPLEMENTATION

#include <Input.hpp>
#include <Window.hpp>
#include <Shader.hpp>
#include <Model.hpp>
#include <Camera.hpp>
#include <Texture2D.hpp>
#include <Quad.h>
#include <Cube.h>

int main(int argc, const char * argv[])
{
	const unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 600;

    Window window(SCR_WIDTH, SCR_HEIGHT);

    Camera camera(glm::vec3 (0.0f, 3.0f, 3.0f));
    
    Shader modelShader("../Glitter/Shader/model_loading.vs", "../Glitter/Shader/model_loading.fs");
	Shader quadShader("../Glitter/Shader/quad.vs", "../Glitter/Shader/quad.fs");
	quadShader.use();
	quadShader.setInt("screenTexture", 1);
	Shader cubeShader("../Glitter/Shader/cube.vs", "../Glitter/Shader/cube.fs");

	Shader shadowMappingShader("../Glitter/Shader/shadow_mapping.vs", "../Glitter/Shader/shadow_mapping.fs");
	Shader simpleDepthShader("../Glitter/Shader/shadow_mapping_depth.vs", "../Glitter/Shader/shadow_mapping_depth.fs");
	Shader debugDepthQuadShader("../Glitter/Shader/shadowmap_debug_quad.vs", "../Glitter/Shader/shadowmap_debug_quad.fs");
    
    Model ellia("../Glitter/Resource/Object/ellia-halloween/ellia-halloween.dae");
	Model terrain("../Glitter/Resource/Object/MiniYosshiHomeGalaxy/MiniYosshiHomeGalaxy.obj");

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

	Quad quad;
	Cube cube;

	// configure depth map FBO
	// -----------------------
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	unsigned int depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// shader configuration
	// --------------------
	shadowMappingShader.use();
	shadowMappingShader.setInt("diffuseTexture", 0);
	shadowMappingShader.setInt("shadowMap", 1);
	debugDepthQuadShader.use();
	debugDepthQuadShader.setInt("depthMap", 0);

	// lighting info
	// -------------
	glm::vec3 lightPos(-2.0f, 4.0f, -1.0f);
    
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
		ellia.Draw(modelShader);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		window.Clear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 1. render depth of scene to texture (from light's perspective)
		// --------------------------------------------------------------
		glm::mat4 lightProjection, lightView;
		glm::mat4 lightSpaceMatrix;
		float near_plane = 1.0f, far_plane = 7.5f;
		//lightProjection = glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, near_plane, far_plane); // note that if you use a perspective projection matrix you'll have to change the light position as the current light position isn't enough to reflect the whole scene
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
		lightView = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
		lightSpaceMatrix = lightProjection * lightView;
		// render scene from light's point of view
		simpleDepthShader.use();
		simpleDepthShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		glActiveTexture(GL_TEXTURE0);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		simpleDepthShader.setMat4("model", model);
		ellia.Draw(simpleDepthShader);

		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 26.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		simpleDepthShader.setMat4("model", model);
		terrain.Draw(simpleDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		// reset viewport
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// 2. render scene as normal using the generated depth/shadow map  
		// --------------------------------------------------------------
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shadowMappingShader.use();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = camera.GetViewMatrix();
		shadowMappingShader.setMat4("projection", projection);
		shadowMappingShader.setMat4("view", view);
		// set light uniforms
		shadowMappingShader.setVec3("viewPos", camera.Position);
		shadowMappingShader.setVec3("lightPos", lightPos);
		shadowMappingShader.setMat4("lightSpaceMatrix", lightSpaceMatrix);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, depthMap);
		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.75f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		shadowMappingShader.setMat4("model", model);
		ellia.Draw(shadowMappingShader);

		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 26.0f));
		model = glm::scale(model, glm::vec3(0.02f, 0.02f, 0.02f));
		shadowMappingShader.setMat4("model", model);
		terrain.Draw(shadowMappingShader);

		quad.Draw(quadShader, fboTexture.ID);

		// Render coordination system
		{
			cubeShader.use();

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-1.75f, 0.0f, 0.0f));
			model = glm::scale(model, glm::vec3(0.5f, 0.01f, 0.01f));
			cubeShader.setMat4("model", model);
			cubeShader.setVec4("color", glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
			cube.Draw(cubeShader);

			cubeShader.setMat4("projection", projection);
			cubeShader.setMat4("view", view);
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-2.0f, 0.25f, 0.0f));
			model = glm::scale(model, glm::vec3(0.01f, 0.5f, 0.01f));
			cubeShader.setMat4("model", model);
			cubeShader.setVec4("color", glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
			cube.Draw(cubeShader);

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(-2.0f, 0.0f, 0.25f));
			model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.5f));
			cubeShader.setMat4("model", model);
			cubeShader.setVec4("color", glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));
			cube.Draw(cubeShader);
		}
        
        window.SwapBuffer();

		// change light position over time
		//lightPos.x = sin(glfwGetTime()) * 3.0f;
		//lightPos.z = cos(glfwGetTime()) * 2.0f;
		//lightPos.y = 5.0 + cos(glfwGetTime()) * 1.0f;
    }

	glDeleteFramebuffers(1, &fbo);
    
    return 0;
}
