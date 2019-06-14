#pragma once

class Quad {

public:
	Quad();
	~Quad() {};

	void Draw(class Shader& shader, GLuint textureID);

private:
	unsigned int VAO;
	unsigned int VBO;
	float vertices[24];
};