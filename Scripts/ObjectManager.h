
#ifndef _OBJECT_MANAGER_H_
#define _OBJECT_MANAGER_H_

#include "Object.h"
#include "MeshLoader.h"
#include "Light.h"
#include "InputManager.h"

#include <vector>
#include <unordered_map>
#include <string>

class ObjectManager
{
public:
	ObjectManager();
	~ObjectManager();

	void Initialize();
	void Update(double deltaTime);

	Mesh* GetCubeMesh()
	{
		if (m_cubeMesh == nullptr)
		{
			m_cubeMesh = GetMesh("box");
		}
		return m_cubeMesh;
	}

	Object* CreateObject(Mesh* pMesh);
	Mesh* GetMesh(std::string name)
	{
		return m_meshes[name];
	}

	std::vector<Object*>& GetObjects()
	{
		return m_objects;
	}

	std::vector<Light*>& GetLights()
	{
		return m_lights;
	}

	DirectionalLight* GetMainDirLight()
	{
		return m_mainDirLight;
	}


private:

	void LoadMeshes();
	void InitialLights();
	void InitialObjects();

	std::vector<Object*> m_objects;
	std::unordered_map<std::string, Mesh*> m_meshes;
	std::vector<Light*> m_lights;
	DirectionalLight* m_mainDirLight;

	Mesh* m_cubeMesh;
};

extern ObjectManager g_objectManager;

#endif // !_OBJECT_MANAGER_H_
