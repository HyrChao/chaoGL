#version 330 core
out vec4 FragColor;
in vec3 vColor;
uniform vec4 ourColor;
void main()
{
	FragColor = vec4(ourColor.rgb - vColor, 1.0);
}