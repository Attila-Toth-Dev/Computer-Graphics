#include "GraphicsApp.h"
#include "Gizmos.h"
#include "Input.h"

#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <imgui.h>

#include "Scene.h"
#include "Instance.h"
#include "ParticleEmitter.h"

using glm::vec3;
using glm::vec4;
using glm::mat4;
using aie::Gizmos;

GraphicsApp::GraphicsApp() 
{
}

GraphicsApp::~GraphicsApp() 
{
}

bool GraphicsApp::startup() 
{
	setBackgroundColour(0.25f, 0.25f, 0.25f);

	// initialise gizmo primitive counts
	Gizmos::create(10000, 10000, 10000, 10000);

	// create simple camera transforms
	m_viewMatrix = glm::lookAt(vec3(10), vec3(0), vec3(0, 1, 0));
	m_projectionMatrix = glm::perspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);

	#pragma region Scene Setup
	// Ambient Light
	Light light;
	light.direction = { 1, -1, 1 };
	light.colour = { 1, 1, 1 };

	// This will create one scene to use
	m_scene = new Scene(&m_simpleCamera, glm::vec2(getWindowWidth(), getWindowHeight()), light);
	
	m_scene->GetPointLights().push_back(Light(glm::vec3(5, 3, 0), glm::vec3(1, 0, 0), 10));

	#pragma endregion

	/*m_emitter = new ParticleEmitter();
	m_emitter->Initialise(1000, 500, 0.1f, 0.1f, 1, 5, 1, 0.1f, glm::vec4(1, 0, 0, 1), glm::vec4(1, 1, 0, 1));*/

	return LaunchShaders();
}

void GraphicsApp::shutdown() 
{
	Gizmos::destroy();

	delete m_scene;
}

void GraphicsApp::update(float deltaTime) 
{
	if (m_scene->GetWindowSize().x != (float)getWindowWidth())
		m_scene->SetWindowSize(glm::vec2((float)getWindowWidth(), (float)getWindowHeight()));

	// wipe the gizmos clean for this frame
	Gizmos::clear();

	// draw a simple grid with gizmos
	vec4 white(1);
	vec4 black(0, 0, 0, 1);
	for (int i = 0; i < 21; ++i) {
		Gizmos::addLine(vec3(-10 + i, 0, 10),
						vec3(-10 + i, 0, -10),
						i == 10 ? white : black);
		Gizmos::addLine(vec3(10, 0, -10 + i),
						vec3(-10, 0, -10 + i),
						i == 10 ? white : black);
	}

	// add a transform so that we can see the axis
	Gizmos::addTransform(mat4(1));
	
	m_simpleCamera.Update(deltaTime);

	// Returns time since initialisation in seconds
	float time = getTime();
	
	// quit if we press escape
	aie::Input* input = aie::Input::getInstance();

	if (input->isKeyDown(aie::INPUT_KEY_ESCAPE))
		quit();

	//m_emitter->Update(deltaTime, m_simpleCamera.GetTransform());

	m_scene->Update(deltaTime);
}

void GraphicsApp::draw() 
{
	// Bind to the render target
	m_renderTarget.bind();

	// wipe the screen to the background colour
	clearScreen();

	m_scene->Draw();

	// update perspective based on screen size
	m_projectionMatrix = m_simpleCamera.GetProjectionMatrix(m_scene->GetWindowSize().x, m_scene->GetWindowSize().y);
	m_viewMatrix = m_simpleCamera.GetViewMatrix();

	auto pv = m_projectionMatrix * m_viewMatrix;

	Gizmos::draw(pv);

	// Unbind the target from the backbuffer
	m_renderTarget.unbind();

	// Clear the back buffer
	clearScreen();

	m_postProcess.bind();
	m_postProcess.bindUniform("colourTarget", 0);
	m_postProcess.bindUniform("postProcessTarget", m_scene->GetPostProcessingValue());

	m_postProcess.bindUniform("iTime", getTime());

	m_renderTarget.getTarget(0).bind(0);

	//// bind particle shader
	//m_particleShader.bind();

	//// bind particle transform
	//auto pvm = m_projectionMatrix * m_viewMatrix * m_particleTransform;
	//m_particleShader.bindUniform("ProjectionViewModel", pvm);

	//m_emitter->Draw();

	m_screenQuad.Draw();
}

bool GraphicsApp::LaunchShaders()
{
	if (!LoadShaders(m_simpleShader, "./shaders/color.", "Color Shader"))
		return false;

	if (!LoadShaders(m_boundTexture, "./shaders/textured.", "Textured Shader"))
		return false;

	if (!LoadShaders(m_classicPhong, "./shaders/classicPhong.", "Simple Phong Shader"))
		return false;

	if (!LoadShaders(m_normalMap, "./shaders/normalMap.", "Simple Phong Shader"))
		return false;

	if (!LoadShaders(m_postProcess, "./shaders/post.", "Post Process"))
		return false;

	if (m_renderTarget.initialise(1, getWindowWidth(), getWindowHeight()) == false)
	{
		printf("Render Target has an Error!!\n");
		return false;
	}

	#pragma region Quad Data
	m_quadMesh.InitialiseQuad();
	m_screenQuad.InitialiseFullscreenQuad();

	m_quadTransform =
	{
		 10,  0,  0,  0,
		 0,	 10,  0,  0,
		 0,  0,  10,  0,
		 0,  0,  0,  1
	};

	m_gridTexture.load("./textures/numbered_grid.tga");

	#pragma endregion

	// Spear Loader
	ObjLoader(m_spearMesh, m_spearTransform, "./soulspear/soulspear.obj", "Soulspear", true, 2);
	
	// Bunny Loader
	ObjLoader(m_bunnyMesh, m_bunnyTransform, "./stanford/bunny.obj", "Stanford", false, 0.2f);
	
	// Dragon Loader
	ObjLoader(m_dragonMesh, m_dragonTransform, "./stanford/dragon.obj", "Stanford", false, 0.2f, {1.f, 0, 1.f});
	
	// Robo Loader
	ObjLoader(m_roboMesh, m_roboTransform, "./robo/robo.obj", "robo", false, .5f);

	return true;
}

bool GraphicsApp::LoadShaders(aie::ShaderProgram& _shaderToLoad, const char* _filePath, std::string _errorName)
{
	std::string vert = _filePath;
	std::string frag = _filePath;

	_shaderToLoad.loadShader(aie::eShaderStage::VERTEX, vert.append("vert").c_str());
	_shaderToLoad.loadShader(aie::eShaderStage::FRAGMENT, frag.append("frag").c_str());

	if (_shaderToLoad.link() == false)
	{
		printf(_errorName.append(" Shader Error: %s\n").c_str(), _shaderToLoad.getLastError());
		return false;
	}

	return true;
}

bool GraphicsApp::ObjLoader(aie::OBJMesh& _objMesh, glm::mat4& _transform, const char* _filePath, std::string _fileName, bool _flipTexture, float _scale, glm::vec3 _position)
{
	if (_objMesh.load(_filePath, true, _flipTexture) == false)
	{
		printf(_fileName.append(" Mesh Error! \n").c_str());
		return false;
	}
	
	_transform =
	{
		_scale,		 0,			  0,			0,
		0,			 _scale,	  0,			0,
		0,			 0,			  _scale,		0,
		_position.x, _position.y, _position.z,  1
	};

	return true;
}
