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
	 Level();
	~ Level();



	void SetSunLight(Light* sunlight)
	{
		this->sunlight = sunlight;
	}



protected:

	virtual void Loop();

private:

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
		CaptureEnvironmentCubemap();
		CaptureIrradianceCubemap();
		skydomMaterial = new Material("/Shaders/Common/HDR_Cube.vs", "/Shaders/Common/HDR_Cube.fs");
		skydomMaterial->AddTexture(envCubemap);
		//skydomMaterial->AddTexture(irradianceCubemap);

	}

	void DrawSkydome()
	{
		glm::mat4 modelMat = glm::mat4(1.0f);
		////glm::translate(modelMat, glm::vec3(0.0f));
		//glm::vec3 scale = glm::vec3(80);
		//modelMat = glm::scale(modelMat, scale);
		Render::DrawCube(skydomMaterial, modelMat);
	}

	void CaptureIrradianceCubemap()
	{
		int irradianceCubeRes = 32;

		// Generate new frame buffer to capture cubemap
		glGenFramebuffers(1, &captureFBO);
		glGenRenderbuffers(1, &captureRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, irradianceCubeRes, irradianceCubeRes);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);


		irradianceCubemap.useMip = false;
		irradianceCubemap.SetType(TextureType::Irridiance);
		glGenTextures(1, &irradianceCubemap.id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, irradianceCubemap.id);
		
		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, irradianceCubeRes, irradianceCubeRes, 0, GL_RGB, GL_FLOAT, 0);
		}

		// Set texture parameters 
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Render::SetViewport(irradianceCubeRes, irradianceCubeRes);  // don't forget to set view port to the same demensions before render
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);

		irradianceConvolveMaterial->AddTexture(envCubemap);
		irradianceConvolveMaterial->BindTextures();
		irradianceConvolveMaterial->SetParam("projection", captureProjection);
		for (int i = 0; i < 6; i++)
		{
			irradianceConvolveMaterial->SetParam("view", captureViewMats[i]);
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

	void CaptureEnvironmentCubemap()
	{
		// Generate new frame buffer to capture cubemap
		glGenFramebuffers(1, &captureFBO);
		glGenRenderbuffers(1, &captureRBO);

		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		glBindRenderbuffer(GL_RENDERBUFFER, captureRBO);

		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, captureRBO);

		// Generate cubemap

		glGenTextures(1, &envCubemap.id);
		glBindTexture(GL_TEXTURE_CUBE_MAP, envCubemap.id);
		envCubemap.SetType(TextureType::Cube);
		envCubemap.useMip = false;

		for (int i = 0; i < 6; i++)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, 0);
		}

		// Set texture parameters 
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Load skydome texture
		LoadEquirectangularSkydomeTexture();
		equirectangularToCubemapMaterial->AddTexture(equirectangularSkyTex);

		Render::SetViewport(512, 512);  // don't forget to set view port to the same demensions before render
		glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
		// Let's render!
		equirectangularToCubemapMaterial->SetParam("projection", captureProjection);
		for (int i = 0; i < 6; i++)
		{
			equirectangularToCubemapMaterial->SetParam("view", captureViewMats[i]);
			//https://www.khronos.org/registry/OpenGL-Refpages/es2.0/xhtml/glFramebufferTexture2D.xml
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, envCubemap.id, 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			// Draw cube
			glBindVertexArray(CommonAssets::instance->cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}

		Render::ResetViewport();
		// Setback to main farme buffer
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

protected:

	// Skydome texture
	Texture equirectangularSkyTex = Texture("/Assets/Texture/HDR/Mans_Outside_2k.hdr", TextureType::Equirectangular, false);

	// Sun light (Main directional light)
	Light* sunlight;

	Material* skydomMaterial;

	bool initialized = false;

	Texture envCubemap;
	Texture irradianceCubemap;

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

	// shader for convert equirectangular map tp cubmap 
	Material* equirectangularToCubemapMaterial;
	// shader for convolve irradiance
	Material* irradianceConvolveMaterial;
};

#endif // !LEVEL_H

