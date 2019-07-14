#include<Section/Hello.h>

Hello::Hello()
{
	if (!isDataInitialized)
	{
		Geo::geo = new Geo();
		//----------------------------------
		//Shader
		//Shader shader("./Shader/Vertex/HelloTriangle.vs", "./Shader/Fragment/HelloTriangle.fs");
		helloTriShader = new Shader("/Shaders/Vertex/HelloTriangle.vs", "/Shaders/Fragment/HelloTriangle.fs");
		helloTexShader = new Shader("/Shaders/Vertex/HelloTexture.vs", "/Shaders/Fragment/HelloTexture.fs");
		helloProjShader = new Shader("/Shaders/Vertex/HelloProjection.vs", "/Shaders/Fragment/HelloProjection.fs");


		// load tecture
		LoadTexture();
	}

	Light::ClearAllLight();

	isDataInitialized = true;
}

Hello::~Hello()
{

}

void Hello::SetupLight_Old()
{
//    pointLight1 = new Light(glm::vec3(0.0f,3.0f,0.0f),LightType::Point);
//	pointLight1->color = glm::vec3(0.6f, 0.2f, 0.2f);
//    pointLight1->constant = 1.0f;
//    pointLight1->linear = 0.09f;
//    pointLight1->quadratic = 0.032f;
//
//    pointLight2 = new Light(glm::vec3(-5.0f,-3.0f,-5.0f),LightType::Point);
//    pointLight2->color = glm::vec3(0.1f, 0.8f, 0.1f);
//    pointLight2->constant = 1.0f;
//    pointLight2->linear = 0.7f;
//    pointLight2->quadratic = 1.8f;
//    
//    dirLight = new Light(glm::vec3(0),LightType::Directional);
//    dirLight->color = glm::vec3(1.0f, 1.0f, 1.0f);
//    dirLight->dir = glm::vec3(-1,-1,-1);
//    
//    spotLight = new Light(glm::vec3(-3.0f,-1.0f,-8.0f),LightType::Spot);
////    spotLight->color = glm::vec3(0.7f, 0.5f, 0.2f);
//    spotLight->color = glm::vec3(1.0f, 1.0f, 0.1f);
//    spotLight->dir = glm::vec3(3.0f, 1.0f, 8.0f);
//    spotLight->SetCutOff(12.5f);
//    spotLight->SetOuterCutOff(17.5f);
////    spotLight->cutOff = 0.0347f;
////    spotLight->outerCutOff = 0.0486f;
    
}

void Hello::HelloTriangle()
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
	glBindVertexArray(Geo::geo->triVAO);
	glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(0);
}

void Hello::HelloTransform()
{
	helloTexShader->use();
	//glBindTexture(GL_TEXTURE_2D, texture);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_2);

	glBindVertexArray(Geo::geo->recVAO);
	Transform(helloTexShader,glm::vec3(0.5f, -0.5f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.5, 0.5, 0.5));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	Transform(helloTexShader,glm::vec3(0.5f, 0.5f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.5, 0.5, 0.5));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	Transform(helloTexShader,glm::vec3(-0.5f, 0.5f, 0.0f), glm::vec3(1.0f, 1.0f, 0.0f), glm::vec3(0.5, 0.5, 0.5));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
	Transform(helloTexShader,glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.5, 0.5, 0.5));
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);

}

void Hello::HelloProjection()
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
	Transform(helloProjShader,glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(1.5, 1.5, 1.5));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, diffuseTex_2);
	glBindVertexArray(Geo::geo->cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

}

void Hello::HelloBox()
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
	glBindVertexArray(Geo::geo->cubeVAO);
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

void Hello::HelloCamera()
{
	Camera::main->CameraAutoSpan();

	HelloBox();
}

