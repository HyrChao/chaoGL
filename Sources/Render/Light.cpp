#include<Render/Light.h>

list<Light*> Light::lights;
int Light::maxPointLight = 5;

Light::Light(LightParam param)
{

	pos = param.pos;
	type = param.type;
	color = param.color;
	dir = param.dir;
	constant = param.constant;
	linear = param.linear;
	quadratic = param.quadratic;
	SetCutoffEulerAbgle(param.cutOffAngle);
	SetOuterCutoffEulerAbgle(param.outerCutoffAngle);

	AddLight(this);
}

Light::~Light()
{
    
    // Remove from list while delete
    list<Light*>::iterator i = lights.begin();
    
    while (i != lights.end())
    {
        Light *pLight = *i;
        
        if (pLight == this)
        {
            i = lights.erase(i);
        }
        else
        {
            ++i;
        }
    }
}

void Light::SetLightParam(LightParam& param)
{
	pos = param.pos;
	type = param.type;
	color = param.color;
	dir = param.dir;
	constant = param.constant;
	linear = param.linear;
	quadratic = param.quadratic;
	SetCutoffEulerAbgle(param.cutOffAngle);
	SetOuterCutoffEulerAbgle(param.outerCutoffAngle);
}

void Light::SetCutoffEulerAbgle(float eulerAngle)
{
	this->cutOff = cos(glm::radians(eulerAngle));
}

void Light::SetOuterCutoffEulerAbgle(float eulerAngle)
{
	this->outerCutOff = cos(glm::radians(eulerAngle));
}



void Light::DrawAvatar()
{
	if (!lightAvatarInitialized)
	{
		glGenVertexArrays(1, &lightVAO);
		glBindVertexArray(lightVAO);

		glBindBuffer(GL_ARRAY_BUFFER, CommonAssets::instance->cubeVBO);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		// transform , for light avatar
		scale = glm::vec3(0.1f);
		//trans = glm::mat4
		trans = glm::translate(trans, this->pos);
		trans = glm::scale(trans, scale);

		//lightColor = glm::vec3(0.3f, 0.1f, 0.1f);
		//lightColor = glm::vec3(0.1f, 0.0f, 0.0f);

		avatarShader = new Shader("./Shaders/Avatar/Light.vs", "./Shaders/Avatar/Light.fs");

		lightAvatarInitialized = true;
	}

	avatarShader->use();

	avatarShader->setMat4f("model", trans);
	avatarShader->setMat4f("view", Camera::main->viewMat);
	avatarShader->setMat4f("projection", Camera::main->projMat);

	avatarShader->setVec3f("lightColor", color);

	glBindVertexArray(CommonAssets::instance->cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void Light::SetCutOff(float angle)
{
	cutOff = glm::radians(angle);
}

void Light::SetOuterCutOff(float angle)
{
	outerCutOff = glm::radians(angle);
}

float Light::GetCutOffAngle() const
{
	return glm::degrees(cutOff);
}

float Light::GetOuterCutOffAngle() const
{
	return glm::degrees(outerCutOff);
}

void Light::AddLight(Light * light)
{
	lights.push_back(light);
}

void Light::DelLight(Light * light)
{
	list<Light*>::iterator i = lights.begin();

	while (i != lights.end())
	{
		Light *pLight = *i;

		if (pLight == light)
		{
			i = lights.erase(i);
		}
		else
		{
			++i;
		}
	}
}

void Light::ClearAllLight()
{
	list<Light*>::iterator i = lights.begin();

	while (i != lights.end())
	{
		i = lights.erase(i);
	}

	lights.clear();
}








