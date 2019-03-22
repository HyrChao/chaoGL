#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 vColor;
in vec2 TexCoord;
in vec3 FragPos;

struct Material {
	sampler2D diffuse_1;
	sampler2D diffuse_2;
    sampler2D specular;
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

void main()
{
	// sample diffuse color 
	vec4 diffuseCol_1 = texture(material.diffuse_1, TexCoord);
	vec4 diffuseCol_2 = texture(material.diffuse_2, TexCoord);
	vec3 specularCol = texture(material.specular, TexCoord).rgb;
	vec3 baseColor = mix(diffuseCol_1, diffuseCol_2, diffuseCol_2.a).rgb;
	
	// calc lights
	// light direction
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPos);

	// ambient color
    vec3 ambient =  light.ambient * baseColor;
	
	// diffuse color
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = light.diffuse * (diff * baseColor);
	
	// specular color
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * (specularCol * spec);

	vec3 result = (diffuse + ambient + specular);
	
	FragColor = vec4(result,1);
}
