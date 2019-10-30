#pragma once

#ifndef  LEVEL_H
#define LEVEL_H

#include <glad/glad.h>
#include <Render/Color.h>
#include <Render/Model.h>
#include<Render/Render.h>

class  Level
{
public:
	 
	 Level()
	 {
		 GlobalInitialize();
		 LoadEquirectangularSkydomeTexture();
		 LevelInitialize();
	 }
	 Level(string skyHDRTexture_path)
	 {
		GlobalInitialize();
		equirectangularSkyTex.path = skyHDRTexture_path;
		LoadEquirectangularSkydomeTexture();
		LevelInitialize();
	 }

	~ Level();

	void Reset()
	{
		initialized = false;
	}

	void SetSunLight(Light* sunlight)
	{
		this->sunlight = sunlight;
	}



protected:

	virtual void Loop();

	void ChangeEnvironment()
	{
		skydomMaterial->ClearTextrues();
		skydomMaterial->AddTexture(envCubemap);
	}

private:


	void GlobalInitialize()
	{
		if (!globalInitialized)
		{
			skydomeShader = new Shader("/Shaders/Common/Cube_Skydome.vs", "/Shaders/Common/Cube_Skydome.fs");
			skydomMaterial = new Material(skydomeShader);
			skydomMaterial->AddTexture(envCubemap);
			globalInitialized = true;
		}
	}

	void LevelInitialize()
	{
		// Generate new frame buffer to capture cubemap
		glGenFramebuffers(1, &captureFBO);
		glGenRenderbuffers(1, &captureRBO);

		equirectangularToCubemapMaterial = new Material("/Shaders/Common/HDR_EquirectangularMap.vs", "/Shaders/Common/HDR_EquirectangularMap.fs");
		irradianceConvolveMaterial = new Material("/Shaders/Common/IBL_Irradiance_Convolution.vs", "/Shaders/Common/IBL_Irradiance_Convolution.fs");
		specularPrefilterMaterial = new Material("/Shaders/Common/IBL_PBR_Specular_Convolution.vs", "/Shaders/Common/IBL_PBR_Specular_Convolution.fs");
		prefilterBRDFMaterial = new Material("/Shaders/Common/IBL_PBR_Prefilter_BRDF.vs", "/Shaders/Common/IBL_PBR_Prefilter_BRDF.fs");

		CaptureEnvironmentCubemap();
		CaptureIrradianceCubemap();
		CaptureSpecularPrefilterMap();
		PrefilterBRDF();
	}

	void LoadEquirectangularSkydomeTexture()
	{
		if (equirectangularSkyTex.id == 0)
		{
			unsigned int textureID = AssetsManager::CubeTextureFromFile(equirectangularSkyTex.path.c_str());
			equirectangularSkyTex.id = textureID;
		}
	}

	void SetSkyDome()
	{

	}

	void DrawSkydome()
	{
		glm::mat4 modelMat = glm::mat4(1.0f);
		skydomMaterial->SetModelMat(modelMat);
		skydomMaterial->use();
		Render::DrawCube();
	}

	void CaptureIrradianceCubemap()
	{
		int res = 64;

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, res, res);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

		irradianceCubemap.Gen(TextureType::Irridiance, res, res, RGB, Clamp, Bilinear, true, false);

		Render::SetViewport(res, res);  // don't forget to set view port to the same demensions before render
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

		irradianceConvolveMaterial->AddTexture(envCubemap);
		irradianceConvolveMaterial->BindTextures();
		irradianceConvolveMaterial->SetParam("projection", captureProjection);
		for (int i = 0; i < 6; i++)
		{
			irradianceConvolveMaterial->SetParam("view", captureViewMats[i]);
			irradianceConvolveMaterial->use();
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, irradianceCubemap.id, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Draw cube
			glBindVertexArray(CommonAssets::instance->cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		Render::ResetViewport();
		glBindFramebuffer(GL_FRAMEBUFFER ,0);

	}

	void CaptureSpecularPrefilterMap()
	{
		//int maxPrefilterMapRes = 128;
		int res = 512;
		int maxMipLevel = 5;

		// generate mip fisrt then replace it
		prefilterEnvironmentMap.Gen(TextureType::PrefilterEnv, res, res, RGB, Clamp, Trilinear, true, true);

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
			Render::SetViewport(mipRes, mipRes);

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

		Render::ResetViewport();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

	}

	void CaptureEnvironmentCubemap()
	{
		int res = 512;

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, res, res);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

		// Generate cubemap
		envCubemap.Gen(TextureType::Cube, res, res, TextureFormat::RGB, TextureRepeatMode::Clamp, TextureFilterMode::Trilinear, true, false);

		// Load skydome texture
		equirectangularToCubemapMaterial->AddTexture(equirectangularSkyTex);

		Render::SetViewport(res, res);  // don't forget to set view port to the same demensions before render
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

		Render::ResetViewport();

		// generate the mipmaps after the cubemap's base texture is set
		envCubemap.GenerateMips();

		// Setback to main farme buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void PrefilterBRDF()
	{
		int res = 512;
		// use HDR to get a more precise result
		prefilterBRDFLUT.Gen(TextureType::BRDFLUT, res, res, RG, Clamp, Bilinear, true, false);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, res, res);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, prefilterBRDFLUT.id, 0);

		glViewport(0, 0, res, res);
		prefilterBRDFMaterial->use();
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		Render::DrawQuad();
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glUseProgram(0);
		Render::ResetViewport();
	}


protected:

	// Skydome texture
	//Texture equirectangularSkyTex = Texture("/Assets/Texture/HDR/Mans_Outside_2k.hdr", TextureType::Equirectangular, false);
	Texture equirectangularSkyTex = Texture("/Assets/Texture/HDR/Ridgecrest_Road_Ref.hdr", TextureType::Equirectangular, false);

	// Sun light (Main directional light)
	Light* sunlight;

	static Material* skydomMaterial;

	bool resourceLoaded = false;
	bool initialized = false;

	Texture envCubemap;
	Texture irradianceCubemap;
	Texture prefilterEnvironmentMap;
	Texture prefilterBRDFLUT;

private:

	static Shader* skydomeShader;
	static bool globalInitialized;

	unsigned int captureFBO, captureRBO;

	// capture view mats from origin
	glm::mat4 captureViewMats[6] =
	{
		glm::lookAt(glm::vec3(0.0f), glm::vec3(1.0f, 0.0f,0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)),
		glm::lookAt(glm::vec3(0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f))
	};
	
	// captue projection mat
	glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);

	// material for convert equirectangular map tp cubmap 
	Material* equirectangularToCubemapMaterial;
	// material for convolve irradiance
	Material* irradianceConvolveMaterial;
	// material for convolve specular 
	Material* specularPrefilterMaterial;
	// material for prefilter BRDF
	Material* prefilterBRDFMaterial;
};

#endif // !LEVEL_H

