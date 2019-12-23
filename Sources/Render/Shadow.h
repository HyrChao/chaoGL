#ifndef SHADOW_H
#define SHADOW_H


#include <glad/glad.h>
#include <Render/Material.h>

class Light;

class Shadow
{
public:

	Shadow() = delete;
	~Shadow() = delete;

	static void RenderShadowMap(void(*drawfunc)(Material*));

	static void InitShadow();

	static void SetActiveSunlight(Light* sunlight);

	static glm::mat4& GetShadowProjectionMat();

	static glm::mat4& GetShadowViewMat();

public:

	static float nearplane;
	static float farplane;
	static float shadowdistance;
	static float shadowrange;

	static bool debugShadowView;

	//static unsigned int shadowMap;
	static Texture shadowMap;

	static glm::mat4 lightspaceMat;

private:

	static void PrepareShaderAndTransformMatrix();
	static void RenderScene();
	static void GenerateBuffers();

private:

	static Material* shadowMapingMaterial;
	static bool shadowInitialized;

	static unsigned int shadowFBO;
	static unsigned int shadowRes;

	static Light* activeSunlight;

	static glm::mat4 projectionMat;
	static glm::mat4 viewMat;

};

#endif // !SHADOW_H

