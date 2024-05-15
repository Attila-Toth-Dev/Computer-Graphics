#include "Instance.h"
#include "Texture.h"
#include "OBJMesh.h"
#include "Scene.h"
#include "SimpleCamera.h"
#include "Shader.h"
#include "Mesh.h"

#include <imgui.h>
#include <glm/ext.hpp>
#include <time.h>

Instance::Instance(glm::mat4 _transform, aie::OBJMesh* _mesh, 
	aie::ShaderProgram* _shader, bool _isUntextured) : 
	m_transform(_transform), m_mesh(_mesh), m_shader(_shader), m_isUntextured(_isUntextured), m_simpleMesh(nullptr)
{
}

Instance::Instance(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale, 
	aie::OBJMesh* _mesh, aie::ShaderProgram* _shader, bool _isUntextured) :
		m_mesh(_mesh), m_shader(_shader), m_isUntextured(_isUntextured)
{
	m_transform = MakeTransform(_position, _eulerAngles, _scale);
}

void Instance::Draw(Scene* _scene)
{
	if (m_simpleMesh == nullptr && m_mesh == nullptr)
		return;

	if (m_mesh != nullptr && !m_isUntextured)
	{
		// This is for a OBJMesh
		OBJDraw(_scene);
		return;			
	}
}

void Instance::OBJDraw(Scene* _scene)
{
	m_shader->bind();

	auto pv = _scene->GetCamera()->GetProjectionMatrix(
		_scene->GetWindowSize().x, _scene->GetWindowSize().y) *
		_scene->GetCamera()->GetViewMatrix();

	m_shader->bindUniform("ProjectionViewModel", pv * m_transform);
	m_shader->bindUniform("ModelMatrix", m_transform);

	m_shader->bindUniform("diffuseTexture", 0);
	m_shader->bindUniform("specularTexture", 0);
	m_shader->bindUniform("normalTexture", 0);

	m_shader->bindUniform("CameraPosition", _scene->GetCamera()->GetPosition());

	m_shader->bindUniform("LightDirection", _scene->GetGlobalLight().direction);
	m_shader->bindUniform("ambientLight", _scene->GetAmbientLightColour());
	m_shader->bindUniform("diffuseLight", _scene->GetAmbientLightColour());

	int numberOfLights = _scene->GetNumberOfLights();
	m_shader->bindUniform("NumberOfLights", numberOfLights);
	m_shader->bindUniform("PointLightPositions", numberOfLights,
		_scene->GetPointLightPositions());
	m_shader->bindUniform("PointLightColours", numberOfLights,
		_scene->GetPointLightColours());

	m_mesh->draw();
}

glm::mat4 Instance::MakeTransform(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale)
{
	return glm::translate(glm::mat4(1), _position)
		* glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.z), glm::vec3(0, 0, 1))
		* glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.y), glm::vec3(0, 1, 0))
		* glm::rotate(glm::mat4(1), glm::radians(_eulerAngles.x), glm::vec3(1, 0, 0))
		* glm::scale(glm::mat4(1), _scale);
}
