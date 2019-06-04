#include "RenderManager.h"

RenderManager g_renderManager;

RenderManager::RenderManager()
{
	m_shader = nullptr;
}

RenderManager::~RenderManager()
{
}

void RenderManager::Initialize()
{
	// init shaders
	m_shader = g_shaderManager.GetShader("shader");
	m_depthShader = g_shaderManager.GetShader("depthOnly");
	m_lightShader = g_shaderManager.GetShader("light");
	m_deferredGeoShader = g_shaderManager.GetShader("deferred_geometry");
	m_deferredLightShader = g_shaderManager.GetShader("deferred_light");
	m_skyboxShader = g_shaderManager.GetShader("skybox");

	m_GBuffer = new GBuffer();
	m_GBuffer->Initialize();

	m_uiMesh = new UIMesh();
	m_uiMesh->Initialize();

	m_cubeMap = new CubeMap();
	m_cubeMap->Initialize("../Textures/ame_limon", 
		{
#if false
			"Right.png",
			"Left.png",
			"Top.png",
			"Bottom.png",
			"Front.png",
			"Back.png",
#else
			// left and right reverted for source image
		"limon_lf.tga",
		"limon_rt.tga",
		"limon_up.tga",
		"limon_dn.tga",
		"limon_ft.tga",
		"limon_bk.tga",
#endif
		});

	m_frameBuffer = new FrameBuffer();
	m_frameBuffer->Initialize();

	InitDepthFrameBuffer();

	InitIBL();

	InitDebugUI();

	m_screen = new UIObject();
	m_screen->Initialize(m_frameBuffer->GetTexture());
	m_screen->screenPos = glm::vec2(SCR_WIDTH, SCR_HEIGHT) * 0.5f;
	m_screen->widthHeight = glm::vec2(SCR_WIDTH, SCR_HEIGHT);

	m_skyBoxCubeMap = m_prefilteredColorMap;

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
}

void RenderManager::InitDebugUI()
{

	for (int i = 0; i < 4; i++)
	{
		UIObject* newUI = new UIObject();
		newUI->Initialize(m_GBuffer->GetGAlbedo());
		m_debugUIs.push_back(newUI);
	}

	// from 0-3, top to bottom
	m_debugUIs[0]->screenPos = glm::vec2(50, 350); // top most
	m_debugUIs[0]->SetTexture(m_GBuffer->GetGAlbedo());

	m_debugUIs[1]->screenPos = glm::vec2(50, 250);
	m_debugUIs[1]->SetTexture(m_GBuffer->GetGNormal());

	m_debugUIs[2]->screenPos = glm::vec2(50, 150);
	m_debugUIs[2]->SetTexture(m_brdfLUT);

	m_debugUIs[3]->screenPos = glm::vec2(50, 50); // bottom most
	m_debugUIs[3]->SetTexture(m_GBuffer->GetGMetallicRoughnessAO());

	//m_uiObject = new UIObject();
	//m_uiObject->Initialize(m_GBuffer->GetGMetallicRoughnessAO()); // bind render texture to this ui for debug use

	// for debugging depth map
	//Shader* depthShader = g_shaderManager.GetShader("depthUI");
	//m_uiObject->Initialize(depthShader, m_depthFrameBuffer->GetTexture()); // bind render texture to this ui for debug use

	//m_uiObject->screenPos = glm::vec2(150, 150);
}

void RenderManager::InitIBL()
{
	glm::mat4x4 proj = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 views[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),		// right
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),	// left
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),		// top
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),		// bottom
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),		// front
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))		// back
	};
	Mesh* cubeMesh = g_objectManager.GetCubeMesh();

	InitIrradianceMap(proj, views, cubeMesh);
	InitPrefilteredColorMap(proj, views, cubeMesh);

	m_brdfLUT = new Texture();
	m_brdfLUT->Initialize("../Textures/ibl_brdf_lut.png");
}

