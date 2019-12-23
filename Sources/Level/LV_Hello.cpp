#include<Level/LV_Hello.h>

LV_Hello::LV_Hello()
{
}

LV_Hello::~LV_Hello()
{

}

void LV_Hello::Loop()
{
	Level::Loop();

	if (Input::GetKeyOnce(GLFW_KEY_TAB))
	{
		currentHelloScene++;

		if (currentHelloScene > (LastScene - 1))
			currentHelloScene = 0;
	}

	switch (currentHelloScene)
	{
	default:
		break;
	case S_HelloLight:
		HelloLight();
		break;
	case S_HelloTriangle:
		HelloTriangle();
		break;
	case S_HelloTransform:
		HelloTransform();
		break;
	case S_HelloProjection:
		HelloProjection();
		break;
	case S_HelloBox:
		HelloBox();
		break;
	case S_HelloCamera:
		HelloCamera();
		break;
	}
}

void LV_Hello::OnGui()
{
	Level::OnGui();
}

void LV_Hello::Initialize()
{
	Level::Initialize();
	//----------------------------------
	//Shader
	helloTriShader = make_unique<Shader>("/Shaders/Vertex/HelloTriangle.vs", "/Shaders/Fragment/HelloTriangle.fs");
	helloTexShader = make_unique<Shader>("/Shaders/Vertex/HelloTexture.vs", "/Shaders/Fragment/HelloTexture.fs");
	helloProjShader = make_unique<Shader>("/Shaders/Vertex/HelloProjection.vs", "/Shaders/Fragment/HelloProjection.fs");

	// load tecture
	LoadTexture();
}


void LV_Hello::HelloTriangle()
{
	// Use shader program
	// Update color with time
	float timeValue = glfwGetTime();
	float redVal = (cos(timeValue) / 2.0f) + 0.5f;
	float greenVal = (cos(timeValue + 3.14 / 3) / 2.0f) + 0.5f;
	float blueVal = (cos(timeValue - 3.14 / 3) / 2.0f) + 0.5f;
    glm::vec3 lightCol = glm::vec3(redVal, greenVal, blueVal);
    lightCol = Color::GetHue(lightCol, 0.5, 1);
	helloTriShader->use();
	helloTriShader->setVec4f("ourColor", lightCol.r, lightCol.g, lightCol.b, 1.0f);
	helloTriShader->use();
	//Triangle draw test
	glBindVertexArray(CommonAssets::instance->triVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void LV_Hello::HelloTransform()
{
	helloTexShader->use();
	//glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_2);

	glBindVertexArray(CommonAssets::instance->recVAO);
	Transform(helloTexShader.get(),glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.5, 0.5, 0.5));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	Transform(helloTexShader.get(),glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.5, 0.5, 0.5));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	Transform(helloTexShader.get(),glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.5, 0.5, 0.5));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	Transform(helloTexShader.get(),glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5, 0.5, 0.5));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

}

