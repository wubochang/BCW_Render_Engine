#include "MeshLoader.h"

MeshLoader::~MeshLoader()
{
}

std::vector<Mesh*> MeshLoader::LoadFromList(std::vector<std::string> filePaths)
{
	std::vector<Mesh*> fbxsdkMeshes;
	std::vector<Mesh*> aiMeshes;
	for (unsigned int i = 0; i < filePaths.size(); i++)
	{
		LoadFromFBX_fbxsdk(filePaths[i], fbxsdkMeshes);
		LoadFromFBX_assimp(filePaths[i], aiMeshes);
	}
	return aiMeshes;
}

void MeshLoader::LoadFromFBX_assimp(std::string filePath, std::vector<Mesh*>& meshes)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_CalcTangentSpace);

	if (!scene || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	ProcessNode(scene->mRootNode, scene, meshes);
}

void MeshLoader::ProcessNode(aiNode * node, const aiScene* scene, std::vector<Mesh*>& meshes)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		Mesh* m = ProcessMesh(scene->mMeshes[node->mMeshes[i]]);
		m->name = std::string(node->mName.C_Str());
		meshes.push_back(m);
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, meshes);
	}
}

Mesh * MeshLoader::ProcessMesh(aiMesh * mesh)
{
	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position.x = mesh->mVertices[i].x;
		vertex.position.y = mesh->mVertices[i].y;
		vertex.position.z = mesh->mVertices[i].z;

		vertex.color = glm::vec4(1.0f);
		/*
		vertex.color.x = mesh->mColors[i]->r;
		vertex.color.y = mesh->mColors[i]->g;
		vertex.color.z = mesh->mColors[i]->b;
		vertex.color.w = mesh->mColors[i]->a;
		*/

		vertex.normal.x = mesh->mNormals[i].x;
		vertex.normal.y = mesh->mNormals[i].y;
		vertex.normal.z = mesh->mNormals[i].z;

		vertex.tangent.x = mesh->mTangents[i].x;
		vertex.tangent.y = mesh->mTangents[i].y;
		vertex.tangent.z = mesh->mTangents[i].z;

		vertex.uv.x = mesh->mTextureCoords[0][i].x;
		vertex.uv.y = mesh->mTextureCoords[0][i].y;

		verts.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* m = new Mesh();
	m->Initialize(verts, indices);
	return m;
}

void MeshLoader::LoadFromFBX_fbxsdk(std::string filePath, std::vector<Mesh*>& meshes)
{

	auto pFbxSdkManager = FbxManager::Create();

	FbxIOSettings* pIOSettings = FbxIOSettings::Create(pFbxSdkManager, IOSROOT);

	pFbxSdkManager->SetIOSettings(pIOSettings);

	FbxImporter* pImporter = FbxImporter::Create(pFbxSdkManager, "");
	FbxScene* pFbxScene = FbxScene::Create(pFbxSdkManager, "");

	bool bSuccess = pImporter->Initialize(filePath.c_str(), -1, pFbxSdkManager->GetIOSettings());

	if (!bSuccess)
	{
		return;
	}
	bSuccess = pImporter->Import(pFbxScene);
	if (!bSuccess)
	{
		return;
	}

	FbxAxisSystem OurAxisSystem(FbxAxisSystem::eMax);
	FbxAxisSystem SceneAxisSystem = pFbxScene->GetGlobalSettings().GetAxisSystem();
	if (SceneAxisSystem != OurAxisSystem)
	{
		//OurAxisSystem.ConvertScene(pFbxScene);
	}

	FbxGeometryConverter lGeomConverter(pFbxSdkManager);
	lGeomConverter.Triangulate(pFbxScene, true);

	pImporter->Destroy();


	FbxNode* pFbxRootNode = pFbxScene->GetRootNode();
	if (pFbxRootNode)
	{
		int childCnt = pFbxRootNode->GetChildCount();
		for (int i = 0; i < childCnt; i++)
		{
			FbxNode* pFbxChildNode = pFbxRootNode->GetChild(i);

			if (pFbxChildNode->GetNodeAttribute() == NULL)
				continue;

			FbxNodeAttribute::EType AttributeType = pFbxChildNode->GetNodeAttribute()->GetAttributeType();

			if (AttributeType != FbxNodeAttribute::eMesh)
				continue;

			auto name = pFbxChildNode->GetName();
			FbxMesh* pMesh = (FbxMesh*)pFbxChildNode->GetNodeAttribute();
			Mesh* m = LoadMesh(pMesh);
			m->name = name;
			meshes.push_back(m);
		}
	}
}

