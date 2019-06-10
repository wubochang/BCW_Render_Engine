#include "Light.h"

Light::Light()
{
}

Light::Light(const Light &)
{
}

Light::~Light()
{
}

void Light::Initialize()
{
	m_transform = new Transform();
	m_transform->SetPosition(glm::vec3(0, 3, 0));
	ambient = glm::vec3(0.05f, 0.05f, 0.05f);
	albedo = glm::vec3(0.8f, 0.8f, 0.8f);
	specular = glm::vec3(1.0f, 1.0f, 1.0f);
}

void PointLight::SetLight(Shader * shader)
{

#define PROPERTY_NAME(name) "pointLights[" + std::to_string(m_pointLightIdx) + "]."##name

	shader->SetVec3(PROPERTY_NAME("ambient"), ambient);
	shader->SetVec3(PROPERTY_NAME("diffuse"), albedo);
	shader->SetVec3(PROPERTY_NAME("specular"),specular);
	shader->SetVec3(PROPERTY_NAME("position"), GetTransform()->GetPosition());

	shader->SetFloat(PROPERTY_NAME("constant"), constant);
	shader->SetFloat(PROPERTY_NAME("linear"), linear);
	shader->SetFloat(PROPERTY_NAME("quadratic"), quadratic);
}

void PointLight::Initialize()
{
	static int s_pointLightCnt = 0;


	Light::Initialize(); 
	 
	m_transform->SetScale(glm::vec3(0.1f, 0.1f, 0.1f));

	m_pointLightIdx = s_pointLightCnt;
	s_pointLightCnt += 1;
	constant = 0.1f;
	linear = 0.09f;
	quadratic = 0.032f;
}

void DirectionalLight::Initialize()
{
	Light::Initialize(); 
	m_transform->SetScale(glm::vec3(0.1f, 0.1f, 0.5f));
}

void DirectionalLight::SetLight(Shader * shader)
{
	shader->SetVec3("dirLight.direction", GetTransform()->GetForward());
	shader->SetVec3("dirLight.ambient", ambient);
	shader->SetVec3("dirLight.diffuse", albedo);
	shader->SetVec3("dirLight.specular", specular);
}

glm::mat4x4 DirectionalLight::GetProjectionMatrix()
{
	float near_plane = 1.0f, far_plane = 7.5f;
	float size = 10.0f;
	glm::mat4x4 lightProjection = glm::ortho(-size, size, -size, size, near_plane, far_plane);
	return lightProjection;
}

glm::mat4x4 DirectionalLight::GetViewMatrix()
{
	glm::mat4x4 lightView = glm::lookAt(GetTransform()->GetPosition(), GetTransform()->GetForward(), GetTransform()->GetUp());
	return lightView;
}
