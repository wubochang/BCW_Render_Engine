#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMetallicRoughnessAO;

struct DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

// light
const int POINT_LIGHTS_NUM = 32;
uniform PointLight pointLights[POINT_LIGHTS_NUM];
uniform DirLight dirLight;
uniform float pointLightNum;

// IBL
uniform samplerCube irradianceMap;
uniform samplerCube prefilterMap;
uniform sampler2D brdfLUT;  

// shadow
uniform mat4 lightProj;
uniform mat4 lightView;
uniform sampler2D shadowMap;

// camera 
uniform vec3 viewPos;

const float PI = 3.14159265359;

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 fragPos, vec3 viewDir,  vec3 albedo, vec3 metallicRoughnessAO, vec4 FragPosLightSpace);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir,  vec3 albedo, vec3 metallicRoughnessAO);
vec3 CalcLight(vec3 normal, vec3 viewDir, vec3 halfwayDir, vec3 lightDir, vec3 albedo, vec3 metallicRoughnessAO, vec3 radiance);
vec3 CalcAmbient(vec3 normal, vec3 viewDir, vec3 albedo, vec3 metallicRoughnessAO);

float DistributionGGX(vec3 normal, vec3 halfwayDir, float roughness);
float GeometrySchlickGGX(float NDotV, float roughness);
float GeometrySmith(vec3 normal, vec3 viewDir, vec3 lightDir, float roughness);
vec3 fresnel(float cosTheta, vec3 F0);
vec3 fresnel(float cosTheta, vec3 F0, float roughness);

void main()
{
	vec3 fragPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = normalize(texture(gNormal, TexCoords).rgb);
    vec3 viewDir = normalize(viewPos - fragPos);

    vec3 albedo = texture(gAlbedo, TexCoords).rgb;
    vec3 metallicRoughnessAO = texture(gMetallicRoughnessAO, TexCoords).rgb;
    float AO = metallicRoughnessAO.b;

	vec4 FragPosLightSpace = lightProj * lightView * vec4(fragPos, 1.0);

    vec3 Lo = vec3(0);

    Lo = CalcDirLight(dirLight, normal, fragPos, viewDir, albedo, metallicRoughnessAO, FragPosLightSpace);

	for(int i = 0; i < pointLightNum; i++)
    {
        Lo += CalcPointLight(pointLights[i], normal, fragPos, viewDir, albedo, metallicRoughnessAO);
    }

    vec3 ambient = CalcAmbient(normal, viewDir, albedo, metallicRoughnessAO);

    vec3 color = ambient + Lo;

    color = color / (color + vec3(1.0));

    color = pow(color, vec3(1.0/2.2));  

    FragColor = vec4(color, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 fragPos, vec3 viewDir,  vec3 albedo, vec3 metallicRoughnessAO, vec4 FragPosLightSpace)
{
    vec3 lightDir = -normalize(light.direction);
    vec3 halfwayDir = normalize(viewDir + lightDir);

    vec3 radiance = light.diffuse;

    float shadowColor = ShadowCalculation(FragPosLightSpace, lightDir, normal);

    radiance *= (1 - shadowColor);

    return CalcLight(normal, viewDir, halfwayDir, lightDir, albedo, metallicRoughnessAO, radiance);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir,  vec3 albedo, vec3 metallicRoughnessAO)
{
    vec3 lightDir = normalize(light.position - fragPos);
    vec3 halfwayDir = normalize(viewDir + lightDir);

    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist+ light.quadratic * (dist* dist));    

    vec3 radiance = light.diffuse * attenuation;

    return CalcLight(normal, viewDir, halfwayDir, lightDir, albedo, metallicRoughnessAO, radiance);
}

vec3 CalcLight(vec3 normal, vec3 viewDir, vec3 halfwayDir, vec3 lightDir, vec3 albedo, vec3 metallicRoughnessAO, vec3 radiance )
{
    float metallic = metallicRoughnessAO.r;
    float roughness = metallicRoughnessAO.g;
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    float NDF = DistributionGGX(normal, halfwayDir, roughness);
    float G = GeometrySmith(normal, viewDir, lightDir, roughness);
    vec3 F = fresnel(clamp(dot(halfwayDir, viewDir), 0.0, 1.0), F0);

    vec3 kS = F;
    vec3 kD = vec3(1.0) - kS;
    kD *= 1.0 - metallic;

    vec3 nominator = NDF * G * F;
    float denominator = 4.0 * max(dot(normal, viewDir), 0.0) * max(dot(normal, lightDir), 0.0) + 0.001;
    vec3 specular = nominator / denominator;

    float NDotL = max(dot(normal, lightDir), 0.0);

    vec3 res = (kD * albedo / PI + specular) * radiance * NDotL;

    return res;
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r; 
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // add bias to reduce shadow acne
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);  
    // check whether current frag pos is in shadow
    float shadow = (currentDepth - bias) > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

vec3 CalcAmbient(vec3 normal, vec3 viewDir, vec3 albedo, vec3 metallicRoughnessAO)
{
    float metallic = metallicRoughnessAO.r;
    float roughness = metallicRoughnessAO.g;
    float AO = metallicRoughnessAO.b;

    vec3 irradiance = texture(irradianceMap, normal).rgb;
    vec3 diffuse = irradiance * albedo;
    vec3 R = reflect(-viewDir, normal); 

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);
    vec3 F = fresnel(max(dot(normal, viewDir), 0.0), F0, roughness);
    vec3 kS = F;
    vec3 kD = 1.0 - kS;
    kD *= 1.0 - metallic;

    const float MAX_REFLECTION_LOD = 4.0;

    vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(brdfLUT, vec2(max(dot(normal, -viewDir), 0.0), roughness)).rg;
    vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * AO;

    //ambient = specular;

    // use irradiance map to replace constant ambient;
    //ambient = AO * albedo * vec3(0.03);
    return ambient;
}

vec3 fresnel(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}  

vec3 fresnel(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(1.0 - cosTheta, 5.0);
}  

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / max(denom, 0.001); // prevent divide by zero for roughness=0.0 and NdotH=1.0
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}