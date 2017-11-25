#pragma once

#include <iostream>

#include "Engine/GUIObjectNode.h"
#include "Engine/TextureAnimation.h"
#include "Engine/InputManager.h"
#include "Engine/TimeSlice.h"
#include "Engine/OpenGLCamera.h"

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

	enum KeyPress { KEYPRESS_UP, KEYPRESS_DOWN, KEYPRESS_LEFT, KEYPRESS_RIGHT, KEYPRESS_CTRL, KEYPRESS_W, KEYPRESS_A, KEYPRESS_S, KEYPRESS_D, KEYPRESS_COUNT };
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

	//  Basic 3D cube in quads
	auto cubeHalf = 10.0f;
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_TRIANGLE_STRIP);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);
		glVertex3f(0, 1, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(0, 1, 1);
		glVertex3f(1, 1, 1);
		glVertex3f(0, 0, 1);
		glVertex3f(1, 0, 1);
		glVertex3f(1, 0, 1);
		glVertex3f(1, 0, 0);
		glVertex3f(1, 0, 1);
		glVertex3f(1, 1, 0);
		glVertex3f(1, 1, 1);
		glVertex3f(0, 1, 0);
		glVertex3f(0, 1, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);
	glEnd();

	//  Basic 3D cube in lines, to outline
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(3);
	glBegin(GL_LINE_STRIP);
		glVertex3f(0, 1, 0);
		glVertex3f(0, 0, 0);
		glVertex3f(1, 0, 0);
		glVertex3f(1, 1, 0);
		glVertex3f(0, 1, 0);
		glVertex3f(0, 1, 1);
		glVertex3f(1, 1, 1);
		glVertex3f(1, 1, 0);
		glVertex3f(1, 0, 0);
		glVertex3f(1, 0, 1);
		glVertex3f(1, 1, 1);
		glVertex3f(0, 1, 1);
		glVertex3f(0, 0, 1);
		glVertex3f(0, 0, 1);
		glVertex3f(0, 0, 0);
		glVertex3f(0, 0, 1);
		glVertex3f(1, 0, 1);
	glEnd();
}

inline void World3DExample::TakeCameraInput()
{
	m_Pressed[KEYPRESS_W] = inputManager.GetKeyPressed(SDL_SCANCODE_W);
	m_Pressed[KEYPRESS_A] = inputManager.GetKeyPressed(SDL_SCANCODE_A);
	m_Pressed[KEYPRESS_S] = inputManager.GetKeyPressed(SDL_SCANCODE_S);
	m_Pressed[KEYPRESS_D] = inputManager.GetKeyPressed(SDL_SCANCODE_D);
	m_Pressed[KEYPRESS_UP] = inputManager.GetKeyPressed(SDL_SCANCODE_UP);
	m_Pressed[KEYPRESS_DOWN] = inputManager.GetKeyPressed(SDL_SCANCODE_DOWN);
	m_Pressed[KEYPRESS_LEFT] = inputManager.GetKeyPressed(SDL_SCANCODE_LEFT);
	m_Pressed[KEYPRESS_RIGHT] = inputManager.GetKeyPressed(SDL_SCANCODE_RIGHT);

	if (m_Pressed[KEYPRESS_W])		m_Camera.MoveForward(frameSeconds * 110.0f);
	if (m_Pressed[KEYPRESS_S])		m_Camera.MoveForward(frameSeconds * -110.0f);
	if (m_Pressed[KEYPRESS_A])		m_Camera.MoveRight(frameSeconds * -110.0f);
	if (m_Pressed[KEYPRESS_D])		m_Camera.MoveRight(frameSeconds * 110.0f);
	if (m_Pressed[KEYPRESS_UP])		m_Camera.RotateWorld(frameSeconds * 3.0f, -1.0f, 0.0f, 0.0f);
	if (m_Pressed[KEYPRESS_DOWN])	m_Camera.RotateWorld(frameSeconds * 3.0f,  1.0f, 0.0f, 0.0f);
	if (m_Pressed[KEYPRESS_LEFT])	m_Camera.RotateLocalY(frameSeconds * 5.0f, true);
	if (m_Pressed[KEYPRESS_RIGHT])	m_Camera.RotateLocalY(frameSeconds * -5.0f, true);
}