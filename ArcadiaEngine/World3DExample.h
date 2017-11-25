#pragma once

#include <iostream>

#include "Engine/GUIObjectNode.h"
#include "Engine/TextureAnimation.h"
#include "Engine/InputManager.h"
#include "Engine/TimeSlice.h"
#include "Engine/OpenGLCamera.h"
#include "Engine/BasicPrimativeCube.h"

class World3DExample : public GUIObjectNode
{
public:
	World3DExample();
	~World3DExample();

	void Input(int xOffset = 0, int yOffset = 0) override;
	void Update() override;
	void Render(int xOffset = 0, int yOffset = 0) override;
	void Render3D() override;

private:
	void TakeCameraInput();

	OpenGLCamera m_Camera;
	BasicPrimativeCube m_CubeObject;

	enum KeyPress { KEYPRESS_UP, KEYPRESS_DOWN, KEYPRESS_LEFT, KEYPRESS_RIGHT, KEYPRESS_CTRL, KEYPRESS_SPACE, KEYPRESS_W, KEYPRESS_A, KEYPRESS_S, KEYPRESS_D, KEYPRESS_COUNT };
	bool m_Pressed[KEYPRESS_COUNT];
};

inline World3DExample::World3DExample()
{
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
	GUIObjectNode::Update();
}

inline void World3DExample::Render(int xOffset, int yOffset)
{
	GUIObjectNode::Render(xOffset, yOffset);

	return;
}

inline void World3DExample::Render3D()
{
	m_Camera.ApplyCameraTransform();

	m_CubeObject.Render(Vector3<float>(0, 0, 20));
}

inline void World3DExample::TakeCameraInput()
{
	if (KEY_PRESSED(SDL_SCANCODE_W))		m_Camera.MoveForward(frameSeconds * 110.0f);
	if (KEY_PRESSED(SDL_SCANCODE_S))		m_Camera.MoveForward(frameSeconds * -110.0f);
	if (KEY_PRESSED(SDL_SCANCODE_A))		m_Camera.MoveRight(frameSeconds * -110.0f);
	if (KEY_PRESSED(SDL_SCANCODE_D))		m_Camera.MoveRight(frameSeconds * 110.0f);
	if (KEY_PRESSED(SDL_SCANCODE_UP))		m_Camera.RotateLocalX(frameSeconds * -2.0f);
	if (KEY_PRESSED(SDL_SCANCODE_DOWN))		m_Camera.RotateLocalX(frameSeconds * 2.0f);
	if (KEY_PRESSED(SDL_SCANCODE_LEFT))		m_Camera.RotateWorld(frameSeconds * 3.0f, 0.0f, 1.0f, 0.0f);
	if (KEY_PRESSED(SDL_SCANCODE_RIGHT))	m_Camera.RotateWorld(frameSeconds * -3.0f, 0.0f, 1.0f, 0.0f);
	if (KEY_PRESSED(SDL_SCANCODE_LCTRL))	m_Camera.TranslateWorld(0.0f, frameSeconds * -60.0f, 0.0f);
	if (KEY_PRESSED(SDL_SCANCODE_SPACE))	m_Camera.TranslateWorld(0.0f, frameSeconds *  60.0f, 0.0f);

}