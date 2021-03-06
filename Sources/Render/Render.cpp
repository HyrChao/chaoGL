#include<Render/Render.h>
#include <Render/Model.h>
#include <Application/Application.h>	
#include <Render/RenderDevice.h>
#include "Render/PostFX.h"
glm::vec4 Render::clearColor;
glm::mat4 Render::projectMat;
glm::mat4 Render::viewMat;
glm::vec3 Render::viewPos;


Light* Render::sunlight;
Shader* Render::framebufferDebugShader;
bool Render::framebufferDebugInitialized = false;

DrawableList* Render::currentDrawablelist = nullptr;


void Render::PrepareRender()
{
	// enable Z test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);;

	// filter across cubemap faces
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);

	Shadow::InitShadow();
	Capture::InitCapture();
	RenderDevice::InitMainFrameBuffer();
}

void Render::DrawOnFrameBegin()
{    
    // Update camera
    Camera::main->UpdateCamera();
    
    // move model forward equals to move view backward
    viewMat = Camera::main->viewMat;
    projectMat = Camera::main->projMat;
    viewPos = Camera::main->pos;
    
	// Refresh shader params
	UpdateShaderLightParams();
	UpdateShaderCameraVP();

	RenderDevice::UpdateMainFrameBuffer();

}

void Render::Draw()
{
	// Render shadow if has sunlight
	if (sunlight != nullptr)
	{
		Shadow::SetActiveSunlight(sunlight);
		Shadow::RenderShadowMap(Render::ExcuteDrawlistWithReplacedMaterial);
	}

	ExcuteMainDrawlist();
}

void Render::DrawOnFrameEnd()
{
	// Draw light avatars
	DrawLightAvatars();

	// draw quad & post process
	PostFXManager::DrawPostFX();
	
	// clear
	ClearCurrentDrawableList();
}


//void Render::Draw(IDrawable* drawable, Material* material, glm::mat4 modelMat)
//{
//	DrawableContext context(material, modelMat);
//	currentDrawablelist->insert(std::make_pair(drawable, context));
//}

void Render::AddToCurrentDrawableList(IDrawable* drawable, Material* material, glm::mat4* model)
{
	DrawableContext context(drawable, material, model);
	currentDrawablelist->AddContext(context);
}

void Render::BindCurrentDrawableList(DrawableList & list)
{
	currentDrawablelist = &list;
}

void Render::UnbindCurrentDrawableList()
{
	currentDrawablelist = nullptr;
}

void Render::ClearCurrentDrawableList()
{
	currentDrawablelist->clear();
}

void Render::ExcuteMainDrawlist()
{
	if (!currentDrawablelist)
		return;

	DrawableList list = (*currentDrawablelist);

	if (list.begin() == list.end())
		return;

	for (auto i = list.begin(); i != list.end(); i++)
	{
		DrawableContext context = (*i).second;
		IDrawable* drawable = (*i).second.drawable;

		Material* mtl = context.material;
		if (Shadow::debugShadowView)
		{
			// FOR DEBUG SHADOW
			glm::mat4 lightProject = Shadow::GetShadowProjectionMat();
			glm::mat4 lightView = Shadow::GetShadowViewMat();
			mtl->SetParam("projection", lightProject);
			mtl->SetParam("view", lightView);
		}
		drawable->Draw(context.material);
	}
}

void Render::ExcuteDrawlistWithReplacedMaterial(Material * material)
{
	if (!currentDrawablelist)
		return;

	DrawableList list = (*currentDrawablelist);

	if (list.begin() == list.end())
		return;

	for (auto i = list.begin(); i != list.end(); i++)
	{
		DrawableContext context = (*i).second;
		IDrawable* drawable = (*i).second.drawable;
		drawable->Draw(material);
	}
}


