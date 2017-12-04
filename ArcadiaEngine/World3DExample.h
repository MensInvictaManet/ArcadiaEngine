#pragma once

#include <iostream>

#include "Engine/GUIObjectNode.h"
#include "Engine/InputManager.h"
#include "Engine/TimeSlice.h"
#include "Engine/BasicPrimativeCube.h"
#include "Engine/BasicPrimativeIcohedron.h"
#include "Engine/SplittableIcohedron.h"
#include "Engine/SplittableCube.h"

#include "Engine/GLMCamera.h"
#include "Engine/Shader.h"
#include "Engine/Program.h"

class World3DExample : public GUIObjectNode
{
public:
	World3DExample();
	~World3DExample();

	void Input(int xOffset = 0, int yOffset = 0) override;
	void Update() override;
	void Render3D() override;

private:
	void TakeCameraInput();

	Camera m_Camera;

	BasicPrimativeIcohedron m_BasicIco;
	SplittableIcohedron m_UnsplitIco;
	SplittableIcohedron m_SplitIco1;
	SplittableIcohedron m_SplitIco2;
	SplittableIcohedron m_SplitIco3;
	SplittableIcohedron m_SplitIco4;

	BasicPrimativeCube m_BasicCube;
	SplittableCube m_UnsplitCube;
	SplittableCube m_SplitCube1;
	SplittableCube m_SplitCube2;
	SplittableCube m_SplitCube3;
	SplittableCube m_SplitCube4;

	GUILabel* m_CurrentFPSLabel;

	GLuint shaderTestV;
	GLuint shaderTestF;

	tdogl::Program* gProgram = NULL;

	void LoadShaders() {
		std::vector<tdogl::Shader> shaders;
		shaders.push_back(tdogl::Shader::shaderFromFile("Shaders/vertex-shader.txt", GL_VERTEX_SHADER));
		shaders.push_back(tdogl::Shader::shaderFromFile("Shaders/fragment-shader.txt", GL_FRAGMENT_SHADER));
		gProgram = new tdogl::Program(shaders);
	}
};

inline World3DExample::World3DExample()
{
	m_BasicIco.SetValues(10.0f, true, 50.0f);
	m_UnsplitIco.SetValues(10.0f, 0, true, 50.0f);
	m_SplitIco1.SetValues(10.0f, 1, true, 50.0f);
	m_SplitIco2.SetValues(10.0f, 3, true, 50.0f);
	m_SplitIco3.SetValues(10.0f, 5, true, 50.0f);
	m_SplitIco4.SetValues(10.0f, 6, true, 50.0f);

	m_BasicCube.SetValues(10.0f, 10.0f, 10.0f, true, 50.0f);
	m_UnsplitCube.SetValues(10.0f, 0, true, 50.0f);
	m_SplitCube1.SetValues(10.0f, 1, true, 50.0f);
	m_SplitCube2.SetValues(10.0f, 3, true, 50.0f);
	m_SplitCube3.SetValues(10.0f, 5, true, 50.0f);
	m_SplitCube4.SetValues(10.0f, 6, true, 50.0f);

	//  Create the current fps label
	m_CurrentFPSLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Current FPS: Unknown", 10, 748, 260, 22);
	AddChild(m_CurrentFPSLabel);

	LoadShaders();

	// setup GLM Camera
	gCamera.setPosition(glm::vec3(0, 0, 4));
	gCamera.setRotation(0.0f, 0.0f);
	gCamera.setViewportAspectRatio(SCREEN_SIZE_W / SCREEN_SIZE_H);
}

inline World3DExample::~World3DExample()
{
}

inline void World3DExample::Input(int xOffset, int yOffset)
{
	// Take input for the camera
	TakeCameraInput();

	GUIObjectNode::Input(xOffset, yOffset);
}

inline void World3DExample::Update()
{
	m_CurrentFPSLabel->SetText("Current FPS: " + std::to_string(averageFPS));

	GUIObjectNode::Update();
}

