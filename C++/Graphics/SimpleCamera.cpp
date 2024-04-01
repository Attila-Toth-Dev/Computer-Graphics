#include "SimpleCamera.h"

#include "Input.h"
#include <glm/ext.hpp>

SimpleCamera::SimpleCamera()
{
	m_position = glm::vec3(-10, 2, 0);
	
	m_phi = 0;
	m_theta = 0;
}

void SimpleCamera::Update(float _deltaTime)
{
	aie::Input* input = aie::Input::getInstance();

	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));
	glm::vec3 right(-glm::sin(thetaR), 0, glm::cos(thetaR));
	glm::vec3 up(0, 1, 0);

	// Implemented WASD movement
	if (input->isKeyDown(aie::INPUT_KEY_W))
		m_position += forward * m_moveSpeed * _deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_S))
		m_position -= forward * m_moveSpeed * _deltaTime;

	if (input->isKeyDown(aie::INPUT_KEY_D))
		m_position += right * m_moveSpeed * _deltaTime;
	
	if (input->isKeyDown(aie::INPUT_KEY_A))
		m_position -= right * m_moveSpeed * _deltaTime;

	// Upward and Downward movement
	if (input->isKeyDown(aie::INPUT_KEY_Q))
		m_position += up * m_moveSpeed * _deltaTime;
	
	if (input->isKeyDown(aie::INPUT_KEY_E))
		m_position -= up * m_moveSpeed * _deltaTime;

	// Mouse inputs
	float mouseX = input->getMouseX();
	float mouseY = input->getMouseY();

	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		m_theta += m_turnSpeed * (mouseX - m_lastMousePos.x) * _deltaTime;
		m_phi	+= m_turnSpeed * (mouseY - m_lastMousePos.y) * _deltaTime;
	}

	m_lastMousePos = glm::vec2(mouseX, mouseY);
}

glm::mat4 SimpleCamera::GetViewMatrix()
{
	// Convert to Radians
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	// Forward Direction
	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));

	return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

glm::mat4 SimpleCamera::GetProjectionMatrix(float _width, float _height)
{
	return glm::perspective(glm::pi<float>() * 0.25f, _width / _height, 0.1f, 1000.0f);
}
