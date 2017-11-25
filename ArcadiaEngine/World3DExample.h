#pragma once

#include <iostream>

#include "Engine/GUIObjectNode.h"
#include "Engine/InputManager.h"
#include "Engine/TimeSlice.h"
#include "Engine/OpenGLCamera.h"
#include "Engine/BasicPrimativeIcohedron.h"

class World3DExample : public GUIObjectNode
{
public:
	World3DExample();
	~World3DExample();

	void Input(int xOffset = 0, int yOffset = 0) override;
	void Render3D() override;

private:
	void TakeCameraInput();

	OpenGLCamera m_Camera;
	BasicPrimativeIcohedron m_3DObject;
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

inline void World3DExample::Render3D()
{
	m_Camera.ApplyCameraTransform();

	m_3DObject.Render(Vector3<float>(0, 0, 80));
}

inline void World3DExample::TakeCameraInput()
{
	if (KEY_PRESSED(SDL_SCANCODE_W))		m_Camera.MoveForward(frameSeconds * 110.0f);
	if (KEY_PRESSED(SDL_SCANCODE_S))		m_Camera.MoveForward(frameSeconds * -110.0f);
	if (KEY_PRESSED(SDL_SCANCODE_A))		m_Camera.MoveRight(frameSeconds * -110.0f);
	if (KEY_PRESSED(SDL_SCANCODE_D))		m_Camera.MoveRight(frameSeconds * 110.0f);
	if (KEY_PRESSED(SDL_SCANCODE_UP))		m_Camera.RotateLocalX(frameSeconds * -2.0f);
	if (KEY_PRESSED(SDL_SCANCODE_DOWN))		m_Camera.RotateLocalX(frameSeconds * 2.0f);
	if (KEY_PRESSED(SDL_SCANCODE_LEFT))		m_Camera.RotateWorld(frameSeconds * 2.0f, 0.0f, 1.0f, 0.0f);
	if (KEY_PRESSED(SDL_SCANCODE_RIGHT))	m_Camera.RotateWorld(frameSeconds * -2.0f, 0.0f, 1.0f, 0.0f);
	if (KEY_PRESSED(SDL_SCANCODE_LCTRL))	m_Camera.TranslateWorld(0.0f, frameSeconds * -60.0f, 0.0f);
	if (KEY_PRESSED(SDL_SCANCODE_SPACE))	m_Camera.TranslateWorld(0.0f, frameSeconds *  60.0f, 0.0f);

}