void RenderManager::InitIrradianceMap(glm::mat4x4 &proj, glm::mat4  views[6], Mesh* cubeMesh)
{
	int irradianceWidth = 32;
	int irradianceHeight = 32;
	m_irradianceMap = new CubeMap();
	m_irradianceMap->Initialize(irradianceWidth, irradianceHeight);

	m_irradianceShader = g_shaderManager.GetShader("irradiance");

	m_irradianceFBO = new FrameBuffer();
	m_irradianceFBO->Initialize(m_irradianceMap->GetWidth(), m_irradianceMap->GetHeight());

	cubeMesh->SetBuffers();

	glViewport(0, 0, irradianceWidth, irradianceHeight);
	m_irradianceFBO->BindFrameBuffer();
	{
		m_irradianceShader->Use();
		m_irradianceShader->SetMat4("projection", proj);
		m_irradianceShader->SetCubeMap("environmentMap", m_cubeMap, 0);

		for (unsigned int i = 0; i < 6; i++)
		{
			m_irradianceShader->SetMat4("view", views[i]);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_irradianceMap->GetTextureID(), 0);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			glDrawElements(GL_TRIANGLES, cubeMesh->GetVertCnt(), GL_UNSIGNED_INT, 0);
		}
	}
	m_irradianceFBO->UnbindFrameBuffer();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}

void RenderManager::InitPrefilteredColorMap(glm::mat4x4 &proj, glm::mat4  views[6], Mesh* cubeMesh)
{
	int prefilterMapWidth = 128;
	int prefilterMapHeight = 128;
	m_prefilteredColorMap = new	CubeMap();
	m_prefilteredColorMap->Initialize(prefilterMapWidth, prefilterMapHeight);
	m_prefilteredColorMap->EnableMipMap();

	m_prefilterShader = g_shaderManager.GetShader("prefilter");

	m_prefilterFBO = new FrameBuffer();
	m_prefilterFBO->Initialize(m_prefilteredColorMap->GetWidth(), m_prefilteredColorMap->GetHeight());

	m_prefilterFBO->BindFrameBuffer();
	{
		m_prefilterShader->Use();
		m_prefilterShader->SetCubeMap("environmentMap", m_cubeMap, 0);
		m_prefilterShader->SetMat4("projection", proj);

		unsigned int maxMipLevels = 5;
		for (unsigned int mip = 0; mip < maxMipLevels; mip++)
		{
			// reisze framebuffer according to mip-level size.
			unsigned int mipWidth = 128 * std::pow(0.5, mip);
			unsigned int mipHeight = 128 * std::pow(0.5, mip);
			glBindRenderbuffer(GL_RENDERBUFFER, m_prefilterFBO->GetFrameBuffer());
			glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, mipWidth, mipHeight);

			glViewport(0, 0, mipWidth, mipHeight);

			float roughness = (float)mip / (float)(maxMipLevels - 1);
			m_prefilterShader->SetFloat("roughness", roughness);

			for (unsigned int i = 0; i < 6; i++)
			{
				m_prefilterShader->SetMat4("view", views[i]);

				glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, m_prefilteredColorMap->GetTextureID(), mip);
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				glDrawElements(GL_TRIANGLES, cubeMesh->GetVertCnt(), GL_UNSIGNED_INT, 0);
			}
		}
	}
	m_prefilterFBO->UnbindFrameBuffer();
	glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
}

