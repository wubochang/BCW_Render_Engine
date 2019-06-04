
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "Transform.h"
#include "Mesh.h"

class Transform; // forward decleration
class Mesh;
class Material;

class Object
{
public:
	Object();
	Object(const Object&);
	~Object();

	void Initialize(Mesh*);
	void Shutdown();

	Transform* GetTransform()
	{
		return m_transform;
	}

	Mesh* GetMesh()
	{
		return m_mesh;
	}

	Material* GetMaterial()
	{
		return m_material;
	}

	void SetMaterial(Material* mat)
	{
		m_material = mat;
	}

private:

	Transform* m_transform;

	Mesh* m_mesh;

	Material* m_material;
};


#endif // !_OBJECT_H_
