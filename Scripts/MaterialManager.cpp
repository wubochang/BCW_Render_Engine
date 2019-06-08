#include "MaterialManager.h"

void MaterialManager::Initialize()
{
	m_materials = std::unordered_map<std::string, Material*>();

	AddNewMaterial("ConcreteMandala",
		"../Textures/ConcreteMandala/albedo.png",
		"../Textures/ConcreteMandala/normal.png",
		"../Textures/ConcreteMandala/metallic.png",
		"../Textures/ConcreteMandala/roughness.png",
		"../Textures/ConcreteMandala/AO.png",
		"../Textures/ConcreteMandala/height.png"
	);

	AddNewMaterial("Brick",
		"../Textures/Brick/albedo.png",
		"../Textures/Brick/normal.png",
		"../Textures/Brick/metallic.png",
		"../Textures/Brick/roughness.png",
		"../Textures/Brick/AO.png"
	);

	AddNewMaterial("Tile025",
		"../Textures/Tile025/Tiles_025_basecolor.jpg",
		"../Textures/Tile025/Tiles_025_normal.jpg",
		"../Textures/Tile025/metallic.png",
		"../Textures/Tile025/Tiles_025_roughness.jpg",
		"../Textures/Tile025/Tiles_025_ambientOcclusion.jpg"
	);


	AddNewMaterial("MetalGrid",
		"../Textures/MetalGrid/metalgrid3_basecolor.png",
		"../Textures/MetalGrid/metalgrid3_normal-ogl.png",
		"../Textures/MetalGrid/metalgrid3_metallic.png",
		"../Textures/MetalGrid/roughness.png",
		"../Textures/MetalGrid/metalgrid3_AO.png"
	);

	AddNewMaterial("RustyPanel",
		"../Textures/RustyPanel/rusty-panel-albedo3b.png",
		"../Textures/RustyPanel/rusty-panel-norma-ogl.png",
		"../Textures/RustyPanel/rusty-panel-metal.png",
		"../Textures/RustyPanel/roughness.png",
		"../Textures/RustyPanel/rusty-panel-ao3.png"
	);

	AddNewMaterial("ChippedMetal",
		"../Textures/ChippedMetal/chipped-paint-metal-albedo.png",
		"../Textures/ChippedMetal/chipped-paint-metal-normal-ogl.png",
		"../Textures/ChippedMetal/chipped-paint-metal.png",
		"../Textures/ChippedMetal/roughness.png",
		"../Textures/ChippedMetal/chipped-paint-ao.png"
	);
}

void MaterialManager::AddNewMaterial(std::string name, Material * mat)
{
	if (m_materials[name] != nullptr)
	{
		std::cout << " Warning! Material " << name << "already exists, overwriting previous one" << std::endl;
	}
	m_materials[name] = mat;
}

void MaterialManager::AddNewMaterial(std::string name, std::string albedoPath, std::string normalPath, std::string metallicPath, std::string roughnessPath, std::string AOPath, std::string heightPath /* = std::string() */)
{
	Material* mat = new Material();
	mat->Initialize(albedoPath, normalPath, metallicPath, roughnessPath, AOPath, heightPath);

	AddNewMaterial(name, mat);
}

Material * MaterialManager::GetMaterial(std::string name)
{
	Material* mat = m_materials[name];
	if (mat == nullptr)
	{
		std::cout << "Material " << name << " do not exist" << std::endl;
	}
	return mat;
}

std::vector<Material*> MaterialManager::GetMaterials(std::vector<std::string> names)
{
	std::vector<Material*> res;
	for (std::string name : names)
	{
		res.push_back(GetMaterial(name));
	}

	return res;
}
