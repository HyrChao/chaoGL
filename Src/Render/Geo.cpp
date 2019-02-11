#include<Render/Geo.h>

Geo* Geo::geo;

Geo::Geo()
{
	geo = this;

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
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// vertex color property
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// vetex ovcoord property
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

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