void RenderManager::InitDepthFrameBuffer()
{
	Texture* depthTexture = new Texture();
	depthTexture->Initialize(GL_DEPTH_COMPONENT, GL_DEPTH_COMPONENT16, SHADOW_WIDTH, SHADOW_HEIGHT, GL_UNSIGNED_BYTE, nullptr);
	depthTexture->SetParameteri(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	depthTexture->SetParameteri(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	depthTexture->SetParameterfv(GL_TEXTURE_BORDER_COLOR, borderColor);
	m_depthFrameBuffer = new FrameBuffer();
	m_depthFrameBuffer->Initialize(depthTexture, GL_DEPTH_ATTACHMENT);
}

void RenderManager::Shutdown()
{
}

void RenderManager::Render()
{
	glm::mat4x4 proj = g_mainCamera.GetProjectionMatrix();
	glm::mat4x4 view = g_mainCamera.GetViewMatrix();

	DirectionalLight* dirLight = g_objectManager.GetMainDirLight();
	glm::mat4x4 lightProj = dirLight->GetProjectionMatrix();
	glm::mat4x4 lightView = dirLight->GetViewMatrix();

	// deferred geometry pass, render everything into G buffer
	DeferredGeometryPass(proj, view);

	// render depth
	RenderDepthMap(lightProj, lightView);

	// deferred lightning pass
	DeferredLightningPass(lightProj, lightView);

	// copy GBuffer's depth buffer to m_frameBuffer's depth texture
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBuffer->GetFrameBuffer());
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_frameBuffer->GetFrameBuffer());
	glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// then do the normal forward rendering 
	m_frameBuffer->BindFrameBuffer();
	{
		RenderLightMeshes_Debug(proj, view);

		RenderSkyBox(view, proj);
	}
	m_frameBuffer->UnbindFrameBuffer();

	// from render buffer to ui
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	m_screen->RenderUIObject(0.5);

	// render all debug UIs
	for (auto ui : m_debugUIs)
	{
		ui->RenderUIObject(0.0);
	}
	//m_uiObject->RenderUIObject(0.0);
}

void RenderManager::ForwardRender(glm::mat4x4 &proj, glm::mat4x4 &view, glm::mat4x4 &lightProj, glm::mat4x4 &lightView)
{
	m_frameBuffer->BindFrameBuffer();
	{
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		RenderOpaqueObjects(proj, view, lightProj, lightView);
		RenderLightMeshes_Debug(proj, view);
	}
	m_frameBuffer->UnbindFrameBuffer();
}

void RenderManager::DeferredGeometryPass(glm::mat4x4 &proj, glm::mat4x4 &view)
{
	//geometry pass for Deferred rendering
	glEnable(GL_CULL_FACE);
	m_GBuffer->BindFrameBuffer();
	{
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_deferredGeoShader->Use();
		m_deferredGeoShader->SetMat4("projection", proj);
		m_deferredGeoShader->SetMat4("view", view);

		for (Object* obj : g_objectManager.GetObjects())
		{
			glm::mat4x4 model = obj->GetTransform()->GetWorldMatrix();
			m_deferredGeoShader->SetMat4("model", model);

			m_deferredGeoShader->SetMaterial(obj->GetMaterial());

			obj->GetMesh()->SetBuffers();
			glDrawElements(GL_TRIANGLES, obj->GetMesh()->GetVertCnt(), GL_UNSIGNED_INT, 0);
		}
	}
	m_GBuffer->UnbindFrameBuffer();
	glDisable(GL_CULL_FACE);
}

void RenderManager::DeferredLightningPass(glm::mat4x4 &lightProj, glm::mat4x4 &lightView)
{
	m_frameBuffer->BindFrameBuffer();
	{
		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// deferred rendering light pass
		m_deferredLightShader->Use();
		m_deferredLightShader->SetTexture("gPosition", m_GBuffer->GetGPosition(), 0);
		m_deferredLightShader->SetTexture("gNormal", m_GBuffer->GetGNormal(), 1);
		m_deferredLightShader->SetTexture("gAlbedo", m_GBuffer->GetGAlbedo(), 2);
		m_deferredLightShader->SetTexture("gMetallicRoughnessAO", m_GBuffer->GetGMetallicRoughnessAO(), 3);
		m_deferredLightShader->SetCubeMap("irradianceMap", m_irradianceMap, 7);
		m_deferredLightShader->SetCubeMap("prefilterMap", m_prefilteredColorMap, 11);
		m_deferredLightShader->SetTexture("brdfLUT", m_brdfLUT, 12);
		m_deferredLightShader->SetVec3("viewPos", g_mainCamera.GetPosition());

		auto lights = g_objectManager.GetLights();
		for (unsigned int i = 0; i < lights.size(); i++)
		{
			Light* pLight = lights[i];
			pLight->SetLight(m_deferredLightShader);
		}

		m_deferredLightShader->SetFloat("pointLightNum", lights.size() - 1);
		m_deferredLightShader->SetTexture("shadowMap", m_depthFrameBuffer->GetTexture(), 5);
		m_deferredLightShader->SetMat4("lightProj", lightProj);
		m_deferredLightShader->SetMat4("lightView", lightView);

		m_uiMesh->SetBuffers();
		glDrawElements(GL_TRIANGLES, m_uiMesh->GetVertCnt(), GL_UNSIGNED_INT, 0);

	}
	m_frameBuffer->UnbindFrameBuffer();
}

