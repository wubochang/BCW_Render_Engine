
#ifndef _UI_MESH_H_
#define _UI_MESH_H_

#include "glad_include.h"

#include "Vertex.h"
#include "Mesh.h"

#include <vector>

class UIMesh : public Mesh
{
public:
	UIMesh();
	~UIMesh();

	static UIMesh* GetMesh()
	{
		static UIMesh* mesh = nullptr;
		if (mesh == nullptr)
		{
			mesh = new UIMesh();
			mesh->Initialize();
		}
		return mesh;
	}

private:

	void Initialize();
};

#endif // !_UI_MESH_H_
