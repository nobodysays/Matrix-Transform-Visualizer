#pragma once
#include "helpfile.h"

class Object
{
protected: 
	class Shader* shader = nullptr;
	glm::vec3 position = glm::vec3(0.0);
	glm::vec3 size = glm::vec3(1.0);
	GLuint VAO=0;
public:
	virtual void Update() = 0;
};