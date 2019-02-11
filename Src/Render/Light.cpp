#include<Render/Light.h>

Light::Light(glm::vec3 pos)
{
	this->pos = pos;

	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	// ֻ��Ҫ��VBO�����ٴ�����VBO�����ݣ���Ϊ���ӵ�VBO�������Ѿ���������ȷ�������嶥������
	glBindBuffer(GL_ARRAY_BUFFER, Geo::geo->cubeVBO);
	// ���õ�������Ķ������ԣ������ǵĵ���˵����ֻ��λ�����ݣ�
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// transform 
	scale = glm::vec3(0.2f);

	trans = glm::translate(trans, pos);
	trans = glm::scale(trans, scale);

	//lightColor = glm::vec3(0.3f, 0.1f, 0.1f);
	//lightColor = glm::vec3(0.1f, 0.0f, 0.0f);

	avatarShader = new Shader("./Shader/Avatar/Light.vs", "./Shader/Avatar/Light.fs");
}

void Light::DrawAvatar()
{

	avatarShader->use();

	avatarShader->setMat4f("model", trans);
	avatarShader->setMat4f("view", Camera::main->viewMat);
	avatarShader->setMat4f("projection", Camera::main->projMat);

	avatarShader->setVec3f("lightColor", lightColor);

	glBindVertexArray(Geo::geo->cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}







