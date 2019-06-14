#include <glad/glad.h>

#include <Shader.hpp>
#include <Quad.h>

Quad::Quad()
	: vertices{
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  1.0f,  1.0f, 1.0f
	}
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
}

void Quad::Draw(Shader& shader, GLuint textureID)
{
	shader.use();
	glBindMultiTextureEXT(GL_TEXTURE1, GL_TEXTURE_2D, textureID); 	// This equals to glActiveTexture(GL_TEXTURE1); + glBindTexture(GL_TEXTURE_2D, textureID);
	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

