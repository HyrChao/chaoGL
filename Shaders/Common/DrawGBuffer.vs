#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;


out vec2 texCoords;

void main()
{
    vec4 pos = vec4(aPos.x, aPos.y, 0.0, 1.0);
	gl_Position = pos;
    texCoords = aTexCoords;
}