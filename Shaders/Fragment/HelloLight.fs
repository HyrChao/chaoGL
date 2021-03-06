#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 vColor;
in vec2 TexCoord;
in vec3 FragPos;

struct FragOut
{
	vec3 diffuse;
	vec3 normal;
	vec3 specular;
	vec3 ambient;
};

struct Material 
{
	sampler2D diffuse_1;
	sampler2D diffuse_2;
    sampler2D specular;
    float shininess;
}; 

uniform Material material;

struct DirLight 
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform DirLight dirLight;

struct PointLight 
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight 
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;	

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};  
uniform SpotLight spotLight;

uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, FragOut frag, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction.xyz);
    // diff
    float diff = max(dot(frag.normal, lightDir), 0.0);
    // spec
    vec3 reflectDir = reflect(-lightDir, frag.normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // merge
    vec3 ambient  = light.ambient  * frag.diffuse;
    vec3 diffuse  = light.diffuse  * diff * frag.diffuse;
    vec3 specular = light.specular * spec * frag.specular;
    return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, FragOut frag, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse
    float diff = max(dot(frag.normal, lightDir), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, frag.normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // attenuation
    float lightDist    = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear * lightDist + light.quadratic * (lightDist * lightDist));    
    // merge
	// attenuation = 1.0f / (light.constant + light.linear + light.quadratic);
    // attenuation = clamp(attenuation,0,1000);
    attenuation = clamp(attenuation,0,1);
    // attenuation = abs(attenuation);
    vec3 ambient  = light.ambient  * frag.diffuse;
    vec3 diffuse  = light.diffuse  * diff * frag.diffuse;
    vec3 specular = light.specular * spec * frag.specular;
    ambient  *= attenuation;
    diffuse  *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, FragOut frag, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);

    // diffuse
    float diff = max(dot(frag.normal, lightDir), 0.0);
    // specular
    vec3 reflectDir = reflect(-lightDir, frag.normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    // merge
    vec3 ambient  = light.ambient  * frag.diffuse;
    vec3 diffuse  = light.diffuse  * diff * frag.diffuse;
    vec3 specular = light.specular * spec * frag.specular;

	float theta     = dot(lightDir, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	diffuse  *= intensity;
	specular *= intensity;
	return (ambient + diffuse + specular);
}

void main()
{
	// sample diffuse color 
	FragOut frag;
	vec4 diffuseCol_1 = texture(material.diffuse_1, TexCoord);
	vec4 diffuseCol_2 = texture(material.diffuse_2, TexCoord);

	frag.diffuse = mix(diffuseCol_1, diffuseCol_2, diffuseCol_2.a).rgb;
	frag.normal = normalize(Normal);
	frag.specular = texture(material.specular, TexCoord).rgb;
	frag.ambient = vec3(0); // evironment ambient

	// calc lights
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 result = vec3(0);
    // dirlight
    result = CalcDirLight(dirLight, frag, viewDir);
    // pointlight
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], frag, FragPos, viewDir);    
	}
    // spotlight
    result += CalcSpotLight(spotLight, frag, FragPos, viewDir);    
    // result = vec3(pointLights[0].constant + pointLights[0].linear + pointLights[0].quadratic);    
    // float lightDist    = length(pointLights[0].position - FragPos);
    // result = vec3(1.0/(pointLights[0].constant + pointLights[0].linear + pointLights[0].quadratic));    
    // result = vec3(pointLights[0].linear + pointLights[0].quadratic);    
    // result = vec3(1.0/lightDist);    
	FragColor = vec4(result,1);
}
