#include "ObjectManager.h"

void ObjectManager::Initialize()
{
	m_cubeMesh = nullptr;

	LoadMeshes();

	InitialObjects();

	InitialLights();

	Camera::GetMainCamera().Initialize();
}

void ObjectManager::LoadMeshes()
{
	std::vector<std::string> filePaths =
	{
		g_meshLoader.RelativeToAbsolutePath("../Assets/box.fbx"),
		g_meshLoader.RelativeToAbsolutePath("../Assets/hammer.fbx"),
		g_meshLoader.RelativeToAbsolutePath("../Assets/plane.fbx"),
		g_meshLoader.RelativeToAbsolutePath("../Assets/sphere.fbx"),
	};

	std::vector<Mesh*> meshes = g_meshLoader.LoadFromList(filePaths);

	for (int i = 0; i < meshes.size(); i++)
	{
		Mesh* mesh = meshes[i];
		m_meshes[mesh->name] = mesh;
	}
}

void ObjectManager::InitialObjects()
{
	Mesh* m = GetMesh("box");
	auto sphereMats = MaterialManager::getInstance().GetMaterials({
		"ConcreteMandala",
		"SmoothMetal",
		//"Brick",
		//"Tile025",
		"MetalGrid",
		//"RustyPanel",
		//"ChippedMetal"
		});

	/*
	std::vector<glm::vec3> cubePositions = {
		glm::vec3(-3.0f, 1.001f, 3.0f),
		glm::vec3(3.0f, 1.001f, 3.0f),
	};

	for (int i = 0; i < cubePositions.size(); i++)
	{
		Object* obj = CreateObject(m);
		obj->SetMaterial(brickMat);
		glm::vec3 rot = glm::vec3(0, glm::radians(i * 30.0f), 0);
		obj->GetTransform()->SetPosition(cubePositions[i]);
		obj->GetTransform()->SetRotationEuler(rot);
		obj->GetTransform()->SetScale(glm::vec3(2.0f));
	}
	*/

	m = GetMesh("plane");
	Object* plane = CreateObject(m);
	plane->SetMaterial(sphereMats[1]);
	plane->GetTransform()->SetPosition(glm::vec3(0, 0, 0));
	plane->GetTransform()->SetScale(glm::vec3(8, 1, 8));

	m = GetMesh("sphere");
	int sphereNum = sphereMats.size();
	for (int i = 0; i < sphereNum; i++)
	{
		Object* sphere = CreateObject(m);
		sphere->SetMaterial(sphereMats[i]);
		sphere->GetTransform()->SetPosition(glm::vec3(0, 2.1, (i - (sphereNum - 1)/2.0f) * 2.5f));
		sphere->GetTransform()->SetScale(glm::vec3(1));
	}
}

void ObjectManager::Update()
{
	Camera::GetMainCamera().Update();

	double deltaTime = Timer::getInstance().GetDeltaTime();

	static double s_time = 0;
	static bool s_rotateMainLight = true;
	if (s_rotateMainLight)
	{
		s_time += deltaTime;
		DirectionalLight* dirLight = m_mainDirLight;
		dirLight->GetTransform()->SetPosition(glm::vec3(cos(s_time), 4.0f, sin(s_time)));
		dirLight->GetTransform()->SetFowardUp(glm::vec3(-cos(s_time), -1.0f, -sin(s_time)), glm::vec3(0, 1, 0));
	}

	if (InputManager::getInstance().GetKeyDown(GLFW_KEY_SPACE))
	{
		s_rotateMainLight = !s_rotateMainLight;
	}

	if (InputManager::getInstance().GetKeyDown(GLFW_KEY_I))
	{
		RenderManager::getInstance().ToggleSkybox();
	}

	bool arrowUpDown = InputManager::getInstance().GetKeyDown(GLFW_KEY_UP);
	bool arrowDownDown = InputManager::getInstance().GetKeyDown(GLFW_KEY_DOWN);

	if (arrowUpDown || arrowDownDown)
	{
		float factor = -0.5f;
		if (arrowUpDown)
		{
			factor *= -1;
		}
		for (unsigned int i = 0; i < m_lights.size() - 1; i++)
		{
			PointLight* pLight = dynamic_cast<PointLight*>(m_lights[i]);

			float radius = 1.0f / pLight->constant;

			radius += factor;

			if (radius < 0.2f)
			{
				radius = 0.2f;
			}

			pLight->constant = 1.0f / radius;

			std::cout << "Radius for point light[" << i << "]:" << radius << std::endl;
		}
	}
}

Object* ObjectManager::CreateObject(Mesh * pMesh)
{
	Object* obj = new Object();
	obj->Initialize(pMesh);

	m_objects.push_back(obj);

	return obj;
}

void ObjectManager::InitialLights()
{
	std::vector<glm::vec3> pointLightsPositions = {
		glm::vec3(0.7f,  0.7f,  2.0f),
		glm::vec3(2.3f, 0.3f, -1.0f),
		glm::vec3(-1.4f,  0.4f, -2.0f),
		glm::vec3(0.0f,  1.0f, -3.0f)
	};

	int pointLightsNum = pointLightsPositions.size();

	for (int i = 0; i < pointLightsNum; i++)
	{
		PointLight* pLight = new PointLight();
		pLight->Initialize();
		m_lights.push_back(pLight);
	}

	for (int i = 0; i < pointLightsNum; i++)
	{
		m_lights[i]->GetTransform()->SetPosition(pointLightsPositions[i]);
	}

	m_lights[0]->albedo = glm::vec3(1, 0, 0);
	m_lights[1]->albedo = glm::vec3(1, 1, 0);
	m_lights[2]->albedo = glm::vec3(1, 0, 1);
	m_lights[3]->albedo = glm::vec3(0, 1, 0);

	DirectionalLight* pDirLight = new DirectionalLight();
	pDirLight->Initialize();
	pDirLight->ambient = glm::vec3(0, 0, 0);
	pDirLight->albedo = glm::vec3(1, 1, 1);
	pDirLight->GetTransform()->SetPosition(glm::vec3(-2.0f, 4.0f, -1.0f));
	pDirLight->GetTransform()->SetFowardUp(glm::vec3(1.0f, -1.0f, 1.0f), glm::vec3(0, 1, 0));

	m_lights.push_back(pDirLight);

	m_mainDirLight = pDirLight;
}
