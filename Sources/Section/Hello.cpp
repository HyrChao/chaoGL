﻿#include<Section/Hello.h>

Hello::Hello()
{
	if (!isDataInitialized)
	{
		Geo::geo = new Geo();
		//----------------------------------
		//Shader
		//Shader shader("./Shader/Vertex/HelloTriangle.vs", "./Shader/Fragment/HelloTriangle.fs");
		helloTriShader = new Shader("./Shaders/Vertex/HelloTriangle.vs", "./Shaders/Fragment/HelloTriangle.fs");
		helloTexShader = new Shader("./Shaders/Vertex/HelloTexture.vs", "./Shaders/Fragment/HelloTexture.fs");
		helloProjShader = new Shader("./Shaders/Vertex/HelloProjection.vs", "./Shaders/Fragment/HelloProjection.fs");
		helloLightShader = new Shader("./Shaders/Vertex/HelloLight.vs", "./Shaders/Fragment/HelloLight.fs");

		// load tecture
		LoadTexture(helloTexShader);
		LoadTexture(helloProjShader);
		LoadTexture(helloLightShader);

	}

	SetupLight();

	isDataInitialized = true;
}

Hello::~Hello()
{
	delete helloTriShader;
	delete helloTexShader;
	delete helloProjShader;
	delete helloLightShader;
}

void Hello::SetupLight()
{
	light1 = new Light(glm::vec3(0.0f,3.0f,-3.0f));
	light1->lightColor = glm::vec3(0.6f, 0.2f, 0.2f);
	//light1->lightColor = glm::vec3(0.2f, 0.05f, 0.05f);

}

void Hello::HelloTriangle()
{
	// Use shader program
	// Update color with time
	float timeValue = glfwGetTime();
	float redVal = (cos(timeValue) / 2.0f) + 0.5f;
	float greenVal = (cos(timeValue + 3.14 / 3) / 2.0f) + 0.5f;
	float blueVal = (cos(timeValue - 3.14 / 3) / 2.0f) + 0.5f;

	helloTriShader->use();
	helloTriShader->setVec4f("ourColor", redVal, greenVal, blueVal, 1.0f);
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
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);

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
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
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
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
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
	
	// change light color among with time 
	float timeval1 = (glm::sin(Time::time) + 1)/2;
	float timeval2 = (glm::cos(Time::time) + 1)/2;
	float timeval3 = (glm::sin(Time::time + 3.14f) + 1)/2;
	light1->lightColor = glm::vec3(timeval1, timeval2, timeval3);

	light1->DrawAvatar();

	helloLightShader->use();
	// ortho matrix
	//glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
	glm::mat4 model;
	model = glm::rotate(model, glm::radians(55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

	glm::mat4 view;
	// move model forward equals to move view backward
	view = Camera::main->viewMat;

	glm::mat4 projection;
	projection = Camera::main->projMat;

	helloLightShader->setMat4f("model", model);
	helloLightShader->setMat4f("view", view);
	helloLightShader->setMat4f("projection", projection);
	helloLightShader->setVec3f("viewPos", Camera::main->pos);

	helloLightShader->setVec3f("material.ambient", 1.0f, 0.5f, 0.31f);
	helloLightShader->setVec3f("material.diffuse", 1.0f, 0.5f, 0.31f);
	helloLightShader->setVec3f("material.specular", 0.5f, 0.5f, 0.5f);
	helloLightShader->setFloat("material.shininess", 32.0f);

	//helloLightShader->setVec3f("lightColor", light1->lightColor);
	helloLightShader->setVec3f("light.position", light1->pos);
	helloLightShader->setVec3f("light.ambient", 0.2f * light1->lightColor);
	helloLightShader->setVec3f("light.diffuse", 0.5f * light1->lightColor);
	helloLightShader->setVec3f("light.specular", 1.0f * light1->lightColor);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2);
	glBindVertexArray(Geo::geo->cubeVAO);
	for (unsigned int i = 0; i < 10; i++)
	{
		glm::mat4 model;
		model = glm::translate(model, cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model, glm::radians(angle) + (float)glfwGetTime(), glm::vec3(glm::sin((float)glfwGetTime()), glm::cos((float)glfwGetTime()), -glm::cos((float)glfwGetTime())));
		helloLightShader->setMat4f("model", model);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindVertexArray(0);
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

void Hello::LoadTexture(Shader* shader)
{
	// texture1
	// create a texture object 
	glGenTextures(1, &texture1);
	// bind a texture
	glBindTexture(GL_TEXTURE_2D, texture1);
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

	// texture2
	glGenTextures(1, &texture2);
	glBindTexture(GL_TEXTURE_2D, texture2);
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


	// texture unit setup
	shader-> use();
	glUniform1i(glGetUniformLocation(shader->ID, "texture1"), 0); // setup manully
	//glUniform1i(glGetUniformLocation(helloTexShader->ID, "texture2"), 1); // setup manully
	shader->setInt("texture2", 1); // or use function in shader class
}
