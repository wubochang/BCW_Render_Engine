
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "Transform.h"
#include "Shader/Shader.h"

class Shader;

class Light
{
public:
	Light();
	Light(const Light&);
	~Light();

	void Initialize();

	virtual void SetLight(Shader* shader) = 0;

	Transform* GetTransform() { return m_transform; }

	glm::vec3 ambient;
	glm::vec3 albedo;
	glm::vec3 specular;

protected:

	Transform* m_transform;
};

class DirectionalLight : public Light
{
public:

	void Initialize();

	void SetLight(Shader* shader);

	glm::mat4x4 GetProjectionMatrix();
	glm::mat4x4 GetViewMatrix();
};

class PointLight : public Light
{
public:
	void SetLight(Shader* shader);
	void Initialize();

	float constant;
	float linear;
	float quadratic;

private:
	int m_pointLightIdx;
};

#endif // !_LIGHT_H_
