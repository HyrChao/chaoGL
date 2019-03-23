#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include<Render/Geo.h>
#include<Render/Shader.h>
#include<Render/Camera.h>
#include <glad/glad.h>
#include <Object/BaseObject.h>


class Light : public BaseObject
{
public:
    Light(glm::vec3 pos);
	~Light();

    void DrawAvatar() const;

public:
    
    glm::vec3 color = glm::vec3(1.0f);
    glm::vec4 dir = glm::vec4(1.0f);

private:


	glm::vec3 scale = glm::vec3(1, 1, 1);

	glm::mat4 trans = glm::mat4();
	
	unsigned int lightVAO;

	Shader *avatarShader;


	// mesh
	


};


#endif
