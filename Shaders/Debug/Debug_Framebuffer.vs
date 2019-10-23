#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform vec2 anchor;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoords;

void main()
{
    vec4 clipPos = 0.3 * projection * view * vec4(aPos,0);
    clipPos.zw = vec2(1.0);
    clipPos.xy += anchor;
    texCoords = aTexCoords;
	gl_Position = clipPos;
}