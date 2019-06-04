#ifndef _MESH_LOADER_H_
#define _MESH_LOADER_H_

#include "Mesh.h"
#include "ObjectManager.h"

class MeshLoader
{
public:
	MeshLoader();
	~MeshLoader();

	std::vector<Mesh*> LoadFromList(std::vector<std::string> filePaths);

	std::string RelativeToAbsolutePath(std::string str)
	{
		char dir[256];
		char* fileExt;
		GetFullPathName(str.c_str(), 256, dir, &fileExt);

		return std::string(dir);
	}

private:

	void LoadFromFBX(std::string filePath, std::vector<Mesh*>& meshes);

	Mesh* LoadMesh(fbxsdk::FbxMesh * pMesh);

	void ReadColor(FbxMesh * pMesh, int ctrlPointIndex, int vertexCounter, FbxColor * pColor);
	bool ReadUV(FbxMesh* pMesh, int ctrlPointIndex, int textureUVIndex, int uvLayer, FbxVector2* pUV);
	void ReadNormal(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, FbxVector4* pNormal);
	void ReadTangent(FbxMesh* pMesh, int ctrlPointIndex, int vertecCounter, FbxVector4* pTangent);
};

extern MeshLoader g_meshLoader;
#endif // !_MESH_LOADER_H_

