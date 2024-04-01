#include "Planet.h"

#include "Gizmos.h"

#include <glm/ext.hpp>

Planet::Planet(glm::vec3 _center, float _radius, float _row, float _column, glm::vec4 _colour, bool _hasRings, bool _hasMoon)
	: m_center(_center), m_radius(_radius), m_row(_row), m_column(_column), m_colour(_colour), m_hasRings(_hasRings), m_hasMoon(_hasMoon)
{
	m_matrix = glm::mat4(1);

	m_moon = nullptr;
}

Planet::~Planet() {}

void Planet::Update(float _deltaTime)
{
	m_matrix = glm::rotate(m_matrix, _deltaTime, m_center);
}

void Planet::Draw()
{
	aie::Gizmos::addSphere(m_center, m_radius, m_row, m_column, m_colour);

	if (m_hasMoon)
		DrawMoon(glm::vec3(m_center.x, m_center.y, m_center.z + m_radius + 0.5f), glm::vec4(1, 1, 1, 1));

	if (m_hasRings)
		DrawRing(m_center, m_radius + .1f, m_radius + .5f, 20, m_colour);
}

void Planet::DrawRing(glm::vec3 _center, float _innerRadius, float _outerRadius, float _segments, glm::vec4 _colour)
{
	aie::Gizmos::addRing(_center, _innerRadius, _outerRadius, _segments, _colour);
}

void Planet::DrawMoon(glm::vec3 _center, glm::vec4 _colour)
{
	m_moon = new Planet(_center, 0.1f, 16, 16, glm::vec4(1, 1, 1, 1), false, false);
	m_moon->Draw();
}
