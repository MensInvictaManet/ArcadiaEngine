#pragma once

#include <iostream>

#include "Engine/GUIObjectNode.h"
#include "Engine/InputManager.h"
#include "Engine/TimeSlice.h"
#include "Engine/OpenGLCamera.h"
#include "Engine/SplittableIcohedron.h"
#include "Engine/SplittableCube.h"

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

	OpenGLCamera m_Camera;

	SplittableIcohedron m_UnsplitIco;
	SplittableIcohedron m_SplitIco1;
	SplittableIcohedron m_SplitIco2;
	SplittableIcohedron m_SplitIco3;
	SplittableIcohedron m_SplitIco4;

	SplittableCube m_UnsplitCube;
	SplittableCube m_SplitCube1;
	SplittableCube m_SplitCube2;
	SplittableCube m_SplitCube3;
	SplittableCube m_SplitCube4;

	GUILabel* m_CurrentFPSLabel;
};

inline World3DExample::World3DExample()
{
	m_SplitIco1.SetValues(10.0f, 1, true, 5.0f);
	m_SplitIco2.SetValues(10.0f, 3, true, 8.0f);
	m_SplitIco3.SetValues(10.0f, 5, true, 11.0f);
	m_SplitIco4.SetValues(10.0f, 6, true, 13.0f);

	m_SplitCube1.SetValues(10.0f, 1, true, 5.0f);
	m_SplitCube2.SetValues(10.0f, 3, true, 8.0f);
	m_SplitCube3.SetValues(10.0f, 5, true, 11.0f);
	m_SplitCube4.SetValues(10.0f, 6, true, 13.0f);

	//  Create the current fps label
	m_CurrentFPSLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Current FPS: Unknown", 10, 748, 260, 22);
	AddChild(m_CurrentFPSLabel);
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
	m_Camera.ApplyCameraTransform();

	m_UnsplitIco.Render(Vector3<float>(20, 0, 80));
	m_SplitIco1.Render(Vector3<float>(20, 0, 100));
	m_SplitIco2.Render(Vector3<float>(20, 0, 120));
	m_SplitIco3.Render(Vector3<float>(20, 0, 140));
	//m_SplitIco4.Render(Vector3<float>(20, 0, 160));

	m_UnsplitCube.Render(Vector3<float>(-20, 0, 80));
	m_SplitCube1.Render(Vector3<float>(-20, 0, 100));
	m_SplitCube2.Render(Vector3<float>(-20, 0, 120));
	m_SplitCube3.Render(Vector3<float>(-20, 0, 140));
	//m_SplitCube4.Render(Vector3<float>(-20, 0, 160));
}

inline void World3DExample::TakeCameraInput()
{
	if (KEY_PRESSED(SDL_SCANCODE_W))		m_Camera.MoveForward(frameSecondsF * 110.0f);
	if (KEY_PRESSED(SDL_SCANCODE_S))		m_Camera.MoveForward(frameSecondsF * -110.0f);
	if (KEY_PRESSED(SDL_SCANCODE_A))		m_Camera.MoveRight(frameSecondsF * -110.0f);
	if (KEY_PRESSED(SDL_SCANCODE_D))		m_Camera.MoveRight(frameSecondsF * 110.0f);
	if (KEY_PRESSED(SDL_SCANCODE_UP))		m_Camera.RotateLocalX(frameSecondsF * -2.0f);
	if (KEY_PRESSED(SDL_SCANCODE_DOWN))		m_Camera.RotateLocalX(frameSecondsF * 2.0f);
	if (KEY_PRESSED(SDL_SCANCODE_LEFT))		m_Camera.RotateWorld(frameSecondsF * 2.0f, 0.0f, 1.0f, 0.0f);
	if (KEY_PRESSED(SDL_SCANCODE_RIGHT))	m_Camera.RotateWorld(frameSecondsF * -2.0f, 0.0f, 1.0f, 0.0f);
	if (KEY_PRESSED(SDL_SCANCODE_LCTRL))	m_Camera.TranslateWorld(0.0f, frameSecondsF * -60.0f, 0.0f);
	if (KEY_PRESSED(SDL_SCANCODE_SPACE))	m_Camera.TranslateWorld(0.0f, frameSecondsF *  60.0f, 0.0f);

}