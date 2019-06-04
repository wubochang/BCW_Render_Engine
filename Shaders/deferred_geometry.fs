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

void main()
{
    gPosition = fs_in.FragPos;

	vec3 normal = texture(material.normal, fs_in.texCoord).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	gNormal = normalize(fs_in.TBN * normal);

    gAlbedo.rgb = texture(material.albedo, fs_in.texCoord).rgb;

	float metallic = texture(material.metallic, fs_in.texCoord).r;
	float roughness = texture(material.roughness, fs_in.texCoord).r;
	float AO = texture(material.AO, fs_in.texCoord).r;

	gMetallicRoughnessAO = vec3(metallic, roughness, AO);

}
