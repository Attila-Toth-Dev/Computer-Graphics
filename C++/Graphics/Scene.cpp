#include "Scene.h"

#include <string>
#include <iostream>

#include "SimpleCamera.h"
#include "Instance.h"
#include "Gizmos.h"

#include "gl_core_4_4.h"
#include "imgui.h"
#include "glm/ext.hpp"

Scene::Scene(SimpleCamera* _camera, glm::vec2 _windowSize, Light& _globalLight)
	: m_mainCamera(_camera), m_windowSize(_windowSize), m_globalLight(_globalLight) 
{
	m_enableLights = false;
}

Scene::~Scene() 
{
	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
		delete* it;
}

void Scene::Update(float _dt)
{
	m_mainCamera->Update(_dt);
	
	// The last function of this loop
	ImGui_Functions();
}

void Scene::Draw()
{
	for (int i = 0; i < MAX_LIGHTS && i < GetNumberOfLights(); i++)
	{
		m_pointLightPositions[i] = m_pointLights[i].direction;
		m_pointLightColours[i] = m_pointLights[i].colour;
	}

	for (auto it = m_instances.begin(); it != m_instances.end(); it++)
	{
		Instance* instance = *it;
		instance->Draw(this);
	}
}

void Scene::ImGui_Functions()
{
	// -- Scene Settings --
	#pragma region Scene Settings

	ImGui::Begin("Scene Settings");
	
	ImGui::Text("Camera Settings");
	float move = m_mainCamera->GetSpeed();
	if (ImGui::DragFloat("Move Speed", &move, 0.05f, 0.f, 100.f))
		m_mainCamera->SetSpeed(move);
	
	float turn = m_mainCamera->GetTurnSpeed();
	if (ImGui::DragFloat("Turn Speed", &turn, 0.05f, 0.f, 100.f))
		m_mainCamera->SetTurnSpeed(turn);

	ImGui::Spacing();

	ImGui::Text("Background");
	glClearColor(m_clearColour.r, m_clearColour.g, m_clearColour.b, 1);
	ImGui::ColorEdit3("Clear Colour", glm::value_ptr(m_clearColour));

	ImGui::Spacing();

	ImGui::Text("Post Processing Effects");
	ImGui::DragInt("Effect", &m_postProcessEffect, 1.f, 0, 10);

	ImGui::End();

#pragma endregion

	// -- Lighting Settings --
	#pragma region Lighting Settings

	ImGui::Begin("Lighting Settings");

	ImGui::Text("Global Lighting");
	glm::vec3 lightDirection = GetGlobalLight().direction;
	if (ImGui::DragFloat3("Global Direction", &lightDirection[0], 0.05f, -1.f, 1.f))
		GetGlobalLight().SetDirection(lightDirection);

	glm::vec3 colour = GetGlobalLight().colour;
	if (ImGui::ColorEdit3("Global Colour", &colour[0]))
		GetGlobalLight().SetColour(colour);

	ImGui::Spacing();

	ImGui::Text("Lights");
	if (ImGui::Button("Add Light"))
	{
		m_consoleLogs["ADDED LIGHT"] = glm::vec4(0, 1, 0, 1);
		m_pointLights.push_back(Light(glm::vec3(0), glm::vec3(0), 1));
	}
	ImGui::SameLine();
	if (ImGui::Button("Remove Light"))
	{
		if (!m_pointLights.empty())
		{
			m_consoleLogs["REMOVED LIGHT"] = glm::vec4(0, 1, 0, 1);
			m_pointLights.pop_back();
		}
		else
			m_consoleLogs["ERROR, LIGHT LIST IS EMPTY"] = glm::vec4(1, 0, 0, 1);
	}

	for (int i = 0; i < MAX_LIGHTS && i < GetNumberOfLights(); i++)
	{
		ImGui::Spacing();

		int idNumber = i + 1;
		std::string id = std::to_string(idNumber);
		std::string pointLightName = std::string("Light ") += id;

		std::string positionString = std::string("Position ") += id;
		std::string colourString = std::string("Colour ") += id;
		std::string intensityString = std::string("Intensity ") += id;

		ImGui::Text(pointLightName.c_str());

		glm::vec3 direction = GetPointLights()[i].direction;
		glm::vec3 colour = GetPointLights()[i].colour;
		float intensity = GetPointLights()[i].intensity;

		if (ImGui::DragFloat3(positionString.c_str(), &direction[0], 0.05f, -100.f, 100.f))
			m_pointLights[i].SetDirection(direction);

		if (ImGui::ColorEdit3(colourString.c_str(), &colour[0]))
			m_pointLights[i].SetColour(colour);

		if (ImGui::DragFloat(intensityString.c_str(), &intensity, 0.05f, 0.f, 100.f))
			m_pointLights[i].SetIntensity(intensity);
		
		glm::vec3 colV3 = GetPointLights()[i].GetColour();
		glm::vec4 colV4(colV3.x, colV3.y, colV3.z, 0.5f);

		aie::Gizmos::addSphere(GetPointLights()[i].direction, GetPointLights()[i].intensity / 25, 10, 10, colV4);
	}

	ImGui::End();

#pragma endregion

	// -- Instance Settings --
	#pragma region Instance Settings
	
	ImGui::Begin("Hierarchy");

	if (ImGui::Button("Add"))
	{
		m_consoleLogs["ADDED INSTANCE"] = glm::vec4(0, 1, 0, 1);
	}

	ImGui::SameLine();
	
	if (ImGui::Button("Remove"))
	{
		if (!m_instances.empty())
		{
			m_instances.pop_back();
			m_consoleLogs["REMOVED INSTANCE"] = glm::vec4(0, 1, 0, 1);
		}
		else
			m_consoleLogs["ERROR REMOVING INSTANCE, LIST IS EMPTY"] = glm::vec4(1, 0, 0, 1);
	}

	for (int i = 0; i < MAX_INSTANCE_COUNT && i < m_instances.size(); i++)
	{
		ImGui::Spacing();

		int idNumber = i + 1;
		std::string id = std::to_string(idNumber);
		std::string instanceName = std::string("Object ") += id;

		std::string positionString = std::string("Position ") += id;
		std::string rotationString = std::string("Rotation ") += id;
		std::string scaleString = std::string("Scale ") += id;

		ImGui::Text(instanceName.c_str());

		Instance* ins = ListIterGetter(m_instances, i);

		glm::vec3 position = glm::vec3(ins->GetTransform()[3]);
		if (ImGui::DragFloat3(positionString.c_str(), &position.x, 0.05f))
		{
			glm::vec3 scale;
			scale.x = glm::length(glm::vec3(ins->GetTransform()[0]));
			scale.y = glm::length(glm::vec3(ins->GetTransform()[1]));
			scale.z = glm::length(glm::vec3(ins->GetTransform()[2]));

			glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.f), scale);
			glm::mat4 posMatrix = glm::translate(glm::mat4(1.f), position);

			glm::mat4 newTransform = posMatrix * scaleMatrix;
			ins->SetTransform(newTransform);
		}
	}

	ImGui::End();

#pragma endregion

	ImGui_DebugFunctions();
}

void Scene::ImGui_DebugFunctions()
{
	ImGui::Begin("Console Logs");
	
	if (ImGui::BeginMenu("Options"))
	{
		if (ImGui::MenuItem("Clear Log"))
			m_consoleLogs.clear();
		
		ImGui::EndMenu();
	}

	for (auto i : m_consoleLogs)
	{
		ImVec4 colour = { i.second.r, i.second.g, i.second.b, i.second.a };
		ImGui::TextColored(colour, i.first.c_str());
	}

	ImGui::End();
}

void Scene::RemoveIterInstance(Instance* _instance)
{
	std::list<Instance*>::iterator it = m_instances.begin();
	for (it; it != m_instances.end(); ++it)
	{
		if (*it == _instance)
			m_instances.erase(it);
	}
}

Instance* Scene::ListIterGetter(std::list<Instance*> _list, int _i)
{
	std::list<Instance*>::iterator it = _list.begin();
	for (int i = 0; i < _i; i++)
		++it;

	return *it;
}
