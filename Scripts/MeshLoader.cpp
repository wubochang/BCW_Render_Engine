#include "MeshLoader.h"

MeshLoader g_meshLoader;

MeshLoader::MeshLoader()
{
}

MeshLoader::~MeshLoader()
{
}

std::vector<Mesh*> MeshLoader::LoadFromList(std::vector<std::string> filePaths)
{
	std::vector<Mesh*> meshes;
	for (int i = 0; i < filePaths.size(); i++)
	{
		LoadFromFBX(filePaths[i], meshes);
	}
	return meshes;
}

void MeshLoader::LoadFromFBX(std::string filePath, std::vector<Mesh*>& meshes)
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

