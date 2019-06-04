
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

	void Initialize();

private:
};

#endif // !_UI_MESH_H_
