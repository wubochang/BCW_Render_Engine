#ifndef _RENDER_MANAGER_H_
#define _RENDER_MANAGER_H_

#include "glad_include.h"
#include <GLFW/glfw3.h>

#include "Shader/Shader.h"
#include "Shader/ShaderManager.h"
#include "Texture.h"
#include "TextRenderer.h"
#include "CubeMap.h"
#include "Object.h"
#include "Vertex.h"
#include "Material.h"
#include "Light.h"
#include "Mesh.h"
#include "Camera.h"
#include "UIObject.h"

#include "Misc.h"

#include "FrameBuffer/FrameBuffer.h"
#include "FrameBuffer/GBuffer.h"

#include <vector>

class UIObject;
class FrameBuffer;
class GBuffer;
class RenderManager
{
	MAKE_SINGLETON(RenderManager)

public:
	~RenderManager() { };

	void Initialize();
	void Shutdown();

	void Render();

	void ToggleSkybox();

	inline int GetScreenWidth() { return m_width; }
	inline int GetScreenHeight() { return m_height; }

	void ResizeScreen(int newWidth, int newHeight);


private:

	int m_width;
	int m_height;

	Shader* m_shader;
	Shader* m_depthShader;
	Shader* m_lightShader;
	Shader* m_skyboxShader;
	Shader* m_irradianceShader;
	Shader* m_prefilterShader;

	// Deferred rendering 
	Shader* m_deferredGeoShader;
	Shader* m_deferredLightShader;
	GBuffer* m_GBuffer;
	CubeMap* m_irradianceMap;
	CubeMap* m_prefilteredColorMap;
	Texture* m_brdfLUT;
	UIMesh* m_uiMesh;

	CubeMap* m_cubeMap;
	CubeMap* m_skyBoxCubeMap;

	std::vector<UIObject*> m_debugUIs;
	UIObject* m_uiObject;
	UIObject* m_screen;

	FrameBuffer* m_frameBuffer;
	FrameBuffer* m_depthFrameBuffer;

	FrameBuffer* m_irradianceFBO;
	FrameBuffer* m_prefilterFBO;

	void InitDepthFrameBuffer();
	void InitIBL();
	void InitDebugUI();
	void InitIrradianceMap(glm::mat4x4 &proj, glm::mat4  views[6], Mesh* cubeMesh);
	void InitPrefilteredColorMap(glm::mat4x4 &proj, glm::mat4  views[6], Mesh* cubeMesh);

	void DeferredGeometryPass(glm::mat4x4 &proj, glm::mat4x4 &view);
	void DeferredLightningPass(glm::mat4x4 &lightProj, glm::mat4x4 &lightView);

	void ForwardRender(glm::mat4x4 &proj, glm::mat4x4 &view, glm::mat4x4 &lightProj, glm::mat4x4 &lightView);

	void RenderOpaqueObjects(const glm::mat4x4 &proj, const glm::mat4x4 &view, const glm::mat4x4 &lightProj, const glm::mat4x4 &lightView);
	void RenderDepthMap(const glm::mat4x4& proj, const glm::mat4x4& view);
	void RenderLightMeshes_Debug(const glm::mat4x4 &proj, const glm::mat4x4 &view);
	void RenderSkyBox(const glm::mat4x4 & view, const glm::mat4x4 & proj);
};

#endif // !_RENDER_MANAGER_H_
