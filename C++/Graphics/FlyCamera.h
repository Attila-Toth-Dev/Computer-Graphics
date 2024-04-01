#pragma once
#include <glm/glm.hpp>

#include "BaseCamera.h"

class FlyCamera : public BaseCamera
{
public:
	virtual void Update(float _dt);

	void SetSpeed(float _speed) { m_speed = _speed; }

private:
	float m_speed;
	float m_turnSpeed = 180.f;

	glm::vec3 m_up;
};