void Hello::HelloLight()
{

	Light* dirLight;

    if (!helloLightInitialized)

		helloLightShader = new Shader("/Shaders/Vertex/HelloLight.vs", "/Shaders/Fragment/HelloLight.fs");

		helloLightShader->use(); // set uniform！for texture
		helloLightShader->setInt("material.diffuse_1", 0);
		helloLightShader->setInt("material.diffuse_2", 1);
		helloLightShader->setInt("material.specular", 2);
    {
		Light::lights.clear();

		LightParam pointLightp1;
		pointLightp1.type = LightType::Point;
		pointLightp1.pos = glm::vec3(0.0f, 3.0f, 0.0f);
		pointLightp1.color = glm::vec3(0.6f, 0.2f, 0.2f);
		pointLightp1.constant = 1.0f;
		pointLightp1.linear = 0.09f;
		pointLightp1.quadratic = 0.032f;
		Light* pointLight1 = new Light(pointLightp1);

		LightParam pointLightp2;
		pointLightp2.type = LightType::Point;
		pointLightp2.pos = glm::vec3(-5.0f, -3.0f, -5.0f);
		pointLightp2.color = glm::vec3(0.1f, 0.8f, 0.1f);
		pointLightp2.constant = 1.0f;
		pointLightp2.linear = 0.7f;
		pointLightp2.quadratic = 1.8f;
		Light* pointLight2 = new Light(pointLightp2);

		LightParam dirlightp1;
		dirlightp1.type = LightType::Directional;
		dirlightp1.pos = glm::vec3(10.f, 10.f, 10.f);
		dirlightp1.color = glm::vec3(1.0f, 1.0f, 1.0f);
		dirlightp1.dir = glm::vec3(-1, -1, -1);;
		dirLight = new Light(dirlightp1);

		LightParam spotlightp1;
		spotlightp1.type = LightType::Spot;
		spotlightp1.pos = glm::vec3(-3.0f, -1.0f, -8.0f);
		spotlightp1.color = glm::vec3(1.0f, 1.0f, 0.1f);
		spotlightp1.dir = glm::vec3(3.0f, 1.0f, 8.0f);
		spotlightp1.cutOff = 12.5f;
		spotlightp1.outerCutoff = 17.5f;
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

	Render::SetShaderLightParams(helloLightShader);
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
	glBindVertexArray(Geo::geo->cubeVAO);
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

void Hello::HelloModel()
{
    glm::vec4 clearColor = glm::vec4(0.07f);
    clearColor.a = 1.0f;
    Render::SetClearColor(clearColor);
    if(!helloModelInitialized){
		Light::lights.clear();

		LightParam pointLightp1;
		pointLightp1.type = LightType::Point;
		pointLightp1.pos = glm::vec3(0.0f, 3.0f, 0.0f);
		pointLightp1.color = glm::vec3(0.6f, 0.2f, 0.2f);
		pointLightp1.constant = 1.0f;
		pointLightp1.linear = 0.09f;
		pointLightp1.quadratic = 0.032f;
		Light* pointLight1 = new Light(pointLightp1);

		LightParam pointLightp2;
		pointLightp2.type = LightType::Point;
		pointLightp2.pos = glm::vec3(-5.0f, -3.0f, -5.0f);
		pointLightp2.color = glm::vec3(0.1f, 0.8f, 0.1f);
		pointLightp2.constant = 1.0f;
		pointLightp2.linear = 0.7f;
		pointLightp2.quadratic = 1.8f;
		Light* pointLight2 = new Light(pointLightp2);

		LightParam dirlightp1;
		dirlightp1.type = LightType::Directional;
		dirlightp1.pos = glm::vec3(10.f, 10.f, 10.f);
		dirlightp1.color = glm::vec3(1.0f, 1.0f, 1.0f);
		dirlightp1.dir = glm::vec3(-1, -1, -1);;
		Light* dirLight = new Light(dirlightp1);

		LightParam spotlightp1;
		spotlightp1.type = LightType::Spot;
		spotlightp1.pos = glm::vec3(-3.0f, -1.0f, -8.0f);
		spotlightp1.color = glm::vec3(1.0f, 1.0f, 0.1f);
		spotlightp1.dir = glm::vec3(3.0f, 1.0f, 8.0f);
		spotlightp1.cutOff = 12.5f;
		spotlightp1.outerCutoff = 17.5f;
		Light* spotlight = new Light(spotlightp1);

        helloModel = new Model(FileSystem::getPath("/Assets/Model/nanosuit/nanosuit.obj"), false,glm::vec3(0.0), glm::vec3(0.0), glm::vec3(0.1));
        helloModelShader = new Shader("/Shaders/Vertex/HelloModel.vs", "/Shaders/Fragment/HelloModel.fs");
        helloModelInitialized = true;
    }
	//float timeVal = glm::sin(Time::time) * 0.02f;
	float timeVal = Time::deltaTime;
	helloModel->Rotate(glm::vec3(0, timeVal, 0));
    helloModel->Draw(helloModelShader);
    
}

void Hello::HelloPBR()
{
	if (!helloPBRInitialized)
	{
		Light::lights.clear();

		LightParam pointLightp1;
		pointLightp1.type = LightType::Point;
		pointLightp1.pos = glm::vec3(0.0f, 3.0f, 0.0f);
		pointLightp1.color = glm::vec3(0.6f, 0.2f, 0.2f);
		pointLightp1.constant = 1.0f;
		pointLightp1.linear = 0.09f;
		pointLightp1.quadratic = 0.032f;
		Light* pointLight1 = new Light(pointLightp1);

		LightParam pointLightp2;
		pointLightp2.type = LightType::Point;
		pointLightp2.pos = glm::vec3(-5.0f, -3.0f, -5.0f);
		pointLightp2.color = glm::vec3(0.1f, 0.8f, 0.1f);
		pointLightp2.constant = 1.0f;
		pointLightp2.linear = 0.7f;
		pointLightp2.quadratic = 1.8f;
		Light* pointLight2 = new Light(pointLightp2);

		LightParam dirlightp1;
		dirlightp1.type = LightType::Directional;
		dirlightp1.pos = glm::vec3(10.f, 10.f, 10.f);
		dirlightp1.color = glm::vec3(1.0f, 1.0f, 1.0f);
		dirlightp1.dir = glm::vec3(-1, -1, -1);;
		Light* dirLight = new Light(dirlightp1);

		LightParam spotlightp1;
		spotlightp1.type = LightType::Spot;
		spotlightp1.pos = glm::vec3(-3.0f, -1.0f, -8.0f);
		spotlightp1.color = glm::vec3(1.0f, 1.0f, 0.1f);
		spotlightp1.dir = glm::vec3(3.0f, 1.0f, 8.0f);
		spotlightp1.cutOff = 12.5f;
		spotlightp1.outerCutoff = 17.5f;
		Light* spotlight = new Light(spotlightp1);

		albedo = AssertsMng::TextureFromFile(FileSystem::getPath("/Assets/Texture/HelloPBR/rustediron2_basecolor.png").c_str());
		normal = AssertsMng::TextureFromFile(FileSystem::getPath("/Assets/Texture/HelloPBR/rustediron2_normal.png").c_str());
		metallic = AssertsMng::TextureFromFile(FileSystem::getPath("/Assets/Texture/HelloPBR/rustediron2_metallic.png").c_str());
		roughness = AssertsMng::TextureFromFile(FileSystem::getPath("/Assets/Texture/HelloPBR/rustediron2_roughness.png").c_str());
		ao = AssertsMng::TextureFromFile(FileSystem::getPath("/Assets/Texture/HelloPBR/ao.png").c_str());

		helloPBRShader = new Shader("/Shaders/Vertex/HelloPBR.vs", "/Shaders/Fragment/HelloPBR.fs");
		helloPBRInitialized = true;
	}

	int nrRows = 7;
	int nrColumns = 7;
	float spacing = 2.5;

	glm::mat4 model = glm::mat4(1.0f);

	helloPBRShader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, albedo);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, metallic);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, roughness);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, ao);

	helloPBRShader->setInt("material.albedo", 0);
	helloPBRShader->setInt("material.normal", 1);
	helloPBRShader->setInt("material.metallic", 2);
	helloPBRShader->setInt("material.roughness", 3);
	helloPBRShader->setInt("material.ao", 4);

	for (int row = 0; row < nrRows; ++row)
	{
		for (int col = 0; col < nrColumns; ++col)
		{
			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(
				(float)(col - (nrColumns / 2)) * spacing,
				(float)(row - (nrRows / 2)) * spacing,
				0.0f
			));
			DrawSphere(helloPBRShader, model);
		}
	}
}

void Hello::Transform(Shader* shader,glm::vec3 translate, glm::vec3 rotate, glm::vec3 scale)
{
	glm::mat4 trans;
	trans = glm::translate(trans, translate);
	trans = glm::rotate(trans, (float)glfwGetTime(), rotate);
	trans = glm::scale(trans, scale);
	// pass matrix to shader
	unsigned int transformLoc = glGetUniformLocation(shader->ID, "model");
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(trans));
}

void Hello::InitData()
{

}

void Hello::glmTest()
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

void Hello::LoadTexture()
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

void Hello::HelloLightInit()
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


