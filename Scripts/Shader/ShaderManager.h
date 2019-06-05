#ifndef _SHADER_MANAGER_H_
#define _SHADER_MANAGER_H_

#include "Shader.h"
#include "Misc.h"

#include <unordered_map>
#include <string>

class ShaderManager
{
	MAKE_SINGLETON(ShaderManager)

public:

	~ShaderManager() {};

	void Initialize();

	Shader* GetShader(std::string name);

private:

	std::unordered_map<std::string, Shader*> m_shaders;
};

#endif // !_SHADER_MANAGER_H_
