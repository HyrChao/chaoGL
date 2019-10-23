#version 330 core
out vec4 FragColor;

in vec2 texCoords;
uniform sampler2D bufferTex;

void main()
{
	vec4 finalColor = texture(bufferTex, texCoords);
	FragColor = vec4(finalColor.rgb, 1.0);
}