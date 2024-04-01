#pragma once
#include <glm/glm.hpp>

struct Particle
{
	glm::vec3 position;
	glm::vec3 velocity;
	glm::vec4 colour;
	float size;
	float lifeTime;
	float lifeSpan;
};

struct ParticleVertex
{
	glm::vec4 position;
	glm::vec4 colour;
};

class ParticleEmitter
{
public:
	ParticleEmitter();
	virtual ~ParticleEmitter();

	void Initialise(unsigned int _maxParticles, unsigned int _emitRate, float _lifetimeMin, float _lifetimeMax,
		float _velocityMin, float _velocityMax, float _startSize, float _endSize, const glm::vec4& _startColour, const glm::vec4 _endColour);

	void Emit();
	void Update(float _deltaTime, const glm::mat4& _cameraTransform);
	void Draw();

	// more to come here!

protected:
	Particle*			m_particles;
	unsigned int		m_firstDead;
	unsigned int		m_maxParticles;

	unsigned int		m_vao, m_vbo, m_ibo;
	ParticleVertex*		m_vertexData;

	glm::vec3			m_position;

	float				m_emitTimer;
	float				m_emitRate;

	float				m_lifespanMin;
	float				m_lifespanMax;

	float				m_velocityMin;
	float				m_velocityMax;

	float				m_startSize;
	float				m_endSize;

	glm::vec4			m_startColour;
	glm::vec4			m_endColour;

	// more to come here!

};

