
#include <Render/Capture.h>
#include <Render/Render.h>

unsigned int Capture::captureFBO;
unsigned int Capture::captureRBO;

glm::mat4 Capture::captureProjection;
glm::mat4 Capture::captureViewMats[6];

Material* Capture::equirectangularToCubemapMaterial;
Material* Capture::irradianceConvolveMaterial;
Material* Capture::specularPrefilterMaterial;
Material* Capture::prefilterBRDFMaterial;

Capture::Capture()
{

}

Capture::~Capture()
{

}

void Capture::InitCapture()
{
	captureViewMats[0] = glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	captureViewMats[1] = glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	captureViewMats[2] = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	captureViewMats[3] = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	captureViewMats[4] = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f));
	captureViewMats[5] = glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f));

	captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

	// Generate new frame buffer to capture cubemap
	glGenFramebuffers(1, &captureFBO);
	glGenRenderbuffers(1, &captureRBO);


	prefilterBRDFMaterial = new Material("/Shaders/Common/IBL_PBR_Prefilter_BRDF.vs", "/Shaders/Common/IBL_PBR_Prefilter_BRDF.fs");
	equirectangularToCubemapMaterial = new Material("/Shaders/Common/HDR_EquirectangularMap.vs", "/Shaders/Common/HDR_EquirectangularMap.fs");
	irradianceConvolveMaterial = new Material("/Shaders/Common/IBL_Irradiance_Convolution.vs", "/Shaders/Common/IBL_Irradiance_Convolution.fs");
	specularPrefilterMaterial = new Material("/Shaders/Common/IBL_PBR_Specular_Convolution.vs", "/Shaders/Common/IBL_PBR_Specular_Convolution.fs");


}

void Capture::CaptureIrradianceCubemap(Texture & envCubemap, Texture & irradianceMap)
{
	int res = 64;

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, res, res);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	irradianceMap.Gen(Texture::TextureType::Irridiance, res, res, Texture::RGB, Texture::Clamp, Texture::Bilinear, true, false);

	RenderDevice::SetViewport(res, res);  // don't forget to set view port to the same demensions before render
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	irradianceConvolveMaterial->ClearTextrues();
	irradianceConvolveMaterial->AddTexture(envCubemap);
	irradianceConvolveMaterial->BindTextures();
	irradianceConvolveMaterial->SetParam("projection", captureProjection);
	for (int i = 0; i < 6; i++)
	{
		irradianceConvolveMaterial->SetParam("view", captureViewMats[i]);
		irradianceConvolveMaterial->use();
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceMap.id, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Draw cube
		glBindVertexArray(CommonAssets::instance->cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	RenderDevice::ResetViewport();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);



}

void Capture::CaptureSpecularPrefilterMap(Texture & envCubemap, Texture & prefilterEnvironmentMap)
{
	//int maxPrefilterMapRes = 128;
	int res = 512;
	int maxMipLevel = 5;

	// generate mip fisrt then replace it
	prefilterEnvironmentMap.Gen(Texture::TextureType::PrefilterEnv, res, res, Texture::RGB, Texture::Clamp, Texture::Trilinear, true, true);

	specularPrefilterMaterial->ClearTextrues();
	specularPrefilterMaterial->AddTexture(envCubemap);
	specularPrefilterMaterial->BindTextures();
	specularPrefilterMaterial->SetParam("projection", captureProjection);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

	for (int mip = 0; mip < maxMipLevel; mip++)
	{
		float currentRoughness = 0.0f;
		currentRoughness = float(mip) / float(maxMipLevel - 1);
		specularPrefilterMaterial->SetParam("roughness", currentRoughness);
		int mipRes = res * std::pow(0.5, mip);

		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipRes, mipRes);
		RenderDevice::SetViewport(mipRes, mipRes);

		for (int i = 0; i < 6; i++)
		{
			specularPrefilterMaterial->SetParam("view", captureViewMats[i]);
			specularPrefilterMaterial->use();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, prefilterEnvironmentMap.id, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Draw cube
			glBindVertexArray(CommonAssets::instance->cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
	}

	RenderDevice::ResetViewport();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Capture::CaptureEnvironmentCubemap(Texture & equirectangularMap, Texture & envCubemap)
{
	int res = 512;


	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, res, res);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

	// Generate cubemap
	envCubemap.Gen(Texture::TextureType::Cube, res, res, Texture::TextureFormat::RGB, Texture::Clamp, Texture::Trilinear, true, false);

	// Load skydome texture
	equirectangularToCubemapMaterial->ClearTextrues();
	equirectangularToCubemapMaterial->AddTexture(equirectangularMap);

	RenderDevice::SetViewport(res, res);  // don't forget to set view port to the same demensions before render
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	// Let's render!
	equirectangularToCubemapMaterial->SetParam("projection", captureProjection);
	for (int i = 0; i < 6; i++)
	{
		equirectangularToCubemapMaterial->SetParam("view", captureViewMats[i]);
		equirectangularToCubemapMaterial->use();
		//https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glFramebufferTexture2D.xml
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap.id, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// Draw cube
		glBindVertexArray(CommonAssets::instance->cubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	}

	RenderDevice::ResetViewport();

	// generate the mipmaps after the cubemap's base texture is set
	envCubemap.GenerateMips();

	// Setback to main farme buffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Capture::PrefilterBRDF(Texture & brdfLUT)
{
	int res = 512;
	// use HDR to get a more precise result
	brdfLUT.Gen(Texture::TextureType::BRDFLUT, res, res, Texture::RG, Texture::Clamp, Texture::Bilinear, true, false);

	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, res, res);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, brdfLUT.id, 0);

	glViewport(0, 0, res, res);
	prefilterBRDFMaterial->use();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	CommonAssets::DrawQuad();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glUseProgram(0);
	RenderDevice::ResetViewport();
}