void Render::UpdateShaderLightParams()
{
	for (unordered_map<int, Shader*>::iterator it = Shader::activeShaders.begin(); it != Shader::activeShaders.end(); it++)
	{
		Shader* shader = it->second;
		if (!shader->isProp)
			continue;
		shader->use();
		list<Light*>::iterator i = Light::lights.begin();
		int pointLightNum = 0;
		while (i != Light::lights.end())
		{
			Light *light = *i;

			if (light->type == Light::LightType::Directional)
			{
				shader->setVec3f("dirLight.direction", light->dir);
				shader->setVec3f("dirLight.ambient", 0.02f * light->color);
				shader->setVec3f("dirLight.diffuse", 0.5f * light->color);
				shader->setVec3f("dirLight.specular", 1.0f * light->color);
				shader->setVec3f("dirLight.irradiance", 5.0f * light->color);
			}
			else if (light->type == Light::LightType::Point && pointLightNum < Light::maxPointLight)
			{
				std::string num = std::to_string(pointLightNum);
				shader->setVec3f("pointLights[" + num + "].position", light->GetPos());
				shader->setFloat("pointLights[" + num + "].constant", light->constant);
				shader->setFloat("pointLights[" + num + "].linear", light->linear);
				shader->setFloat("pointLights[" + num + "].quadratic", light->quadratic);
				shader->setVec3f("pointLights[" + num + "].ambient", 0.01f * light->color);
				shader->setVec3f("pointLights[" + num + "].diffuse", 0.5f * light->color);
				shader->setVec3f("pointLights[" + num + "].specular", 0.8f * light->color);
				shader->setVec3f("pointLights[" + num + "].irradiance", 5.0f * light->color);

				pointLightNum++;
			}
			else if (light->type == Light::LightType::Spot)
			{
				shader->setVec3f("spotLight.position", light->GetPos());
				shader->setVec3f("spotLight.direction", light->dir);
				shader->setFloat("spotLight.cutOff", light->cutOff);  // cutoff is cosine of angle
				shader->setFloat("spotLight.outerCutOff", light->outerCutOff);
				shader->setVec3f("spotLight.ambient", 0.01f * light->color);
				shader->setVec3f("spotLight.diffuse", 0.8f * light->color);
				shader->setVec3f("spotLight.specular", 0.5f * light->color);
				shader->setVec3f("spotLight.irradiance", 5.0f * light->color);
			}
			else
			{

			}

			i++;

		}
	}
}

void Render::UpdateShaderCameraVP()
{
	for (unordered_map<int, Shader*>::iterator it = Shader::activeShaders.begin(); it != Shader::activeShaders.end(); it++)
	{
		Shader* shader = it->second;

		if (!shader->isProp)
			continue;

		shader->use();
		shader->setMat4f("view", viewMat);
		shader->setMat4f("projection", projectMat);
		shader->setVec3f("viewPos", viewPos);
	}
}

void Render::SetMaterialCameraVP(Material * material)
{
	material->SetParam("view", viewMat);
	material->SetParam("projection", projectMat);
	material->SetParam("viewPos", viewPos);
}

void Render::SetVertexShaderParams(Shader * shader, glm::mat4 model)
{
	shader->setMat4f("view", viewMat);
	shader->setMat4f("projection", projectMat);
	shader->setVec3f("viewPos", viewPos);
	shader->setMat4f("model", model);
}


void Render::SetClearColor(glm::vec4 color)
{
	clearColor = color;
}

void Render::WireframeMode(bool on)
{
	if (on)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}


// Debug frame buffer texture

void Render::DisplayFramebufferTexture(Texture texture)
{
	unsigned int textureID = texture.id;

	if (!framebufferDebugInitialized)
	{
		framebufferDebugShader = new Shader("/Shaders/Debug/Debug_Framebuffer.vs", "/Shaders/Debug/Debug_Framebuffer.fs");

		framebufferDebugInitialized = true;
	}
	glm::vec2 anchor = glm::vec2(0.8, 0.8);
	glm::vec2 scale = glm::vec2(0.3);
	glActiveTexture(GL_TEXTURE0);
	framebufferDebugShader->use();
	framebufferDebugShader->setVec2f("anchor", anchor);
	framebufferDebugShader->setVec2f("scale", scale);
	//framebufferDebugShader->setMat4f("view", viewMat);
	//framebufferDebugShader->setMat4f("projection", projectMat);
	framebufferDebugShader->setInt("bufferTex", 0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	CommonAssets::DrawQuad();
	glUseProgram(0);
}

void Render::DrawLightAvatars() 
{
	// Draw light avatar to scene
    list<Light*>::iterator i = Light::lights.begin();
    while (i != Light::lights.end())
    {
        Light *light = *i;
        light->DrawAvatar();
		i++;
    }
}
