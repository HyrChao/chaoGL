#include<Render/Render.h>

Render* Render::render;
glm::vec4 Render::clearColor;
glm::mat4 Render::projectMat;
glm::mat4 Render::viewMat;
glm::vec3 Render::viewPos;

Render::Render()
{
	if (render == nullptr)
	{
		render = this;
	}
	else
	{
		delete this;
	}

	if (Camera::main == nullptr)
	{
		Camera::main = new Camera();
	}

	SetupRenderProperty();
}

Render::~Render()
{
	//delete this;
}

void Render::SetupRenderProperty()
{
	// enable Z test
	glEnable(GL_DEPTH_TEST);
}

void Render::DrawOnFrameBegin()
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
}

void Render::DrawOnFrameEnd()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Render::SetShaderLightParams(Shader *shader)
{
	shader->setFloat("material.shininess", 32.0f);
    list<Light*>::iterator i = Light::lights.begin();
	int pointLightNum = 0;
    while (i != Light::lights.end())
    {
        Light *light = *i;
        if (light->type == LightType::Directional)
        {
            shader->setVec3f("dirLight.direction", light->dir);
            shader->setVec3f("dirLight.ambient", 0.02f * light->color);
            shader->setVec3f("dirLight.diffuse", 0.5f * light->color);
			shader->setVec3f("dirLight.specular", 1.0f * light->color);
			shader->setVec3f("dirLight.irradiance", 5.0f * light->color);
        }
        else if (light->type == LightType::Point && pointLightNum < Light::maxPointLight)
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
        else if (light->type == LightType::Spot)
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


