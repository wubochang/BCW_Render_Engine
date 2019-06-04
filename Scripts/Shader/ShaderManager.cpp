#include "ShaderManager.h"

ShaderManager g_shaderManager;

ShaderManager::ShaderManager()
{
}

ShaderManager::~ShaderManager()
{
}

void ShaderManager::Initialize()
{
	m_shaders = std::unordered_map<std::string, Shader*>();
}

Shader* ShaderManager::GetShader(std::string name)
{

	if (m_shaders.find(name) == m_shaders.end())
	{
		Shader* newShader = new Shader();
		if (!newShader->Initialize("../Shaders/" + name + ".vs", "../Shaders/" + name + ".fs"))
		{
			std::cout << "Cannot load shader " + name << std::endl;
		}
		m_shaders[name] = newShader;
	}
	return m_shaders[name];
}
