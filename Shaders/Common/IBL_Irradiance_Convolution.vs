#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;

uniform mat4 projection;
uniform mat4 view;


out vec3 localPos;
out vec3 Normal;

void main()
{
    localPos = aPos;
    Normal = normalize(aNormal);

    vec4 clipPos = projection * view * vec4(localPos, 1.0);

    gl_Position = clipPos;
}