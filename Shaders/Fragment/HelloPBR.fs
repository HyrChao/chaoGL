#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 vColor;
in vec2 TexCoord;
in vec3 fragPos;

struct Material 
{
	sampler2D albedo;
	sampler2D normal;
    sampler2D roughness;
    sampler2D metallic;
    sampler2D ao;
}; 

uniform Material material;

struct Intensity
{
    vec3 tint;
    vec3 mro;
};

uniform Intensity intensity;

struct DirLight 
{
    vec3 direction;
    vec3 irradiance;
};  
uniform DirLight dirLight;

struct PointLight 
{
    vec3 position;
    vec3 irradiance;
};  
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight 
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;	
    vec3 irradiance;
};  
uniform SpotLight spotLight;

uniform samplerCube irradianceMap;

uniform vec3 viewPos;

// r, g, b, a  albedo, normal, matellic, roughness
uniform vec4 debug_pbr;
// r, g, b, a  irradiance
uniform vec4 debug_light;

const float PI = 3.14159265359;

//Schlick Fresnel
vec3 FresnelSchlick(float cosTheta, vec3 F0)
{
    return (F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0));
}

vec3 FresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return (F0 + (max(vec3(1- roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0));
}

//Trowbridge-Reitz GGX, NDF(Normal Distribute Function), a = roughness * roughness
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a*a;
    float NdotH = max(dot(N,H),0.0);
    float NdotH2 = NdotH * NdotH;

    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom; 
}

//Schlick-GGX, CommonAssetsmetry Obstruction
float CommonAssetsmetrySchlickGGX(float NdotV, float roughness)
{
    float r = roughness + 1.0;
    float k = (r*r) / 8.0;

    float nom = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

//Smith’s method, Calc both obstruction and sub-surface shadowing
float CommonAssetsmetrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N,V), 0.0);
    float NdotL = max(dot(N,L), 0.0);
    float ggx1 = CommonAssetsmetrySchlickGGX(NdotV,roughness);
    float ggx2 = CommonAssetsmetrySchlickGGX(NdotL,roughness);

    return ggx1 * ggx2;
}

vec3 CalcIrradiance(vec3 radiance , vec3 N,vec3 V,vec3 L, vec3 H, vec3 albedo,float roughness, float metallic, vec3 F0)
{
    // calc Fresnel, using Schlick
    float cosTheta = max(dot(H, V), 0.0);
    vec3 F = FresnelSchlick(cosTheta,F0);

    // GGX
    float NDF = DistributionGGX(N,H,roughness);
    float G = CommonAssetsmetrySmith(N, V, L, roughness);

    // Cook-Torrance BRDF
    float NdotL = max(dot(N,L), 0.0);
    float NdotV = max(dot(N,V), 0.0);
    vec3 nominator = NDF * G * F;
    float denominator = 4.0 * NdotV * NdotL + 0.001;
    vec3 specular = nominator / denominator;
    
    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= (1.0 - metallic);

    vec3 dLo = (kD * albedo / PI + specular) * radiance * NdotL;

    return dLo;
}

vec3 CalcDirLightIrradiance(DirLight light , vec3 N,vec3 V, vec3 albedo,float roughness, float metallic, vec3 F0)
{
    vec3 L = normalize(-light.direction.xyz);
    vec3 H = normalize(V + L);

    vec3 radiance = light.irradiance;

    return CalcIrradiance(radiance, N, V, L, H, albedo, roughness, metallic, F0);
}

vec3 CalcPointLightIrradiance(PointLight light , vec3 N,vec3 V, vec3 albedo,float roughness, float metallic, vec3 F0)
{
    vec3 L = normalize(light.position - fragPos);
    vec3 H = normalize(V + L);

    float lightDist = length(light.position - fragPos);
    float attenuation = 1 / (lightDist * lightDist);
    attenuation = clamp(attenuation,0,1);
    

    vec3 radiance = light.irradiance * attenuation;
    // vec3 radiance = vec3(3.0, 0, 0);
    // vec3 radiance = vec3(3.0, 0, 0);

    return CalcIrradiance(radiance, N, V, L, H, albedo, roughness, metallic, F0);
}

