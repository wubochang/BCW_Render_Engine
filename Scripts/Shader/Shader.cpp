#include "Shader.h"

Shader::Shader()
{
}

Shader::~Shader()
{
}

bool Shader::Initialize(const char* vertexPath, const char* fragmentPath)
{
	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		return false;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char * fShaderCode = fragmentCode.c_str();
	// 2. compile shaders

	bool shaderCompiled = true;

	unsigned int vertex, fragment;

	// vertex shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	if (!checkCompileErrors(vertex, "VERTEX"))
	{
		std::cout << "ERROR::VERTEX SHADER NOT COMPILED \n" + std::string(vertexPath) << std::endl;	
		shaderCompiled = false;
	}

	// fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	if(!checkCompileErrors(fragment, "FRAGMENT"))
	{
		std::cout << "ERROR::FRAGMENT SHADER NOT COMPILED \n" + std::string(fragmentPath) << std::endl;	
		shaderCompiled = false;
	}

	// shader Program
	m_shaderID = glCreateProgram();
	glAttachShader(m_shaderID, vertex);
	glAttachShader(m_shaderID, fragment);
	glLinkProgram(m_shaderID);
	if (!checkCompileErrors(m_shaderID, "PROGRAM"))
	{
		std::cout << "ERROR::SHADER NOT COMPILED" << std::endl;
		shaderCompiled = false;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return shaderCompiled;
}

bool Shader::Initialize(std::string vertexPath, std::string fragmentPath)
{
	return Initialize(vertexPath.c_str(), fragmentPath.c_str());
}

void Shader::Shutdown()
{
}

void Shader::Use()
{
	glUseProgram(m_shaderID);
}

void Shader::SetMaterial(Material * mat)
{
	SetTexture("material.albedo", mat->albedo, 0);
	SetTexture("material.normal", mat->normal, 1);
	SetTexture("material.metallic", mat->metallic, 2);
	SetTexture("material.roughness", mat->roughness, 3);
	SetTexture("material.AO", mat->AO, 4);
	SetTexture("material.height", mat->height, 5);
	SetBool("material.needPOM", mat->needPOM);
	SetFloat("material.heightScale", mat->heightScale);
}

void Shader::SetLight(Light * light)
{
	light->SetLight(this);
}

void Shader::SetTexture(const std::string & name, Texture* tex, unsigned int slot)
{
	SetInt(name, slot);
	tex->SetBuffers(slot);
}

void Shader::SetCubeMap(const std::string & name, CubeMap * cubeMap, unsigned int slot)
{
	SetInt(name, slot);
	cubeMap->SetBuffers(slot);
}
