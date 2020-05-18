#version 330 core
out vec4 FragColor;

in vec2 texCoords;
uniform sampler2D bufferTex;

void main()
{
	vec4 finalColor = texture(bufferTex, texCoords);
	// finalColor = vec4(1.0);
	finalColor.a = 1.0;
	FragColor = finalColor;
}