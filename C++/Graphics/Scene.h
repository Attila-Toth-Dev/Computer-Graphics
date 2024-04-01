#pragma once
#include <glm/glm.hpp>
#include <list>
#include <vector>
#include <map>
#include <string>

class SimpleCamera;
class BaseCamera;
class Instance;
class OBJMesh;

const int MAX_LIGHTS = 4;
const int MAX_INSTANCE_COUNT = 100;

struct Light
{
	Light()
	{
		direction = glm::vec3(0);
		colour = glm::vec3(1);
		intensity = 1.f;
	}

	Light(glm::vec3 _position, glm::vec3 _colour, float _intensity = 1)
	{
		direction = _position;
		colour = _colour;
		intensity = _intensity;
	}

	glm::vec3 direction;
	glm::vec3 colour;
	float intensity;

	glm::vec3 GetColour() { return colour * intensity; }

	void SetDirection(glm::vec3 _direction) { direction = _direction; }
	void SetColour(glm::vec3 _colour) { colour = _colour; }
	void SetIntensity(float _intensity) { intensity = _intensity; }
};

class Scene
{
public:
	Scene(SimpleCamera* _camera, glm::vec2 _windowSize, Light& _globalLight);
	~Scene();
	
	// Main Functions
	void Update(float _dt);

	void Draw();

	void ImGui_Functions();
	void ImGui_DebugFunctions();

	void AddPointLights(Light _light)		{ m_pointLights.push_back(_light); }

	void AddInstance(Instance* _instance)	{ m_instances.push_back(_instance); }

	#pragma region Getters & Setters
	// Getters
	SimpleCamera* GetCamera() { return m_mainCamera; }
	
	glm::vec2 GetWindowSize() { return m_windowSize; }
	void SetWindowSize(glm::vec2 _windowSize) { m_windowSize = _windowSize; }

	Light& GetGlobalLight() { return m_globalLight; }
	glm::vec3 GetAmbientLightColour() { return m_globalLight.GetColour(); }
	
	std::vector<Light>& GetPointLights() { return m_pointLights; }
	glm::vec3* GetPointLightPositions() { return &m_pointLightPositions[0]; }
	glm::vec3* GetPointLightColours() { return &m_pointLightColours[0]; }
	int GetNumberOfLights() { return m_pointLights.size(); }

	int GetPostProcessingValue() { return m_postProcessEffect; }

	std::list<Instance*> GetInstances() { return m_instances; }

	std::map<std::string, glm::vec4> GetConsoleLog() { return m_consoleLogs; }

	// Setters
	void SetAmbientLightColour(glm::vec3 _colour) { m_globalLight.GetColour() = _colour; }

#pragma endregion

private:
	void RemoveIterInstance(Instance* _instance);
	Instance* ListIterGetter(std::list<Instance*> _list, int _i);

protected:
	SimpleCamera*				m_mainCamera;
	Light						m_globalLight;
	glm::vec2					m_windowSize;

	std::list<Instance*>		m_instances;

	std::list<OBJMesh*>			m_meshTypes;

	//std::vector<std::string>	m_consoleLogs;
	std::map<std::string, glm::vec4> m_consoleLogs;
	std::vector<Light>			m_pointLights;
	glm::vec3					m_pointLightPositions[MAX_LIGHTS];
	glm::vec3					m_pointLightColours[MAX_LIGHTS];
	
	glm::vec4					m_clearColour{ 0.2f, 0.2f, 0.2f, 1 };

	int							m_postProcessEffect = 0;

	bool						m_enableLights;
};

