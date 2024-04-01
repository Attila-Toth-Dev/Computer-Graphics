#pragma once
#include <glm/glm.hpp>

class SimpleCamera
{
public:
	SimpleCamera();
	~SimpleCamera() {};

	void Update(float _deltaTime);

	glm::vec3 GetPosition() { return m_position; }

	glm::mat4 GetViewMatrix();
	glm::mat4 GetProjectionMatrix(float _width, float _height);

	//glm::mat4 GetTransform(glm::vec3 _position, glm::vec3 _eulerAngles, glm::vec3 _scale);

	float GetSpeed() { return m_moveSpeed; }
	float GetTurnSpeed() { return m_turnSpeed; }

	void SetSpeed(float _moveSpeed) { m_moveSpeed = _moveSpeed; }
	void SetTurnSpeed(float _turnSpeed) { m_turnSpeed = _turnSpeed; }

private:
	float m_turnSpeed = glm::radians(180.0f);
	float m_moveSpeed = 5.f;

	float m_theta;	// In degrees
	float m_phi;	// In degrees


	glm::vec3 m_position;
	glm::vec2 m_lastMousePos;
};
