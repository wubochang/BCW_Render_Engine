#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include "Shader.h"

#include <unordered_map>
#include <string>

class ShaderManager
{
public:
	ShaderManager();
	~ShaderManager();

	void Initialize();

	Shader* GetShader(std::string name);

private:

	std::unordered_map<std::string, Shader*> m_shaders;
};

extern ShaderManager g_shaderManager;

#endif // !_SHADER_MANAGER_H_
