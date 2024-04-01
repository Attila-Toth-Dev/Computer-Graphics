#include "Light.h"

Light::Light()
{
	m_direction = glm::vec3(0);
	m_colour = glm::vec3(1);
	m_intensity = 1.f;
}

Light::Light(glm::vec3 _position, glm::vec3 _colour, float _intensity)
	: m_direction(_position), m_colour(_colour), m_intensity(_intensity)
{
}
