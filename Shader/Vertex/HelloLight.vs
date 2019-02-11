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
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    vColor = aColor;
    TexCoord = aTexCoord;
	//replace this : Normal = mat3(transpose(inverse(model))) * aNormal;
	//can update with scale, but two cost, replace with simple one
	Normal = vec3(model * vec4(aNormal, 1.0));
	FragPos = vec3(model * vec4(aPos, 1.0));
}