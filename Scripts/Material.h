
#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "Shader/Shader.h"
#include "Texture.h"

#include <string>

class Material
{
public:
	Material();
	Material(const Material&);
	~Material();

	void Initialize(std::string albedoPath, std::string normalPath, std::string metallicPath, std::string roughnessPath, std::string AOPath)
	{
		Initialize(albedoPath.c_str(), normalPath.c_str(), metallicPath.c_str(), roughnessPath.c_str(), AOPath.c_str());
	}

	void Initialize(const char* albedoPath, const char* normalPath, const char* metallicPath, const char* roughnessPath, const char* AOPath)
	{
#define LOAD_TEXTURE(x)	x = new Texture(); x->Initialize(x##Path);

		LOAD_TEXTURE(albedo);
		LOAD_TEXTURE(normal);
		LOAD_TEXTURE(metallic);
		LOAD_TEXTURE(roughness);
		LOAD_TEXTURE(AO);
	}

	void Initialize(Texture* inAlbedo, Texture* inNormal, Texture* inMetallic, Texture* inRoughness, Texture* inAO)
	{
		albedo = inAlbedo;
		normal = inNormal;
		metallic = inMetallic;
		roughness = inRoughness;
		AO = inAO;
	}

	Texture* albedo;
	Texture* normal;
	Texture* metallic;
	Texture* roughness;
	Texture* AO;

private:

};

#endif // !_MATERIAL_H_
