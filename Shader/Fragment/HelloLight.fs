#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 vColor;
in vec2 TexCoord;
in vec3 FragPos;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    //FragColor = vec4(vColor,0);
    //FragColor = texture(texture1, TexCoord);
	//FragColor = texture(texture2, TexCoord) * vec4(vColor, 1.0);
	vec4 tex1color = texture(texture1, TexCoord);
	vec4 tex2color = texture(texture2, TexCoord);
	// calc lights
	// light direction
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	// ambient color
	float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;
	// diffuse color
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * lightColor;

	FragColor = vec4(diffuse + ambient,1) * mix(tex1color, tex2color, tex2color.a);
}