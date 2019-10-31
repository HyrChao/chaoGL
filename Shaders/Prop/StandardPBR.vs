#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBitangent;
layout (location = 5) in vec4 aColor;

out vec4 vColor;
out vec2 TexCoord;
out vec3 Normal;
out vec3 Tangent;
out vec3 Bitangent;
out vec3 fragPos;

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
	Normal = normalize(vec3(model * vec4(aNormal, 0.0)));
	Tangent = normalize(vec3(model * vec4(aTangent, 0.0)));
	Bitangent = normalize(vec3(model * vec4(aBitangent, 0.0)));

	fragPos = vec3(model * vec4(aPos, 1.0));
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}