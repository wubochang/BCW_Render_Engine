#version 330 core
out vec4 FragColor;

struct Material {
	sampler2D diffuse;
	sampler2D specular;    
	float shininess;
}; 

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

#define POINT_LIGHTS_NUM 4

in VS_OUT
{
	vec3 FragPos;  
	vec3 Normal;  
	vec4 vertColor;
	vec2 texCoord;
    vec4 FragPosLightSpace;
} fs_in;

uniform vec3 viewPos;
uniform Material material;
uniform PointLight pointLights[POINT_LIGHTS_NUM];
uniform DirLight dirLight;

uniform sampler2D shadowMap;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir);

void main()
{
	vec3 norm = normalize(fs_in.Normal);
    vec3 viewDir = normalize(viewPos -fs_in.FragPos);

	vec3 result = vec3(0,0,0);

	result = CalcDirLight(dirLight, norm, viewDir);
	for(int i = 0; i < POINT_LIGHTS_NUM; i++)
	{
        result += CalcPointLight(pointLights[i], norm, fs_in.FragPos, viewDir);    
	}


    // just for debug purpose
    //FragColor = vec4(vec3(1 - shadowColor), 1);
    FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.texCoord));
    float shadowColor = ShadowCalculation(fs_in.FragPosLightSpace, lightDir);
    return (ambient + (1 - shadowColor) * (diffuse + specular));
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
	vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(viewDir, halfwayDir), 0.0), material.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // combine results
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, fs_in.texCoord));
    vec3 diffuse = light.diffuse * diff * vec3(texture(material.diffuse, fs_in.texCoord));
    vec3 specular = light.specular * spec * vec3(texture(material.specular, fs_in.texCoord));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

float ShadowCalculation(vec4 fragPosLightSpace, vec3 lightDir)
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
    float bias = max(0.05 * (1.0 - dot(fs_in.Normal, lightDir)), 0.005);  
    // check whether current frag pos is in shadow
    float shadow = (currentDepth - bias) > closestDepth  ? 1.0 : 0.0;

    return shadow;
}