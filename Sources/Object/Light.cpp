#include<Object/Light.h>

Light::Light(glm::vec3 pos)
{
    

	this->pos = pos;

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// 只需要绑定VBO不用再次设置VBO的数据，因为箱子的VBO数据中已经包含了正确的立方体顶点数据
	glBindBuffer(GL_ARRAY_BUFFER, Geo::geo->cubeVBO);
	// 设置灯立方体的顶点属性（对我们的灯来说仅仅只有位置数据）
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
}

Light::~Light()
{
    
}

void Light::DrawAvatar() const
{
	avatarShader->use();

	avatarShader->setMat4f("model", trans);
	avatarShader->setMat4f("view", Camera::main->viewMat);
	avatarShader->setMat4f("projection", Camera::main->projMat);

	avatarShader->setVec3f("lightColor", color);

	glBindVertexArray(Geo::geo->cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}








