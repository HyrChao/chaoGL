#include<Assets/CommonAssets.h>

CommonAssets* CommonAssets::instance;

CommonAssets::CommonAssets()
{
	instance = this;

	LoadTriangle();
	LoadRecangle();
	LoadCube();
	LoadSphere();
	LoadCommonTextures();
	LoadShaders();
}

void CommonAssets::LoadShaders()
{
	standardPBRShader = new Shader("/Shaders/Prop/StandardPBR.vs", "/Shaders/Prop/StandardPBR.fs", true);
	defaltErrorShader = new Shader("/Shaders/Debug/Debug_ErrorShader.vs", "/Shaders/Debug/Debug_ErrorShader.fs");
}

void CommonAssets::LoadCommonTextures()
{
	// Load debug textures
	blackTex = AssetsManager::TextureFromFile("/Assets/Texture/black.png");
	whiteTex = AssetsManager::TextureFromFile("/Assets/Texture/white.png");
	greyTex = AssetsManager::TextureFromFile("/Assets/Texture/grey.png");
	flatNormal = AssetsManager::TextureFromFile("/Assets/Texture/flat.png");

}

void CommonAssets::LoadTriangle()
{
	// bind triangle
	//Vertex array object
	glGenVertexArrays(1, &triVAO);
	glBindVertexArray(triVAO);	//After bind, VBO information can transfer to VAO, unbind to define other vertex
								//Vertex buffer object
	glGenBuffers(1, &triVBO);
	glBindBuffer(GL_ARRAY_BUFFER, triVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triVertices), triVertices, GL_STATIC_DRAW);
	// vertex pos property
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// vertex color property
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


}

void CommonAssets::LoadRecangle()
{
	// bind recangle
	//Vertex array object
	glGenVertexArrays(1, &recVAO);
	glBindVertexArray(recVAO);	//After bind, VBO information can transfer to VAO, unbind to define other vertex
								// Vertex buffer object
	glGenBuffers(1, &recVBO);
	glBindBuffer(GL_ARRAY_BUFFER, recVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(recVertices), recVertices, GL_STATIC_DRAW);
	//Element Buffer Object
	glGenBuffers(1, &recEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, recEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(recIndices), recIndices, GL_STATIC_DRAW);
	// vertex pos property
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// vetex ovcoord property
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

}

void CommonAssets::LoadCube()
{
	// bind box
//Vertex array object
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);	//After bind, VBO information can transfer to VAO, unbind to define other vertex
								// Vertex buffer object
	glGenBuffers(1, &cubeVBO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
	// vertex pos property
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//glEnableVertexAttribArray(1);
	// vetex ovcoord property
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

}

void CommonAssets::LoadSphere() 
{
	// Sphere
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glGenBuffers(1, &sphereEBO);

	std::vector<glm::vec3> positions;
	std::vector<glm::vec2> uv;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec3> colors;
	std::vector<unsigned int> indices;

	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359;
	for (unsigned int y = 0; y <= Y_SEGMENTS; ++y)
	{
		for (unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			float xSegment = (float)x / (float)X_SEGMENTS;
			float ySegment = (float)y / (float)Y_SEGMENTS;
			float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
			float yPos = std::cos(ySegment * PI);
			float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

			positions.push_back(glm::vec3(xPos, yPos, zPos));
			uv.push_back(glm::vec2(xSegment, ySegment));
			normals.push_back(glm::vec3(xPos, yPos, zPos));
			colors.push_back(glm::vec3(x / X_SEGMENTS, y / Y_SEGMENTS, 0));
		}
	}

	bool oddRow = false;
	for (int y = 0; y < Y_SEGMENTS; ++y)
	{
		if (!oddRow) // even rows: y == 0, y == 2; and so on
		{
			for (int x = 0; x <= X_SEGMENTS; ++x)
			{
				indices.push_back(y       * (X_SEGMENTS + 1) + x);
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for (int x = X_SEGMENTS; x >= 0; --x)
			{
				indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				indices.push_back(y       * (X_SEGMENTS + 1) + x);
			}
		}
		oddRow = !oddRow;
	}
	sphereIndexCount = indices.size();

	std::vector<float> data;
	for (int i = 0; i < positions.size(); ++i)
	{
		data.push_back(positions[i].x);
		data.push_back(positions[i].y);
		data.push_back(positions[i].z);
		if (normals.size() > 0)
		{
			data.push_back(normals[i].x);
			data.push_back(normals[i].y);
			data.push_back(normals[i].z);
		}
		if (uv.size() > 0)
		{
			data.push_back(uv[i].x);
			data.push_back(uv[i].y);
		}
		if (colors.size() > 0)
		{
			data.push_back(colors[i].x);
			data.push_back(colors[i].y);
			data.push_back(colors[i].z);
		}

		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		float stride = (3 + 3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (void*)(8 * sizeof(float)));
	}


	// Sphere end
}






