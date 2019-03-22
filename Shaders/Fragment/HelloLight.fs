#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 vColor;
in vec2 TexCoord;
in vec3 FragPos;

struct Material {
	sampler2D diffuse;
    vec3 specular;
    float shininess;
}; 

uniform Material material;

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform Light light;

uniform vec3 viewPos;

uniform sampler2D texture2;

void main()
{
    //FragColor = vec4(vColor,0);
    //FragColor = texture(texture1, TexCoord);
	//FragColor = texture(texture2, TexCoord) * vec4(vColor, 1.0);
	vec4 diffuseTex = texture(material.diffuse, TexCoord);
	vec4 tex2color = texture(texture2, TexCoord);
	
	// calc lights
	// light direction
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);

	// ambient color
    vec3 ambient =  light.ambient * diffuseTex.rgb;
	
	// diffuse color
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * diffuseTex.rgb);
	
	// specular color
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (material.specular * spec);

	//vec3 baseColor = mix(tex1color, tex2color, tex2color.a).rgb;
	vec3 result = (diffuse + ambient + specular) * tex2color.rgb;
	
	FragColor = vec4(result,1);
}
