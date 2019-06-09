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

    // need parallax occlusion mapping
    bool needPOM;
}; 

in VS_OUT
{
	vec3 FragPos;  
	vec3 Normal;  
	vec4 vertColor;
	vec2 texCoord;
	mat3 TBN;
    vec3 TangentViewDir;
} fs_in;

uniform Material material;

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir);

void main()
{
	vec2 texCoord = fs_in.texCoord;
    if(material.needPOM)
    {
	    vec3 tangentViewDir = normalize(fs_in.TangentViewDir);
	    texCoord = ParallaxMapping(fs_in.texCoord, tangentViewDir);
	    if(texCoord.x > 1.0 || texCoord.y > 1.0 || texCoord.x < 0.0 || texCoord.y < 0.0)
            discard;
    }

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

vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir)
{ 
    // number of depth layers
    const float minLayers = 10;
    const float maxLayers = 20;
    float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
    vec2 P = viewDir.xy / viewDir.z * 0.02; //height_scale; 
    vec2 deltaTexCoords = P / numLayers;
  
    // get initial values
    vec2  currentTexCoords     = texCoords;
    float currentDepthMapValue = 1 - texture(material.height, currentTexCoords).r;
      
    while(currentLayerDepth < currentDepthMapValue)
    {
        // shift texture coordinates along direction of P
        currentTexCoords -= deltaTexCoords;
        // get depthmap value at current texture coordinates
        currentDepthMapValue = 1 - texture(material.height, currentTexCoords).r;  
        // get depth of next layer
        currentLayerDepth += layerDepth;  
    }
    
    // -- parallax occlusion mapping interpolation from here on
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = (1 - texture(material.height, prevTexCoords).r) - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);

    return finalTexCoords;
}