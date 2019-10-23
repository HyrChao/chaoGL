#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;

uniform mat4 projection;
uniform mat4 view;


out vec3 localPos;
out vec2 texCoords;

void main()
{
    localPos = aPos;
    texCoords = aTexCoords;
    vec4 clipPos = projection * view * vec4(localPos, 1.0);

    gl_Position = clipPos;
}