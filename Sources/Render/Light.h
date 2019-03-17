#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include<Render/Geo.h>
#include<Render/Shader.h>
#include<Application/Camera.h>
#include <glad/glad.h>


class Light
{
public:
	Light(glm::vec3 pos);
	~Light();

	void DrawAvatar();

public:

	glm::vec3 pos = glm::vec3(0, 0, 0);
	glm::vec3 lightColor = glm::vec3(1.0f);

private:


	glm::vec3 scale = glm::vec3(1, 1, 1);

	glm::mat4 trans = glm::mat4();
	
	unsigned int lightVAO;

	Shader *avatarShader;


	// mesh
	


};


#endif