void RenderManager::RenderOpaqueObjects(const glm::mat4x4 &proj, const glm::mat4x4 &view, const glm::mat4x4 &lightProj, const glm::mat4x4 &lightView )
{
	m_shader->Use();
	//m_shader->SetMaterial(m_material);
	m_shader->SetTexture("shadowMap", m_depthFrameBuffer->GetTexture(), 3);

	auto lights = g_objectManager.GetLights();
	for (unsigned int i = 0; i < lights.size(); i++)
	{
		Light* pLight = lights[i];
		pLight->SetLight(m_shader);
	}
	m_shader->SetMat4("projection", proj);
	m_shader->SetMat4("view", view);
	m_shader->SetVec3("viewPos", g_mainCamera.GetPosition());
	m_shader->SetMat4("lightProj", lightProj);
	m_shader->SetMat4("lightView", lightView);

	for (Object* obj : g_objectManager.GetObjects())
	{
		glm::mat4x4 model = obj->GetTransform()->GetWorldMatrix();
		m_shader->SetMat4("model", model);
		m_shader->SetMaterial(obj->GetMaterial());

		obj->GetMesh()->SetBuffers();
		glDrawElements(GL_TRIANGLES, obj->GetMesh()->GetVertCnt(), GL_UNSIGNED_INT, 0);
	}
}

void RenderManager::RenderDepthMap(const glm::mat4x4 & proj, const glm::mat4x4 & view)
{
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	m_depthFrameBuffer->BindFrameBuffer();
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		m_depthShader->Use();
		m_depthShader->SetMat4("projection", proj);
		m_depthShader->SetMat4("view", view);

		for (Object* obj : g_objectManager.GetObjects())
		{
			glm::mat4x4 model = obj->GetTransform()->GetWorldMatrix();
			m_depthShader->SetMat4("model", model);

			obj->GetMesh()->SetBuffers();
			glDrawElements(GL_TRIANGLES, obj->GetMesh()->GetVertCnt(), GL_UNSIGNED_INT, 0);
		}
	}
	m_depthFrameBuffer->UnbindFrameBuffer();
}

void RenderManager::RenderLightMeshes_Debug(const glm::mat4x4 &proj, const glm::mat4x4 &view)
{
	m_lightShader->Use();
	m_lightShader->SetMat4("projection", proj);
	m_lightShader->SetMat4("view", view);

	Mesh* cubeMesh = g_objectManager.GetCubeMesh();
	cubeMesh->SetBuffers();

	auto lights = g_objectManager.GetLights();
	for (Light* light : lights)
	{
		glm::mat4x4 model = light->GetTransform()->GetWorldMatrix();
		m_lightShader->SetMat4("model", model);
		m_lightShader->SetVec3("lightColor", light->albedo);

		glDrawElements(GL_TRIANGLES, cubeMesh->GetVertCnt(), GL_UNSIGNED_INT, 0);
	}
}

void RenderManager::RenderSkyBox(const glm::mat4x4 & view, const glm::mat4x4 & proj)
{
	Mesh* cubeMesh = g_objectManager.GetCubeMesh();
	cubeMesh->SetBuffers();

	// skybox
	glDepthFunc(GL_LEQUAL);
	{
		m_skyboxShader->Use();
		glm::mat4 skyBoxView = glm::mat4(glm::mat3(view));
		m_skyboxShader->SetMat4("projection", proj);
		m_skyboxShader->SetMat4("view", skyBoxView);
		m_skyboxShader->SetCubeMap("skybox", m_skyBoxCubeMap, 1);

		glDrawElements(GL_TRIANGLES, cubeMesh->GetVertCnt(), GL_UNSIGNED_INT, 0);
	}
	glDepthFunc(GL_LESS);
}