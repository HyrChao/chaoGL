#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aColor;

out vec3 vColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    vColor = aColor;
    TexCoord = aTexCoord;
	//replace this : Normal = mat3(transpose(inverse(model))) * aNormal;
	//can update with scale, but two cost, replace with simple one
	// see https://learnopengl-cn.github.io/02%20Lighting/02%20Basic%20Lighting/ for detail
	//model = mat4(1.0f);
	//at4 testMod = mat4(100.0);
	Normal = vec3(model * vec4(aNormal, 0.0));
	FragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}