#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform vec2 anchor;

out vec2 texCoords;

void main()
{
    vec3 scaledPos = aPos * 0.4;
    vec4 cPos = vec4(scaledPos,1.0);
    cPos.z = 0.0;
    // cPos.xy = cPos.xy;
    cPos.xy = cPos.xy + anchor;
	gl_Position = cPos;
    texCoords = aTexCoords;
}