void LV_Hello::HelloProjection()
{
	helloProjShader->use();
	// ortho matrix
	//glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
	glm::mat4 model;
	model = glm::rotate(model, glm::radians(55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view;
	// move model forward equals to move view backward
	view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

	glm::mat4 projection;
	projection = Camera::main->projMat;

	helloProjShader->setMat4f("model", model);
	helloProjShader->setMat4f("view", view);
	helloProjShader->setMat4f("projection", projection);
	Transform(helloProjShader.get(),glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.5, 1.5, 1.5));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_2);
	glBindVertexArray(CommonAssets::instance->cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

}

void LV_Hello::HelloBox()
{
	helloProjShader->use();
	// ortho matrix
	//glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
	glm::mat4 model;
	model = glm::rotate(model, glm::radians(55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view;
	// move model forward equals to move view backward
	view = Camera::main->viewMat;

	glm::mat4 projection;
	projection = Camera::main->projMat;

	helloProjShader->setMat4f("model", model);
	helloProjShader->setMat4f("view", view);
	helloProjShader->setMat4f("projection", projection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_2);
	glBindVertexArray(CommonAssets::instance->cubeVAO);
	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle)+(float)glfwGetTime(), glm::vec3(glm::sin((float)glfwGetTime()), glm::cos((float)glfwGetTime()), -glm::cos((float)glfwGetTime())));
		helloProjShader->setMat4f("model", model);

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
}

void LV_Hello::HelloCamera()
{
	Camera::main->CameraAutoSpan();

	HelloBox();
}

void LV_Hello::HelloLight()
{

    if (!helloLightInitialized)
    {
		Light::ClearAllLights();

		helloLightShader = make_unique<Shader>("/Shaders/Vertex/HelloLight.vs", "/Shaders/Fragment/HelloLight.fs");

		helloLightShader->use(); // set uniform！for texture
		helloLightShader->setInt("material.diffuse_1", 0);
		helloLightShader->setInt("material.diffuse_2", 1);
		helloLightShader->setInt("material.specular", 2);

		Light::LightParam pointLightp1;
		pointLightp1.type = Light::LightType::Point;
		pointLightp1.pos = glm::vec3(0.0f, 3.0f, 0.0f);
		pointLightp1.color = glm::vec3(0.6f, 0.2f, 0.2f);
		pointLightp1.constant = 1.0f;
		pointLightp1.linear = 0.09f;
		pointLightp1.quadratic = 0.032f;
		Light* pointLight1 = new Light(pointLightp1);

		Light::LightParam pointLightp2;
		pointLightp2.type = Light::LightType::Point;
		pointLightp2.pos = glm::vec3(-5.0f, -3.0f, -5.0f);
		pointLightp2.color = glm::vec3(0.1f, 0.8f, 0.1f);
		pointLightp2.constant = 1.0f;
		pointLightp2.linear = 0.7f;
		pointLightp2.quadratic = 1.8f;
		Light* pointLight2 = new Light(pointLightp2);

		Light::LightParam dirlightp1;
		dirlightp1.type = Light::LightType::Directional;
		dirlightp1.pos = glm::vec3(10.f, 10.f, 10.f);
		dirlightp1.color = glm::vec3(1.0f, 1.0f, 1.0f);
		dirlightp1.dir = glm::vec3(-1, -1, -1);;
		dirLight = new Light(dirlightp1);

		Light::LightParam spotlightp1;
		spotlightp1.type = Light::LightType::Spot;
		spotlightp1.pos = glm::vec3(-3.0f, -1.0f, -8.0f);
		spotlightp1.color = glm::vec3(1.0f, 1.0f, 0.1f);
		spotlightp1.dir = glm::vec3(3.0f, 1.0f, 8.0f);
		spotlightp1.cutOffAngle = 30.0f;
		spotlightp1.outerCutoffAngle = 50.0f;
		Light* spotlight = new Light(spotlightp1);

        helloLightInitialized = true;

    }
    glm::vec4 clearColor = glm::vec4(0.0f);
    Render::SetClearColor(clearColor);
    
    // change light color among with time
    float timeval1 = (glm::sin(Time::time) + 3)/4;
    float timeval2 = (glm::cos(Time::time) + 3)/4;
    float timeval3 = (glm::sin(Time::time + 3.14f) + 3)/4;
    glm::vec3 lightCol = glm::vec3(timeval1, timeval2, timeval3);
 
    dirLight->color = lightCol;
  
	helloLightShader->use();

	SetShaderLightParams(helloLightShader.get());
	// ortho matrix
	//glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
	//glm::mat4 model;
	//model = glm::rotate(model, glm::radians(55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	//helloLightShader->setMat4f("model", model);
    helloLightShader->setMat4f("view", Render::viewMat);
    helloLightShader->setMat4f("projection", Render::projectMat);
    helloLightShader->setVec3f("viewPos", Render::viewPos);

	helloLightShader->setVec3f("material.ambient", 1.0f, 0.5f, 0.31f);
	helloLightShader->setVec3f("material.diffuse", 1.0f, 0.5f, 0.31f);
	helloLightShader->setVec3f("material.specular", 0.5f, 0.5f, 0.5f);
	helloLightShader->setFloat("material.shininess", 32.0f);


	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_2);
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, specularTex);
	glBindVertexArray(CommonAssets::instance->cubeVAO);
	for (unsigned int i = 0; i < 10; i++)
	{
        
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, cubePositions[i]);
        float angle = 20.0f * i;
        glm::vec3 axis = glm::vec3(glm::sin((float)glfwGetTime()), glm::cos((float)glfwGetTime()), -glm::cos((float)glfwGetTime()));
        axis = glm::normalize(axis);
        model = glm::rotate(model, glm::radians(angle) + (float)glfwGetTime(), axis);
        helloLightShader->setMat4f("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
    }
	glBindVertexArray(0);

}



