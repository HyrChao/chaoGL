#version 330 core
out vec4 FragColor;

in vec3 vColor;
in vec2 TexCoord;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    //FragColor = vec4(vColor,0);
    //FragColor = texture(texture1, TexCoord);
	//FragColor = texture(texture2, TexCoord) * vec4(vColor, 1.0);
	vec4 tex1color = texture(texture1, TexCoord);
	vec4 tex2color = texture(texture2, TexCoord);
	FragColor = mix(tex1color, tex2color, tex2color.a);
}