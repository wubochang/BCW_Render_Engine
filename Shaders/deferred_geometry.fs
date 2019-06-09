#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec3 gAlbedo;
layout (location = 3) out vec3 gMetallicRoughnessAO;

struct Material {
	sampler2D albedo;
	sampler2D normal;
	sampler2D metallic;    
	sampler2D roughness;    
	sampler2D AO;    
	sampler2D height;    
}; 

in VS_OUT
{
	vec3 FragPos;  
	vec3 Normal;  
	vec4 vertColor;
	vec2 texCoord;
	mat3 TBN;
} fs_in;

uniform Material material;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);

void main()
{
	vec2 texCoord = fs_in.texCoord;

    gPosition = fs_in.FragPos;

	vec3 normal = texture(material.normal, texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	gNormal = normalize(fs_in.TBN * normal);

    gAlbedo.rgb = texture(material.albedo, texCoord).rgb;

	float metallic = texture(material.metallic, texCoord).r;
	float roughness = texture(material.roughness, texCoord).r;
	float AO = texture(material.AO, texCoord).r;

	gMetallicRoughnessAO = vec3(metallic, roughness, AO);

}