#pragma once

class Cube {

public:
	Cube();
	~Cube() {};

	void Draw(class Shader& shader);

private:
	unsigned int VAO;
	unsigned int VBO;
	float vertices[288];
};