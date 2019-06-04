#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
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

// shadow
uniform mat4 lightProj;
uniform mat4 lightView;
uniform sampler2D shadowMap;

// camera 
uniform vec3 viewPos;


vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 FragPosLightSpace, vec4 albedoSpec);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 albedoSpec);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir, vec3 normal);

void main()
{
	vec3 fragPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 Lo = vec3(0,0,0);

	for(int i = 0; i < pointLightNum; i++)
    {
        Lo += 
    }
}

void old_main()
{             
	vec3 FragPos = texture(gPosition, TexCoords).rgb;
    vec3 Normal = texture(gNormal, TexCoords).rgb;
    vec3 viewDir = normalize(viewPos - FragPos);

	vec4 FragPosLightSpace = lightProj * lightView * vec4(FragPos, 1.0);

	vec4 albedoSpec = texture(gAlbedoSpec, TexCoords);

	vec3 result = vec3(0,0,0);

	result = CalcDirLight(dirLight, Normal, viewDir, FragPosLightSpace, albedoSpec);
	for(int i = 0; i < pointLightNum; i++)
	{
        result += CalcPointLight(pointLights[i], Normal, FragPos, viewDir, albedoSpec);    
	}

    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, vec4 FragPosLightSpace, vec4 albedoSpec)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 32.0f);
    // combine results
    vec3 ambient = light.ambient * albedoSpec.rgb;
    vec3 diffuse = light.diffuse * diff * albedoSpec.rgb;
    vec3 specular = light.specular * spec * albedoSpec.a;
    float shadowColor = ShadowCalculation(FragPosLightSpace, lightDir, normal);
    //return ambient + diffuse + specular;
    return (ambient + (1 - shadowColor) * (diffuse + specular));
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 albedoSpec)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // specular shading
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), 32.0f);

    // attenuation
    float dist= length(light.position - fragPos) / 2;
    float attenuation = 1.0 / (light.constant + light.linear * dist+ light.quadratic * (dist* dist));    

    // combine results
    vec3 ambient = light.ambient * albedoSpec.rgb;
    vec3 diffuse = light.diffuse * diff * albedoSpec.rgb;
    vec3 specular = light.specular * spec * (albedoSpec.a);

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
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