inline void World3DExample::Render3D()
{
	gCamera.ApplyTransform();

	bool shader = false;
	if (KEY_PRESSED(SDL_SCANCODE_V)) shader = true;

	if (shader == true)
	{
		gProgram->use();
		gProgram->setUniform("camera", gCamera.matrix());
	}

	if (shader == true) gProgram->setUniform("model", glm::rotate(glm::translate(glm::mat4(), glm::vec3(-20, 0, -80)), glm::radians(m_BasicCube.getRotation()), glm::vec3(0, 1, 0)));
	m_BasicCube.Render(Vector3<float>(-20, 0, -80));
	if (shader == true) gProgram->setUniform("model", glm::rotate(glm::translate(glm::mat4(), glm::vec3(-20, 0, -100)), glm::radians(m_UnsplitCube.getRotation()), glm::vec3(0, 1, 0)));
	m_UnsplitCube.Render(Vector3<float>(-20, 0, -100));
	if (shader == true) gProgram->setUniform("model", glm::rotate(glm::translate(glm::mat4(), glm::vec3(-20, 0, -120)), glm::radians(m_SplitCube1.getRotation()), glm::vec3(0, 1, 0)));
	m_SplitCube1.Render(Vector3<float>(-20, 0, -120));
	if (shader == true) gProgram->setUniform("model", glm::rotate(glm::translate(glm::mat4(), glm::vec3(-20, 0, -140)), glm::radians(m_SplitCube2.getRotation()), glm::vec3(0, 1, 0)));
	m_SplitCube2.Render(Vector3<float>(-20, 0, -140));
	if (shader == true) gProgram->setUniform("model", glm::rotate(glm::translate(glm::mat4(), glm::vec3(-20, 0, -160)), glm::radians(m_SplitCube3.getRotation()), glm::vec3(0, 1, 0)));
	m_SplitCube3.Render(Vector3<float>(-20, 0, -160));
	if (shader == true) gProgram->setUniform("model", glm::rotate(glm::translate(glm::mat4(), glm::vec3(-20, 0, -180)), glm::radians(m_SplitCube4.getRotation()), glm::vec3(0, 1, 0)));
	m_SplitCube4.Render(Vector3<float>(-20, 0, -180));

	if (shader == true) gProgram->setUniform("model", glm::rotate(glm::translate(glm::mat4(), glm::vec3(20, 0, -80)), glm::radians(m_BasicIco.getRotation()), glm::vec3(0, 1, 0)));
	m_BasicIco.Render(Vector3<float>(20, 0, -80));
	if (shader == true) gProgram->setUniform("model", glm::rotate(glm::translate(glm::mat4(), glm::vec3(20, 0, -100)), glm::radians(m_UnsplitIco.getRotation()), glm::vec3(0, 1, 0)));
	m_UnsplitIco.Render(Vector3<float>(20, 0, -100));
	if (shader == true) gProgram->setUniform("model", glm::rotate(glm::translate(glm::mat4(), glm::vec3(20, 0, -120)), glm::radians(m_SplitIco1.getRotation()), glm::vec3(0, 1, 0)));
	m_SplitIco1.Render(Vector3<float>(20, 0, -120));
	if (shader == true) gProgram->setUniform("model", glm::rotate(glm::translate(glm::mat4(), glm::vec3(20, 0, -140)), glm::radians(m_SplitIco2.getRotation()), glm::vec3(0, 1, 0)));
	m_SplitIco2.Render(Vector3<float>(20, 0, -140));
	if (shader == true) gProgram->setUniform("model", glm::rotate(glm::translate(glm::mat4(), glm::vec3(20, 0, -160)), glm::radians(m_SplitIco3.getRotation()), glm::vec3(0, 1, 0)));
	m_SplitIco3.Render(Vector3<float>(20, 0, -160));
	if (shader == true) gProgram->setUniform("model", glm::rotate(glm::translate(glm::mat4(), glm::vec3(20, 0, -180)), glm::radians(m_SplitIco4.getRotation()), glm::vec3(0, 1, 0)));
	m_SplitIco4.Render(Vector3<float>(20, 0, -180));

	if (shader == true)
	{
		gProgram->stopUsing();
	}
}

inline void World3DExample::TakeCameraInput()
{
	//  Move position of camera based on WASD keys, and SPACE and CTRL keys for up and down
	const float moveSpeed = 50.0; //units per second
	if (KEY_PRESSED(SDL_SCANCODE_S))		gCamera.offsetPosition(frameSecondsF * moveSpeed * -gCamera.forward());
	if (KEY_PRESSED(SDL_SCANCODE_W))		gCamera.offsetPosition(frameSecondsF * moveSpeed * gCamera.forward());
	if (KEY_PRESSED(SDL_SCANCODE_A))		gCamera.offsetPosition(frameSecondsF * moveSpeed * -gCamera.right());
	if (KEY_PRESSED(SDL_SCANCODE_D))		gCamera.offsetPosition(frameSecondsF * moveSpeed * gCamera.right());
	if (KEY_PRESSED(SDL_SCANCODE_LCTRL))	gCamera.offsetPosition(frameSecondsF * moveSpeed * -glm::vec3(0, 1, 0));
	if (KEY_PRESSED(SDL_SCANCODE_SPACE))	gCamera.offsetPosition(frameSecondsF * moveSpeed * glm::vec3(0, 1, 0));

	//  Rotate camera based on arrow keys
	if (KEY_PRESSED(SDL_SCANCODE_UP))		gCamera.offsetOrientation(-frameSecondsF * moveSpeed, 0.0f);
	if (KEY_PRESSED(SDL_SCANCODE_DOWN))		gCamera.offsetOrientation(frameSecondsF * moveSpeed, 0.0f);
	if (KEY_PRESSED(SDL_SCANCODE_LEFT))		gCamera.offsetOrientation(0.0f, -frameSecondsF * moveSpeed);
	if (KEY_PRESSED(SDL_SCANCODE_RIGHT))	gCamera.offsetOrientation(0.0f, frameSecondsF * moveSpeed);
}