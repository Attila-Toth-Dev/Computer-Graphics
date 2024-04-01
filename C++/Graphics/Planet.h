#pragma once

#include "glm/glm.hpp"

class Planet
{
public:
	Planet(glm::vec3 _center, float _radius, float _row, float _column, glm::vec4 _colour, bool _hasRings, bool _hasMoon);
	~Planet();

	void Update(float _deltaTime);
	void Draw();

	void DrawRing(glm::vec3 _center, float _innerRadius, float _outerRadius, float _segments, glm::vec4 _colour);
	void DrawMoon(glm::vec3 _center, glm::vec4 _colour);

protected:
	glm::vec3 m_center;
	glm::vec4 m_colour;
	
	float m_radius;
	float m_row;
	float m_column;

	bool m_hasRings;
	bool m_hasMoon;

	Planet* m_moon;

	glm::mat4 m_matrix;

};

