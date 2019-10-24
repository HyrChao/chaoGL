#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform vec2 anchor;
uniform vec2 scale;

out vec2 texCoords;

void main()
{
    vec2 anchor_trans = 2 * clamp(anchor, 0.0, 1.0); 
    vec2 pos = aPos.xy * scale - vec2(1.0) + anchor_trans;
    vec4 cPos = vec4(pos, 0.0, 1.0);
	gl_Position = cPos;
    texCoords = aTexCoords;
}