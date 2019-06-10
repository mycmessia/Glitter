#pragma once

class Quad {

public:
	Quad();
	~Quad() {};

	void Draw(class Shader& shader, GLuint textureID);

private:
	unsigned int quadVAO;
	unsigned int quadVBO;
	float quadVertices[24];
};