// https://learnopengl-cn.github.io/02%20Lighting/05%20Light%20casters/
vec3 CalcSpotLightIrradiance(SpotLight light , vec3 N,vec3 V, vec3 albedo,float roughness, float metallic, vec3 F0)
{
	vec3 L = normalize(light.position - fragPos);
    vec3 H = normalize(V + L);

	float theta     = dot(L, normalize(-light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 radiance = light.irradiance * intensity;

    return CalcIrradiance(radiance, N, V, L, H, albedo, roughness, metallic, F0);
}

vec3 ConvertNormalToWorldspace(vec3 tangentNormal)
{
    vec3 Q1  = dFdx(fragPos);
    vec3 Q2  = dFdy(fragPos);
    vec2 st1 = dFdx(TexCoord);
    vec2 st2 = dFdy(TexCoord);

    vec3 N   = normalize(Normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangentNormal);
}

void main()
{
    // Textures sampling
    vec3 albedo = texture(material.albedo,TexCoord).rgb; 
    albedo = pow(albedo,vec3(2.2f)); // convert from SRGB to linear space
    albedo *= intensity.tint;
    vec3 normal = texture(material.normal, TexCoord).xyz * 2.0 - 1.0;
    normal = ConvertNormalToWorldspace(normal); //PBR calc need wordspace normal
    float metallic = texture(material.metallic, TexCoord).r;
    metallic *= intensity.mro.x;
    float roughness = texture(material.roughness, TexCoord).r;
    roughness *= intensity.mro.y;
    float ao = pow(texture(material.ao, TexCoord).r,2.2);
    ao *= intensity.mro.z;

    // calc F0
    vec3 F0 = vec3(0.04f);
    F0 = mix(F0, albedo, metallic);

    vec3 N = normalize(normal);
    vec3 V = normalize(viewPos - fragPos);

    vec3 Lo = vec3(0.0);

    // calc irradiance in directional lights
    Lo += CalcDirLightIrradiance(dirLight, N, V, albedo, roughness, metallic , F0);    

    // calc irradiance in point lights
    for(int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		Lo += CalcPointLightIrradiance(pointLights[i], N, V, albedo, roughness,metallic, F0);    
	}

    // calc irradiance in spot lights
    Lo += CalcSpotLightIrradiance(spotLight, N, V, albedo, roughness,metallic, F0); 


    float cosTheta = max(dot(N, V), 0.0);
    vec3 Ks = FresnelSchlickRoughness(cosTheta, F0, roughness);
    vec3 Kd = 1 - Ks;
    Kd *= (1.0 - metallic);
    vec3 irradiance = texture(irradianceMap, N).rgb;
    vec3 diffuse = irradiance * albedo;
    vec3 ambient = Kd * diffuse * ao;
    vec3 color = ambient + Lo;

    // HDR to LDR
    color = color / (color + vec3(1.0));
    // Gamma correction
    color = clamp(pow(color, vec3(1.0/2.2)), 0.0, 1.0);
    // color += vColor;
    vec4 finalColor = vec4(color.r, color.g, color.b, 1.0f);

    // debug
    finalColor = mix(finalColor, vec4(albedo,1), debug_pbr.x);
    vec3 debugNormal = 0.5 * normal + 0.5;
    finalColor = mix(finalColor, vec4(debugNormal,1), debug_pbr.y);
    finalColor = mix(finalColor, vec4(metallic,metallic,metallic,1), debug_pbr.z);
    finalColor = mix(finalColor, vec4(roughness,roughness,roughness,1), debug_pbr.w);
    finalColor = mix(finalColor, vec4(ambient, 1), debug_light.x);
    
    // finalColor = vec4(diffuse, 1.0);
    // finalColor = vec4(Lo, 1.0);
    // Output final color
	FragColor = finalColor;
}
