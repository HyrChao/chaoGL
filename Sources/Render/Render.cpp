#include<Render/Render.h>

glm::vec4 Render::clearColor;
glm::mat4 Render::projectMat;
glm::mat4 Render::viewMat;
glm::vec3 Render::viewPos;

glm::mat4 Render::sunlightSpaceMatrix;

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


}

void Render::ExcuteDrawOnFrameBegin()
{
	//// Clear
	//float timeValue = glfwGetTime();
	//glClearColor(0.8f + 0.2*sin(timeValue), 0.8f + 0.2*sin(timeValue + 3.14 / 3), 0.8f + 0.2*sin(timeValue - 3.14 / 3), 1.0f);
	glClearColor(clearColor.x,clearColor.y,clearColor.z, clearColor.w);
	glClear(GL_COLOR_BUFFER_BIT);
    
    // Update camera
    Camera::main->UpdateCamera();
    
    // move model forward equals to move view backward
    viewMat = Camera::main->viewMat;
    projectMat = Camera::main->projMat;
    viewPos = Camera::main->pos;
    
    // Update light
    UpdateLight();

	// Refresh shader params
	UpdateShaderLightParams();
	UpdateShaderCameraVP();

	// Render shadow if has sunlight
	if (sunlight != nullptr)
	{
		glm::mat4 lightProject = glm::ortho(-Shadow::shadowrange, Shadow::shadowrange, -Shadow::shadowrange, Shadow::shadowrange, Shadow::nearplane, Shadow::farplane);
		//glm::mat4 lightProject = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, Shadow::nearplane, Shadow::farplane);
		glm::mat4 lightView = glm::lookAt(glm::normalize(sunlight->dir) * Shadow::shadowdistance, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		sunlightSpaceMatrix = lightProject * lightView;

		Shadow::RenderShadowMap(sunlightSpaceMatrix, Render::ExcuteDrawlistWithMaterial);
	}
}

void Render::ExcuteDraw()
{
	ExcuteMainDrawlist();
}

void Render::ExcuteDrawOnFrameEnd()
{
}


//void Render::Draw(IDrawable* drawable, Material* material, glm::mat4 modelMat)
//{
//	DrawableContext context(material, modelMat);
//	currentDrawablelist->insert(std::make_pair(drawable, context));
//}

void Render::AddToCurrentDrawableList(IDrawable* drawable, Material* material, glm::mat4 modelMat)
{
	DrawableContext context(material, modelMat);
	currentDrawablelist->insert(std::make_pair(drawable, context));
	(*currentDrawablelist)[drawable] = context;
}

void Render::BindCurrentDrawableList(DrawableList & list)
{
	currentDrawablelist = &list;
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
		IDrawable* drawable = (*i).first;

		Material* mtl = context.material;
		// FOR DEBUG SHADOW
		//glm::mat4 lightProject = glm::ortho(-Shadow::shadowdistance, Shadow::shadowdistance, -Shadow::shadowdistance, Shadow::shadowdistance, Shadow::nearplane, Shadow::farplane);
		//glm::mat4 lightView = glm::lookAt(sunlight->dir * Shadow::shadowdistance, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//mtl->SetParam("projection", lightProject);
		//mtl->SetParam("view", lightView);
		drawable->Draw(context.material, context.modelMat);
	}
}

void Render::ExcuteDrawlistWithMaterial(Material * material)
{
	if (!currentDrawablelist)
		return;

	DrawableList list = (*currentDrawablelist);

	if (list.begin() == list.end())
		return;

	for (auto i = list.begin(); i != list.end(); i++)
	{
		DrawableContext context = (*i).second;
		IDrawable* drawable = (*i).first;
		drawable->Draw(material, context.modelMat);
	}
}

void Render::UpdateShaderLightParams()
{
	for (unordered_map<int, Shader*>::iterator it = Shader::loadedShaders.begin(); it != Shader::loadedShaders.end(); it++)
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
	for (unordered_map<int, Shader*>::iterator it = Shader::loadedShaders.begin(); it != Shader::loadedShaders.end(); it++)
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


void Render::SetShaderLightParams(Shader *shader)
{
	shader->setFloat("material.shininess", 32.0f);
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

void Render::DrawGeo(unsigned int geoVAO, Shader * shader, glm::mat4 & model)
{
	shader->use();
	Render::SetVertexShaderParams(shader, model);
	Render::SetShaderLightParams(shader);
	glBindVertexArray(geoVAO);
	glDrawElements(GL_TRIANGLE_STRIP, geoVAO, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Render::DrawCube(Shader * shader, glm::mat4 & model)
{
	shader->use();
	glBindVertexArray(CommonAssets::instance->cubeVAO);
	Render::SetVertexShaderParams(shader, model);
	Render::SetShaderLightParams(shader);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Render::DrawSphere(Shader * shader, glm::mat4 & model)
{
	shader->use();
	Render::SetVertexShaderParams(shader, model);
	Render::SetShaderLightParams(shader);
	glBindVertexArray(CommonAssets::instance->sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, CommonAssets::instance->sphereIndexCount, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
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


void Render::UpdateLight() {

    //lightCol = Color::GetHue(lightCol, 1, 1);
    list<Light*>::iterator i = Light::lights.begin();
    while (i != Light::lights.end())
    {
        Light *light = *i;
        light->DrawAvatar();
		i++;
    }
}
