#include "BaseCamera.h"
#include "Input.h"

glm::mat4 BaseCamera::GetWorldTransform(glm::vec3 _cameraPosition, glm::vec3 _eulerAngles, glm::vec3 _scale)
{
	return glm::mat4();
}

glm::mat4 BaseCamera::GetViewMatrix()
{
	// Convert to Radians
	float thetaR = glm::radians(m_theta);
	float phiR = glm::radians(m_phi);

	// Forward Direction
	glm::vec3 forward(glm::cos(phiR) * glm::cos(thetaR), glm::sin(phiR), glm::cos(phiR) * glm::sin(thetaR));

	return glm::lookAt(m_position, m_position + forward, glm::vec3(0, 1, 0));
}

void BaseCamera::SetProjectionMatrix(float _fieldOfView, float _aspectRatio, float _near, float _far)
{
	/*glm::mat4 p =
	{
		1/_aspectRatio * glm::tan(_fieldOfView/2), 0, 0, 0,
		0, 1/glm::tan(_fieldOfView/2), 0, 0,
		0, 0, -((_far + _near)/(_far - _near)), -((2 * _far * _near)/(_far - _near)),
		0, 0, -1, 0
	};*/
	
	m_projectionViewTransform = glm::perspective(_fieldOfView, _aspectRatio, _near, _far);
}

void BaseCamera::SetProjectionMatrix(float _fieldOfView, float _width, float _height, float _near, float _far)
{
	/*float aspectRatio = _width / _height;

	glm::mat4 p =
	{
		1 / aspectRatio * glm::tan(_fieldOfView / 2), 0, 0, 0,
		0, 1 / glm::tan(_fieldOfView / 2), 0, 0,
		0, 0, -((_far + _near) / (_far - _near)), -((2 * _far * _near) / (_far - _near)),
		0, 0, -1, 0
	};*/

	SetProjectionMatrix(_fieldOfView, _width / _height, _near, _far);
}
