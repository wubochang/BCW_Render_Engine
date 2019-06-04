#ifndef _MATERIAL_MANAGER_H_
#define _MATERIAL_MANAGER_H_

#include "Material.h"

#include <unordered_map>
#include <string>

class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();

	void Initialize();

	void AddNewMaterial(std::string name, Material* mat);

	void AddNewMaterial(std::string name, std::string albedoPath, std::string normalPath, std::string metallicPath, std::string roughnessPath, std::string AOPath);

	Material* GetMaterial(std::string name);
	std::vector<Material*> GetMaterials(std::vector<std::string> names);

private:

	std::unordered_map<std::string, Material*> m_materials;
};

extern MaterialManager g_materialManager;

#endif // !_MATERIAL_MANAGER_H_