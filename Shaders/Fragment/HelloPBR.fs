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
	sampler2D albedo;
	sampler2D normal;
    sampler2D roughness;
    sampler2D metallic;
    sampler2D ao;
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
    vec3 lightDir = normalize(-light.direction.xyx);
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
    float attenuation = 1.0 / (light.constant + light.linear * lightDist + light.quadratic * (lightDist * lightDist));    
    // merge
	//attenuation = 1.0;
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

vec3 ConvertNormalToWorldspace(vec3 tangentNormal)
{
    vec3 Q1  = dFdx(WorldPos);
    vec3 Q2  = dFdy(WorldPos);
    vec2 st1 = dFdx(TexCoords);
    vec2 st2 = dFdy(TexCoords);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
    FragOut FragColor;

    // Textures sampling
    vec3 albedo = pow(texture(materila.albedo,TexCoord).rgb,2.2); // convert from SRGB to linear space
    vec3 normal = texture(material.normal, TexCoords).xyz * 2.0 - 1.0;
    normal = ConvertNormalToWorldspace(normal); //PBR calc need wordspace normal
    float metallic = texture(material.metallic, TexCoord).r;
    float roughness = texture(material.roughness, TexCoord).r;
    float ao = pow(texture(material.ao, TexCoord).r,2.2);

    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - fragPos);

	FragColor = vec4(result,1);
}
