
#ifndef _SHADER_H_
#define _SHADER_H_

#include "glad_include.h"
#include <glm/glm.hpp>

#include "Texture.h"
#include "CubeMap.h"
#include "Material.h"
#include "Light.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

class Material;
class Light;
class Shader
{
public:
	Shader();
	~Shader();

	bool Initialize(const char* vertexPath, const char* fragmentPath);
	bool Initialize(std::string vertexPath, std::string fragmentPath);
	void Shutdown();

	void Use();


	void SetBool(const std::string &name, bool value) const
	{
		glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), (int)value);
	}
	void SetInt(const std::string &name, int value) const
	{
		glUniform1i(glGetUniformLocation(m_shaderID, name.c_str()), value);
	}
	void SetFloat(const std::string &name, float value) const
	{
		glUniform1f(glGetUniformLocation(m_shaderID, name.c_str()), value);
	}
	void SetVec2(const std::string &name, const glm::vec2 &value) const
	{
		glUniform2fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, &value[0]);
	}
	void SetVec2(const std::string &name, float x, float y) const
	{
		glUniform2f(glGetUniformLocation(m_shaderID, name.c_str()), x, y);
	}
	void SetVec3(const std::string &name, const glm::vec3 &value) const
	{
		glUniform3fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, &value[0]);
	}
	void SetVec3(const std::string &name, float x, float y, float z) const
	{
		glUniform3f(glGetUniformLocation(m_shaderID, name.c_str()), x, y, z);
	}
	void SetVec4(const std::string &name, const glm::vec4 &value) const
	{
		glUniform4fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, &value[0]);
	}
	void SetVec4(const std::string &name, float x, float y, float z, float w)
	{
		glUniform4f(glGetUniformLocation(m_shaderID, name.c_str()), x, y, z, w);
	}
	void SetMat2(const std::string &name, const glm::mat2 &mat) const
	{
		glUniformMatrix2fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void SetMat3(const std::string &name, const glm::mat3 &mat) const
	{
		glUniformMatrix3fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void SetMat4(const std::string &name, const glm::mat4 &mat) const
	{
		glUniformMatrix4fv(glGetUniformLocation(m_shaderID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
	}
	void SetTexture(const std::string& name, Texture* tex, unsigned int slot);
	void SetCubeMap(const std::string& name, CubeMap* cubeMap, unsigned int slot);

	void SetMaterial(Material* mat);
	void SetLight(Light* light);

private:

	int m_shaderID;

	// utility function for checking shader compilation/linking errors.
// ------------------------------------------------------------------------
	bool checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				return false;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
				return false;
			}
		}
		return true;
	}
};

#endif // !_SHADER_H_
