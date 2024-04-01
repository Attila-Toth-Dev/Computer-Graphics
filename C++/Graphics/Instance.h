#pragma once
#include <glm/glm.hpp>

class SimpleCamera;
class BaseCamera;
class Mesh;

class Scene;

struct Light;

namespace aie
{
	class OBJMesh;
	class ShaderProgram;
}

class Instance
{
public:
	Instance(glm::mat4 _transform, aie::OBJMesh* _mesh,
		aie::ShaderProgram* _shader, bool _isUntextured = false);
	Instance(glm::mat4 _transform, Mesh* _mesh,
		aie::ShaderProgram* _shader, bool _isUntextured = false);
	Instance(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale,
		aie::OBJMesh* _mesh, aie::ShaderProgram* _shader, bool _isUntextured = false);
	~Instance() {}

	void Draw(Scene* _scene);

	static glm::mat4 MakeTransform(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale);

	glm::mat4 GetTransform() { return m_transform; }
	void SetTransform(glm::mat4 _transform) { m_transform = _transform; }

	bool GetEnabled() { return m_isEnabled; }
	void SetEnabled() { m_isEnabled = !m_isEnabled; }

protected:
	glm::mat4 m_transform;
	aie::OBJMesh* m_mesh;
	Mesh* m_simpleMesh;
	aie::ShaderProgram* m_shader;

	bool m_isUntextured;
	bool m_isEnabled;
};

