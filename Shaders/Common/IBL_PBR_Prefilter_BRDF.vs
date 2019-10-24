#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

out vec2 texCoords;

void main()
{
    texCoords = aTexCoords;
    vec4 clipPos =  vec4(aPos, 1.0);
    clipPos.z = 0.0;
    gl_Position = clipPos;
}