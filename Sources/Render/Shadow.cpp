#include<Render/Shadow.h>

#include <Render/RenderDevice.h>

Material* Shadow::shadowMapingMaterial;
bool Shadow::shadowMapShaderLoaded = false;
glm::mat4 Shadow::lightspaceMat;

unsigned int Shadow::shadowRes = 10;
Texture Shadow::shadowMap;
unsigned int Shadow::shadowFBO;

//float Shadow::farplane = 7.5f;
float Shadow::farplane = 10.0f;
float Shadow::nearplane = 1.0f;

void Shadow::RenderShadowMap(glm::mat4& sunlightSpaceMat, void (*drawfunc)(Material*))
{
	if (!shadowMapShaderLoaded)
	{
		shadowMapingMaterial = new Material("/Shaders/Common/ShadowDepthWrite.vs", "/Shaders/Common/ShadowDepthWrite.fs");
	}

	unsigned int res = glm::pow(2, shadowRes);
	glViewport(0, 0, res, res);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	lightspaceMat = sunlightSpaceMat;
	shadowMapingMaterial->SetParam("lightSpaceMatrix", sunlightSpaceMat);
	// cull front to fix Peter Panning
	glCullFace(GL_FRONT);
	drawfunc(shadowMapingMaterial);
	glCullFace(GL_BACK);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	RenderDevice::ResetViewport();
}

void Shadow::InitShadow()
{
	unsigned int shadowmapRes = glm::pow(2, shadowRes);
	glGenFramebuffers(1, &shadowFBO);

	glGenTextures(1, &shadowMap.id);
	glBindTexture(GL_TEXTURE_2D, shadowMap.id);

	// Generate texture buffer
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowmapRes, shadowmapRes, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float bordercolor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, bordercolor);


	// Use generated buffer as framebuffer's texture buffer
	glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowMap.id, 0);
	// Tell opengl we don't need any color buffer in this framebuffer
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glActiveTexture(0);
}

void Shadow::RenderScene()
{

}



