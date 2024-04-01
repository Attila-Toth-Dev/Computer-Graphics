#pragma once
#include <glm/glm.hpp>

#include "BaseCamera.h"

class StationaryCamera : public BaseCamera
{
public:
	void SetRotation(glm::vec3 _rotation);
};
