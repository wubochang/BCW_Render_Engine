
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

	void Initialize(std::string albedoPath, std::string normalPath, std::string metallicPath, std::string roughnessPath, std::string AOPath, std::string heightPath = std::string())
	{
#define CHECK_NULL(x) x.c_str()

		Initialize(
			CHECK_NULL(albedoPath),
			CHECK_NULL(normalPath),
			CHECK_NULL(metallicPath),
			CHECK_NULL(roughnessPath),
			CHECK_NULL(AOPath),
			CHECK_NULL(heightPath)
			);
	}

	void Initialize(const char* albedoPath, const char* normalPath, const char* metallicPath, const char* roughnessPath, const char* AOPath, const char* heightPath)
	{
#define LOAD_TEXTURE(x)	x = new Texture(); x->Initialize(x##Path);

		LOAD_TEXTURE(albedo);
		LOAD_TEXTURE(normal);
		LOAD_TEXTURE(metallic);
		LOAD_TEXTURE(roughness);
		LOAD_TEXTURE(AO);

		if (std::strlen(heightPath) != 0)
		{
			LOAD_TEXTURE(height);
			needPOM = true;
		}
		else
		{
			height = GetBlackTexture();
			needPOM = false;
		}
	}

	// if some of the texture is null, use GetBlackTexture() to get a completely black one
	void Initialize(Texture* inAlbedo, Texture* inNormal, Texture* inMetallic, Texture* inRoughness, Texture* inAO, Texture* inHeight)
	{
		albedo = inAlbedo;
		normal = inNormal;
		metallic = inMetallic;
		roughness = inRoughness;
		AO = inAO;
		height = inHeight;
	}

	Texture* albedo;
	Texture* normal;
	Texture* metallic;
	Texture* roughness;
	Texture* AO;
	Texture* height;

	// need parallax occlusion mapping
	bool needPOM;

	// return a texture that every pixel is black
	static Texture* GetBlackTexture()
	{
		static Texture* tex = nullptr;
		if (tex == nullptr)
		{
			tex = new Texture();
			tex->Initialize("../Textures/black.png");
		}
		return tex;
	}

	// return a texture that every pixel is white 
	static Texture* GetWhiteTexture()
	{
		static Texture* tex = nullptr;
		if (tex == nullptr)
		{
			tex = new Texture();
			tex->Initialize("../Textures/white.png");
		}
		return tex;
	}
private:

};

#endif // !_MATERIAL_H_
