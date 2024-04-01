#include "FlyCamera.h"

#include "Input.h"

void FlyCamera::Update(float _dt)
{
	aie::Input* input = aie::Input::getInstance();

	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));
	glm::vec3 right(-glm::sin(thetaR), 0, glm::cos(thetaR));
	glm::vec3 up(0, 1, 0);

	// Implemented WASD movement
	if (input->isKeyDown(aie::INPUT_KEY_W))
		m_position += forward * _dt;

	if (input->isKeyDown(aie::INPUT_KEY_S))
		m_position -= forward * _dt;

	if (input->isKeyDown(aie::INPUT_KEY_D))
		m_position += right * _dt;

	if (input->isKeyDown(aie::INPUT_KEY_A))
		m_position -= right * _dt;

	// Upward and Downward movement
	if (input->isKeyDown(aie::INPUT_KEY_Q))
		m_position += up * _dt;

	if (input->isKeyDown(aie::INPUT_KEY_E))
		m_position -= up * _dt;

	// Mouse inputs
	float mouseX = input->getMouseX();
	float mouseY = input->getMouseY();

	if (input->isMouseButtonDown(aie::INPUT_MOUSE_BUTTON_RIGHT))
	{
		m_theta += glm::radians(m_turnSpeed) * (mouseX - m_lastMousePos.x) * _dt;
		m_phi += glm::radians(m_turnSpeed) * (mouseY - m_lastMousePos.y) * _dt;
	}

	m_lastMousePos = glm::vec2(mouseX, mouseY);
}
