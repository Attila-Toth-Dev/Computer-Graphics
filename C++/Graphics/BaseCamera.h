#pragma once
#include <glm/glm.hpp>
#include <glm/ext.hpp>

class BaseCamera
{
public:
	virtual void Update(float _dt) {};

	// Getter Functions
	glm::vec3 GetPosition() { return m_position; }

	glm::mat4 GetWorldTransform(glm::vec3 _cameraPosition, glm::vec3 _eulerAngles, glm::vec3 _scale);
	
	glm::mat4 GetProjectionViewMatrix() { return GetProjectionMatrix() * GetViewMatrix(); }
	glm::mat4 GetProjectionMatrix() { return m_projectionViewTransform; }

	glm::mat4 GetViewMatrix();
	
	float GetAspectRatio() { return m_aspectRatio; }

	// Setter Functions
	void SetPosition(glm::vec3 _position) { m_position = _position; }
	void SetAspectRatio(float _width, float _height) { m_aspectRatio = _width / _height; }
	void SetViewMatrix(glm::vec3 _from, glm::vec3 _to, glm::vec3 _up) { m_viewTransform = glm::lookAt(_from, _to, _up); }

	void SetProjectionMatrix(float _fieldOfView, float _aspectRatio, float _near, float _far);
	void SetProjectionMatrix(float _fieldOfView, float _width, float _height, float _near, float _far);

protected:
	glm::mat4 m_projectionViewTransform;
	glm::mat4 m_worldTransform;
	glm::mat4 m_viewTransform;

	glm::vec3 m_position;
	glm::vec2 m_lastMousePos;

	float m_aspectRatio;
	float m_theta;
	float m_phi;

};

