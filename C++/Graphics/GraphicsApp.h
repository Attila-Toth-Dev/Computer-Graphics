#pragma once

#include "Application.h"

#include <glm/mat4x4.hpp>
#include <list>

#include "Planet.h"
#include "Shader.h"
#include "Mesh.h"
#include "OBJMesh.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "ParticleEmitter.h"

#include "SimpleCamera.h"

class BaseCamera;
class Scene;

class GraphicsApp : public aie::Application {
public:
	GraphicsApp();
	virtual ~GraphicsApp();

	virtual bool startup();
	virtual void shutdown();

	virtual void update(float deltaTime);
	virtual void draw();

protected:
	bool LaunchShaders();
	bool LoadShaders(aie::ShaderProgram& _shaderToLoad, const char* _filePath, std::string _errorName);

	bool ObjLoader(aie::OBJMesh& _objMesh, glm::mat4& _transform, const char* _filePath, std::string _fileName = " ", bool _flipTexture = false, float scale = 1.f, glm::vec3 _position = { 0, 0, 0 });

protected:
	// camera transforms
	glm::mat4	m_viewMatrix;
	glm::mat4	m_projectionMatrix;

	aie::RenderTarget m_renderTarget;
	Scene* m_scene;

	SimpleCamera m_simpleCamera;

	ParticleEmitter* m_emitter;
	aie::ShaderProgram m_particleShader;
	glm::mat4 m_particleTransform;
	
	// Shaders
	aie::ShaderProgram m_simpleShader;
	aie::ShaderProgram m_simpleTexture;

	aie::ShaderProgram m_simplePhong;
	aie::ShaderProgram m_classicPhong;
	
	aie::ShaderProgram m_normalMap;

	aie::ShaderProgram m_boundTexture;
	aie::ShaderProgram m_postProcess;

	glm::vec3 m_ambientLight;

	// Texture
	aie::Texture m_gridTexture;

	// Simple Quad
	Mesh m_quadMesh;
	Mesh m_screenQuad;
	glm::mat4 m_quadTransform;

	// Bunny Mesh
	aie::OBJMesh m_bunnyMesh;
	glm::mat4 m_bunnyTransform;

	// Dragon Mesh
	aie::OBJMesh m_dragonMesh;
	glm::mat4 m_dragonTransform;

	// Spear Mesh
	aie::OBJMesh m_spearMesh;
	glm::mat4 m_spearTransform;

};