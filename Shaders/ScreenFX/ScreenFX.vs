#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 fragCoord;
uniform vec2 iResolution;

void main()
{
    vec4 cPos = vec4(aPos.xy, 0.0, 1.0);
	gl_Position = cPos;
    // Compatible with shadertoy
    fragCoord = aTexCoords * iResolution;
}