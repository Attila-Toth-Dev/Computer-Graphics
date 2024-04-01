#pragma once
#include <glm/glm.hpp>

#include "BaseCamera.h"

class OrbitalCamera
{
public:
	void SetTarget(glm::mat4 _transform, float _distanceToTarget);

private:
	float m_distanceToTarget;
	float m_orbitalSpeed;

};