Mesh * MeshLoader::LoadMesh(fbxsdk::FbxMesh * pMesh)
{
	std::vector<Vertex> verts;
	std::vector<unsigned int> indices;

	FbxVector4* pVertices = pMesh->GetControlPoints();

	int triangleCnt = pMesh->GetPolygonCount();

	int vertexCounter = 0;

	for (int j = 0; j < triangleCnt; j++)
	{
		int iNumVertices = pMesh->GetPolygonSize(j);

		for (int k = 0; k < iNumVertices; k++)
		{
			int iControlPointIndex = pMesh->GetPolygonVertex(j, k);

			Vertex vertex;
			vertex.position.x = (float)pVertices[iControlPointIndex].mData[0];
			vertex.position.y = (float)pVertices[iControlPointIndex].mData[1];
			vertex.position.z = (float)pVertices[iControlPointIndex].mData[2];

			FbxColor color;
			ReadColor(pMesh, iControlPointIndex, vertexCounter, &color);

			FbxVector2 uv;
			ReadUV(pMesh, iControlPointIndex, pMesh->GetTextureUVIndex(j, k), 0, &uv);

			FbxVector4 normal;
			ReadNormal(pMesh, iControlPointIndex, vertexCounter, &normal);

			FbxVector4 tangent;
			ReadTangent(pMesh, iControlPointIndex, vertexCounter, &tangent);

			vertex.color = glm::vec4(color[0], color[1], color[2], color[3]);
			vertex.normal = glm::vec3(normal[0], normal[1], normal[2]);
			vertex.uv = glm::vec2(uv[0], uv[1]);
			vertex.tangent = glm::vec3(tangent[0], tangent[1], tangent[2]);

			verts.push_back(vertex);

			indices.push_back(vertexCounter);
			vertexCounter++;
		}
	}

	Mesh* m = new Mesh();
	m->Initialize(verts, indices);

	return m;
}

void MeshLoader::ReadColor(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, FbxColor* pColor)
{
	if (pMesh->GetElementVertexColorCount() < 1)
	{
		return;
	}

	FbxGeometryElementVertexColor* pVertexColor = pMesh->GetElementVertexColor(0);

	if (pVertexColor->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		int idx = ctrlPointIndex;
		if (pVertexColor->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			idx = ctrlPointIndex;
		}
		if (pVertexColor->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
		{
			idx = pVertexColor->GetIndexArray().GetAt(ctrlPointIndex);
		}
		*pColor = pVertexColor->GetDirectArray().GetAt(idx);
	}
	if (pVertexColor->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		int idx = vertexCounter;
		if (pVertexColor->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			idx = vertexCounter;
		}
		if (pVertexColor->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
		{
			idx = pVertexColor->GetIndexArray().GetAt(vertexCounter);
		}
		*pColor = pVertexColor->GetDirectArray().GetAt(idx);
	}
}

bool MeshLoader::ReadUV(FbxMesh* pMesh, int ctrlPointIndex, int textureUVIndex, int uvLayer, FbxVector2* pUV)
{
	if (uvLayer >= 2 || pMesh->GetElementUVCount() <= uvLayer)
	{
		return false;
	}

	FbxGeometryElementUV* pVertexUV = pMesh->GetElementUV(uvLayer);

	if (pVertexUV->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		int idx = ctrlPointIndex;
		if (pVertexUV->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			idx = ctrlPointIndex;
		}
		if (pVertexUV->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
		{
			idx = pVertexUV->GetIndexArray().GetAt(ctrlPointIndex);
		}
		*pUV = pVertexUV->GetDirectArray().GetAt(idx);
	}
	if (pVertexUV->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		*pUV = pVertexUV->GetDirectArray().GetAt(textureUVIndex);
	}

	return true;
}

void MeshLoader::ReadNormal(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, FbxVector4* pNormal)
{
	if (pMesh->GetElementNormalCount() < 1)
	{
		return;
	}

	FbxGeometryElementNormal* leNormal = pMesh->GetElementNormal(0);
	if (leNormal->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		int idx = ctrlPointIndex;
		if (leNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			idx = ctrlPointIndex;
		}
		if (leNormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
		{
			idx = leNormal->GetIndexArray().GetAt(ctrlPointIndex);
		}
		*pNormal = leNormal->GetDirectArray().GetAt(idx);
	}

	if (leNormal->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		int idx = vertexCounter;
		if (leNormal->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			idx = vertexCounter;
		}
		if (leNormal->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
		{
			idx = leNormal->GetIndexArray().GetAt(vertexCounter);
		}
		*pNormal = leNormal->GetDirectArray().GetAt(idx);
	}
}

void MeshLoader::ReadTangent(FbxMesh* pMesh, int ctrlPointIndex, int vertexCounter, FbxVector4* pTangent)
{
	if (pMesh->GetElementTangentCount() < 1)
	{
		return;
	}

	FbxGeometryElementTangent* leTangent = pMesh->GetElementTangent(0);

	if (leTangent->GetMappingMode() == FbxGeometryElement::eByControlPoint)
	{
		int idx = ctrlPointIndex;
		if (leTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			idx = ctrlPointIndex;
		}
		if (leTangent->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
		{
			idx = leTangent->GetIndexArray().GetAt(ctrlPointIndex);
		}
		*pTangent = leTangent->GetDirectArray().GetAt(idx);
	}

	if (leTangent->GetMappingMode() == FbxGeometryElement::eByPolygonVertex)
	{
		int idx = vertexCounter;
		if (leTangent->GetReferenceMode() == FbxGeometryElement::eDirect)
		{
			idx = vertexCounter;
		}
		if (leTangent->GetReferenceMode() == FbxGeometryElement::eIndexToDirect)
		{
			idx = leTangent->GetIndexArray().GetAt(vertexCounter);
		}
		*pTangent = leTangent->GetDirectArray().GetAt(idx);
	}
}

