#pragma once
#ifndef LIGHT_H
#define LIGHT_H

#include <list>
#include <Assets/CommonAssets.h>
#include <Render/Material.h>
#include <Render/Camera.h>
#include <glad/glad.h>
#include <Object/Transform.h>



class Light : public Transform
{
public:
    
	enum LightType
	{
		Directional,
		Point,
		Spot
	};

	struct LightParam
	{
		LightType type = LightType::Point;
		glm::vec3 pos;
		glm::vec3 color;
		glm::vec3 dir;
		float constant;
		float linear;
		float quadratic;
		float cutOffAngle;
		float outerCutoffAngle;
	};

    static list<Light*> lights;
    static int maxPointLight;
    LightType type;
    
    float intensity = 1.0f;
    
    glm::vec3 color = glm::vec3(1.0f);
    glm::vec3 dir = glm::vec3(1.0f, 0.0f, 0.0f);
    // pointlight property default 50m, see
    // https://learnopengl-cn.github.io/02%20Lighting/05%20Light%20casters/
    // point light property
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    // spotlight property
    float cutOff = 0.125f;
    float outerCutOff = 0.167f;
public:
    
    Light(LightParam param);
	~Light();

	void SetLightParam(LightParam & param);

	void SetCutoffEulerAbgle(float eulerAngle);

	void SetOuterCutoffEulerAbgle(float eulerAngle);

    void DrawAvatar();
	void SetCutOff(float angle);
	void SetOuterCutOff(float angle);
	float GetCutOffAngle() const;
	float GetOuterCutOffAngle() const;
	static void AddLight(Light* light);
	static void DelLight(Light* light);
	static void ClearAllLights();

    
private:

	glm::vec3 scale = glm::vec3(1, 1, 1);

	glm::mat4 trans = glm::mat4();
	
	unsigned int lightVAO;

	Shader *avatarShader;

	bool lightAvatarInitialized = false;

	// mesh
	


};


#endif
