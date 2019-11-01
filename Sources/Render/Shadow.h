#ifndef SHADOW_H
#define SHADOW_H


#include <glad/glad.h>
#include <Render/Shader.h>
class Shadow
{
public:

	Shadow();
	~Shadow();

	static void RenderShadowMap();


private:

	static void CreateTransformMatrix();
	static Shader* shadowMapingShader;

};

#endif // !SHADOW_H

