#include <glad/glad.h>

#include <Shader.hpp>
#include <Quad.h>

Quad::Quad()
	: quadVertices{
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  1.0f,  1.0f, 1.0f
	}
{
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Quad::Draw(Shader& shader, GLuint textureID)
{
	shader.use();
	glBindMultiTextureEXT(GL_TEXTURE1, GL_TEXTURE_2D, textureID); 	// This equals to glActiveTexture(GL_TEXTURE1); + glBindTexture(GL_TEXTURE_2D, textureID);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

