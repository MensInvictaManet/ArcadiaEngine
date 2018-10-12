#pragma once

#include <iostream>

#include "Engine/GUIObjectNode.h"
#include "Engine/InputManager.h"
#include "Engine/TimeSlice.h"
#include "Engine/BasicPrimativeCube.h"
#include "Engine/BasicPrimativeIcosahedron.h"
#include "Engine/SplittableIcosahedron.h"
#include "Engine/SplittableCube.h"
#include "Engine/BasicPrimativeQuad.h"

#include "Engine/GLMCamera.h"
#include "Engine/Shader.h"
#include "Engine/Program.h"

class World3DExample : public GUIObjectNode
{
public:
	World3DExample(float screenWidth, float screenHeight);
	~World3DExample();

	void Input(int xOffset = 0, int yOffset = 0) override;
	void Update() override;
	void Render3D() override;

private:
	void TakeCameraInput();

	Camera m_Camera;

	BasicPrimativeIcosahedron m_BasicIco;
	SplittableIcosahedron m_UnsplitIco;
	SplittableIcosahedron m_SplitIco1;
	SplittableIcosahedron m_SplitIco2;
	SplittableIcosahedron m_SplitIco3;
	SplittableIcosahedron m_SplitIco4;

	BasicPrimativeCube m_BasicCube;
	SplittableCube m_UnsplitCube;
	SplittableCube m_SplitCube1;
	SplittableCube m_SplitCube2;
	SplittableCube m_SplitCube3;
	SplittableCube m_SplitCube4;

	BasicPrimativeQuad m_BasicQuad1;
	BasicPrimativeQuad m_BasicQuad2;

	GUILabel* m_CurrentFPSLabel;

	GLuint shaderTestV;
	GLuint shaderTestF;

	tdogl::Program* gProgram = nullptr;

	GLuint m_D6Texture = 0;
	GLuint m_D20Texture = 0;
	GLuint m_NoTexture = 0;

	void LoadShaders() {
		std::vector<tdogl::Shader> shaders;
		shaders.push_back(tdogl::Shader::shaderFromFile("Shaders/VertexShader_Basic.txt", GL_VERTEX_SHADER));
		shaders.push_back(tdogl::Shader::shaderFromFile("Shaders/FragmentShader_Basic.txt", GL_FRAGMENT_SHADER));
		gProgram = new tdogl::Program(shaders);
	}
};

inline World3DExample::World3DExample(float screenWidth, float screenHeight)
{
	LoadShaders();

	m_D6Texture = textureManager.LoadTextureGetID("Assets/3D Base Textures/D6Dice.png");
	m_D20Texture = textureManager.LoadTextureGetID("Assets/3D Base Textures/D20Dice.png");
	m_NoTexture = textureManager.LoadTextureGetID("Assets/3D Base Textures/NoTexture.png");

	m_BasicIco.SetValues(10.0f, true, 50.0f, m_D20Texture, gProgram);
	m_UnsplitIco.SetValues(10.0f, 0, false, 50.0f, m_D20Texture, gProgram);
	m_SplitIco1.SetValues( 10.0f, 1, false, 50.0f, m_D20Texture, gProgram);
	m_SplitIco2.SetValues( 10.0f, 3, true, 50.0f, m_D20Texture, gProgram);
	m_SplitIco3.SetValues( 10.0f, 5, false, 50.0f, m_D20Texture, gProgram);
	m_SplitIco4.SetValues( 10.0f, 6, false, 50.0f, m_D20Texture, gProgram);

	m_BasicCube.SetValues(10.0f, 10.0f, 10.0f, false, 50.0f, m_D6Texture, gProgram);
	m_UnsplitCube.SetValues(10.0f, 0, false, 50.0f, m_D6Texture, gProgram);
	m_SplitCube1.SetValues( 10.0f, 1, false, 50.0f, m_D6Texture, gProgram);
	m_SplitCube2.SetValues( 10.0f, 3, true, 50.0f, m_D6Texture, gProgram);
	m_SplitCube3.SetValues( 10.0f, 5, false, 50.0f, m_D6Texture, gProgram);
	m_SplitCube4.SetValues( 10.0f, 6, false, 50.0f, m_D6Texture, gProgram);

	m_BasicQuad1.SetValues(10.0f, 10.0f, true, 25.0f, m_NoTexture, gProgram);
	m_BasicQuad2.SetValues(10.0f, 10.0f, false, 25.0f, m_NoTexture, gProgram);

	//  Create the current fps label
	m_CurrentFPSLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Current FPS: Unknown", 10, 748, 260, 22);
	AddChild(m_CurrentFPSLabel);

	// setup GLM Camera
	gCamera.setPosition(glm::vec3(0, 0, 4));
	gCamera.setRotation(0.0f, 0.0f);
	gCamera.setViewportAspectRatio(screenWidth / screenHeight);
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

	//  CUBES
	auto cube1Pos = Vector3<float>(-20, 0, -80);
	m_BasicCube.Render(cube1Pos, gCamera);

	auto cube2Pos = Vector3<float>(-20, 0, -100);
	m_UnsplitCube.Render(cube2Pos, gCamera);

	auto cube3Pos = Vector3<float>(-20, 0, -120);
	m_SplitCube1.Render(cube3Pos, gCamera);

	auto cube4Pos = Vector3<float>(-20, 0, -140);
	m_SplitCube2.Render(cube4Pos, gCamera);

	auto cube5Pos = Vector3<float>(-20, 0, -160);
	m_SplitCube3.Render(cube5Pos, gCamera);

	auto cube6Pos = Vector3<float>(-20, 0, -180);
	m_SplitCube4.Render(cube6Pos, gCamera);

	//  ICOSEHEDRONS
	auto ico1Pos = Vector3<float>(20, 0, -80);
	m_BasicIco.Render(ico1Pos, gCamera);

	auto ico2Pos = Vector3<float>(20, 0, -100);
	m_UnsplitIco.Render(ico2Pos, gCamera);

	auto ico3Pos = Vector3<float>(20, 0, -120);
	m_SplitIco1.Render(ico3Pos, gCamera);

	auto ico4Pos = Vector3<float>(20, 0, -140);
	m_SplitIco2.Render(ico4Pos, gCamera);

	auto ico5Pos = Vector3<float>(20, 0, -160);
	m_SplitIco3.Render(ico5Pos, gCamera);

	auto ico6Pos = Vector3<float>(20, 0, -180);
	m_SplitIco4.Render(ico6Pos, gCamera);

	//  QUADS
	auto quad1Pos = Vector3<float>(60, 0, -80);
	m_BasicQuad1.Render(quad1Pos, gCamera);

	auto quad2Pos = Vector3<float>(60, 0, -100);
	m_BasicQuad2.Render(quad2Pos, gCamera);
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