void LV_Hello::Transform(Shader* shader,glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale)
{
	glm::mat4 trans;
	trans = glm::translate(trans, translate);
	trans = glm::rotate(trans, (float)glfwGetTime(), rotate);
	trans = glm::scale(trans, scale);
	// pass matrix to shader
	unsigned int transformLoc = glGetUniformLocation(shader->ID, "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

void LV_Hello::InitData()
{

}

void LV_Hello::glmTest()
{
	glm::vec4 vec(1.0f, 0.0f, 0.0f, 1.0f);
	// if you are using 0.9.9 above
	// you might need to change below line to:
	// glm::mat4 trans = glm::mat4(1.0f)
	glm::mat4 trans;
	trans = glm::translate(trans, glm::vec3(1.0f, 1.0f, 0.0f));
	vec = trans * vec;
	std::cout << vec.x << vec.y << vec.z << std::endl;
}

void LV_Hello::LoadTexture()
{
	// diffuse texture 1
	// create a texture object 
	glGenTextures(1, &diffuseTex_1);
	// bind a texture
	glBindTexture(GL_TEXTURE_2D, diffuseTex_1);
	// set repeat and filtering for current binding texture
	// set uv repeat mode, S and T direction
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
	// set filtering for zoom in / zoom out
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	// mipmap for zoom out
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Load texture using stb_image 
	stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
	int width, height, nrChannels;
	unsigned char *data = stbi_load(FileSystem::getPath("/Assets/Texture/container.png").c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		// using the picture data to generate texture
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D); // bind a mipmap for texture
	}
	else
	{
		std::cout << "Failed to load texture" << "\n" << FileSystem::getPath("/Assets/Texture/container.png").c_str() << std::endl;
	}
	// release data 
	stbi_image_free(data);

	// diffuse texture 2
	glGenTextures(1, &diffuseTex_2);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_2);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	data = stbi_load(FileSystem::getPath("/Assets/Texture/kotoridesu_s.tga").c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		// note that the huaji.tga has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << "\n" << FileSystem::getPath("/Assets/Texture/kotoridesu_s.tga").c_str() << std::endl;
	}
	stbi_image_free(data);
    
    // specular texture
    glGenTextures(1, &specularTex);
    glBindTexture(GL_TEXTURE_2D, specularTex);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);    // set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    data = stbi_load(FileSystem::getPath("/Assets/Texture/container2_specular.png").c_str(), &width, &height, &nrChannels, 0);
    if (data)
    {
        // note that the huaji.tga has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << "\n" << FileSystem::getPath("/Assets/Texture/container2_specular.png").c_str() << std::endl;
    }
    stbi_image_free(data);




}

void LV_Hello::SetShaderLightParams(Shader *shader)
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

void LV_Hello::HelloLightInit()
{
	////helloLightShader->setVec3f("lightColor", light1->lightColor);
	//helloLightShader->setVec3f("dirLight.direction", dirLight->dir);
	//helloLightShader->setVec3f("dirLight.ambient", 0.02f * dirLight->color);
	//helloLightShader->setVec3f("dirLight.diffuse", 0.5f * dirLight->color);
	//helloLightShader->setVec3f("dirLight.specular", 1.0f * dirLight->color);

	//helloLightShader->setVec3f("pointLights[0].position", pointLight1->GetPos());
	//helloLightShader->setFloat("pointLights[0].constant", pointLight1->constant);
	//helloLightShader->setFloat("pointLights[0].linear", pointLight1->linear);
	//helloLightShader->setFloat("pointLights[0].quadratic", pointLight1->quadratic);
	//helloLightShader->setVec3f("pointLights[0].ambient", 0.01f * pointLight1->color);
	//helloLightShader->setVec3f("pointLights[0].diffuse", 0.5f * pointLight1->color);
	//helloLightShader->setVec3f("pointLights[0].specular", 0.8f * pointLight1->color);

	//helloLightShader->setVec3f("pointLights[1].position", pointLight2->GetPos());
	//helloLightShader->setFloat("pointLights[1].constant", pointLight2->constant);
	//helloLightShader->setFloat("pointLights[1].linear", pointLight2->linear);
	//helloLightShader->setFloat("pointLights[1].quadratic", pointLight2->quadratic);
	//helloLightShader->setVec3f("pointLights[1].ambient", 0.01f * pointLight2->color);
	//helloLightShader->setVec3f("pointLights[1].diffuse", 0.3f * pointLight2->color);
	//helloLightShader->setVec3f("pointLights[1].specular", 0.3f * pointLight2->color);

	//helloLightShader->setVec3f("spotLight.position", spotLight->GetPos());
	//helloLightShader->setVec3f("spotLight.direction", spotLight->dir);
	//helloLightShader->setFloat("spotLight.cutOff", glm::cos(spotLight->cutOff));  // cutoff is cosine of angle
	//helloLightShader->setFloat("spotLight.outerCutOff", glm::cos(spotLight->outerCutOff));
	//helloLightShader->setVec3f("spotLight.ambient", 0.01f * spotLight->color);
	//helloLightShader->setVec3f("spotLight.diffuse", 0.8f * spotLight->color);
	//helloLightShader->setVec3f("spotLight.specular", 0.5f * spotLight->color);
}


