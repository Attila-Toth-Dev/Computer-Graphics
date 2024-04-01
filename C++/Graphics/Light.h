#pragma once
#include "glm/glm.hpp"

class Light
{
public:
	Light();
	Light(glm::vec3 _position, glm::vec3 _colour, float _intensity = 1);

	~Light() {}

	// Getters
	glm::vec3 GetColour()		{ return m_colour * m_intensity; }
	glm::vec3 GetDirection()	{ return m_direction; }
	float GetIntensity()		{ return m_intensity; }

	// Setters
	void SetColour(glm::vec3 _colour)		{ m_colour = _colour; }
	void SetDirection(glm::vec3 _direction) { m_direction = _direction; }
	void SetIntensity(float _intensity)		{ m_intensity = _intensity; }

protected:
	glm::vec3 m_direction;
	glm::vec3 m_colour;
	float m_intensity;

};

