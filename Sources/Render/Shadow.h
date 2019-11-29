#ifndef SHADOW_H
#define SHADOW_H


#include <glad/glad.h>
#include <Render/Material.h>

class Shadow
{
public:

	Shadow() = delete;
	~Shadow() = delete;

	static void RenderShadowMap(glm::mat4& sunlightSpaceMat, void(*drawfunc)(Material*));

	static void InitShadow();

	static float nearplane;
	static float farplane;

	//static unsigned int shadowMap;
	static Texture shadowMap;

private:

	static void PrepareShaderAndTransformMatrix();
	static void RenderScene();
	static void GenerateBuffers();

	static Material* shadowMapingMaterial;
	static bool shadowMapShaderLoaded;

	static unsigned int shadowFBO;
	static unsigned int shadowRes;

};

#endif // !SHADOW_H

