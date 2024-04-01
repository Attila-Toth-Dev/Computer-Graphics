#pragma once
#include <vector>

#include "glm/glm.hpp"

class AABB
{
public:
	AABB() { Reset(); }
	~AABB() {}

	void Reset()
	{
		m_min.x = m_min.y = m_min.z = 1e37f;
		m_max.x = m_max.y = m_max.z = -1e37f;
	}

	void Fit(const std::vector<glm::vec3>& _points)
	{
		for (auto& p : _points)
		{
			if (p.x < m_min.x) m_min.x = p.x;
			if (p.y < m_min.y) m_min.y = p.y;
			if (p.x < m_min.z) m_min.z = p.z;
			if (p.x > m_max.x) m_max.x = p.x;
			if (p.y > m_max.y) m_max.y = p.y;
			if (p.z > m_max.z) m_max.z = p.z;
		}
	}

public:
	glm::vec3 m_min;
	glm::vec3 m_max;
};

