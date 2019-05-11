#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include<Render/Geo.h>
#include<Render/Shader.h>
#include<Render/Camera.h>
#include <glad/glad.h>
#include <Object/Transform.h>


class Light : public Transform
{
public:
    Light(glm::vec3 pos);
	~Light();

    void DrawAvatar() const;
    void SetCutOff(float angle)
    {
        cutOff = glm::radians(angle);
    }
    void SetOuterCutOff(float angle)
    {
        outerCutOff = glm::radians(angle);
    }
    float GetCutOffAngle() const
    {
        return glm::degrees(cutOff);
    }
    float GetOuterCutOffAngle() const
    {
        return glm::degrees(outerCutOff);
    }
    

public:
    
    float intensity = 1.0f;
    
    glm::vec3 color = glm::vec3(1.0f);
    glm::vec3 dir = glm::vec3(1.0f);
    // pointlight property default 50m, see
    // https://learnopengl-cn.github.io/02%20Lighting/05%20Light%20casters/
    // point light property
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    // spotlight property
    float cutOff = 0.125f;
    float outerCutOff = 0.167f;
    
private:


	glm::vec3 scale = glm::vec3(1, 1, 1);

	glm::mat4 trans = glm::mat4();
	
	unsigned int lightVAO;

	Shader *avatarShader;


	// mesh
	


};


#endif
