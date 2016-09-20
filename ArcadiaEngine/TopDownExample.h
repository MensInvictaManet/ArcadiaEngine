#pragma once

#include "Engine/GUIObjectNode.h"
#include "Engine/TextureAnimation.h"
#include "Engine/InputManager.h"
#include "Engine/TimeSlice.h"

class TopDownExample : public GUIObjectNode
{
public:
	TopDownExample();
	~TopDownExample();

	void Input(int xOffset = 0, int yOffset = 0) override;
	void Update() override;
	void Render(int xOffset = 0, int yOffset = 0) override;

private:
	void TakeCharacterInput();
	void UpdateCharacter();
	void RenderCharacter(int xOffset, int yOffset);

	enum CharacterState { CHARSTATE_IDLE_UP, CHARSTATE_IDLE_DOWN, CHARSTATE_IDLE_LEFT, CHARSTATE_IDLE_RIGHT, CHARSTATE_WALK_UP, CHARSTATE_WALK_DOWN, CHARSTATE_WALK_LEFT, CHARSTATE_WALK_RIGHT, CHARSTATE_COUNT };
	TextureAnimation* m_Character[CHARSTATE_COUNT];
	CharacterState m_CurrentState;

	int m_CharacterX;
	int m_CharacterY;

	enum KeyPress { KEYPRESS_UP, KEYPRESS_DOWN, KEYPRESS_LEFT, KEYPRESS_RIGHT, KEYPRESS_COUNT };
	bool m_Pressed[KEYPRESS_COUNT];
};

inline TopDownExample::TopDownExample() : 
	m_CurrentState(CHARSTATE_IDLE_DOWN),
	m_CharacterX(512),
	m_CharacterY(384)
{
	for (auto i = 0; i < CHARSTATE_COUNT; ++i) m_Character[i] = nullptr;

	m_Character[CHARSTATE_IDLE_UP]		= TextureAnimation::CreateTextureAnimation("Assets/Sprites/LinkUpIdle.xml");
	m_Character[CHARSTATE_IDLE_DOWN]	= TextureAnimation::CreateTextureAnimation("Assets/Sprites/LinkDownIdle.xml");
	m_Character[CHARSTATE_IDLE_LEFT]	= TextureAnimation::CreateTextureAnimation("Assets/Sprites/LinkLeftIdle.xml");
	m_Character[CHARSTATE_IDLE_RIGHT]	= TextureAnimation::CreateTextureAnimation("Assets/Sprites/LinkRightIdle.xml");
	m_Character[CHARSTATE_WALK_UP]		= TextureAnimation::CreateTextureAnimation("Assets/Sprites/LinkUpWalk.xml");
	m_Character[CHARSTATE_WALK_DOWN]	= TextureAnimation::CreateTextureAnimation("Assets/Sprites/LinkDownWalk.xml");
	m_Character[CHARSTATE_WALK_LEFT]	= TextureAnimation::CreateTextureAnimation("Assets/Sprites/LinkLeftWalk.xml");
	m_Character[CHARSTATE_WALK_RIGHT]	= TextureAnimation::CreateTextureAnimation("Assets/Sprites/LinkRightWalk.xml");
}

inline TopDownExample::~TopDownExample()
{

}

inline void TopDownExample::Input(int xOffset, int yOffset)
{
	// Take input for the top-down character
	TakeCharacterInput();

	GUIObjectNode::Input(xOffset, yOffset);
}

inline void TopDownExample::Update()
{
	// Render the top-down character
	UpdateCharacter();

	GUIObjectNode::Update();
}

inline void TopDownExample::Render(int xOffset, int yOffset)
{
	// Render the top-down character
	RenderCharacter(xOffset, yOffset);

	GUIObjectNode::Render(xOffset, yOffset);
}

inline void TopDownExample::TakeCharacterInput()
{
	m_Pressed[KEYPRESS_UP]		= inputManager.GetKeyDown(SDL_SCANCODE_UP);
	m_Pressed[KEYPRESS_DOWN]	= inputManager.GetKeyDown(SDL_SCANCODE_DOWN);
	m_Pressed[KEYPRESS_LEFT]	= inputManager.GetKeyDown(SDL_SCANCODE_LEFT);
	m_Pressed[KEYPRESS_RIGHT]	= inputManager.GetKeyDown(SDL_SCANCODE_RIGHT);

	if (m_Pressed[KEYPRESS_UP] && m_Pressed[KEYPRESS_DOWN]) { m_Pressed[KEYPRESS_UP] = false; m_Pressed[KEYPRESS_DOWN] = false; }
	if (m_Pressed[KEYPRESS_LEFT] && m_Pressed[KEYPRESS_RIGHT]) { m_Pressed[KEYPRESS_LEFT] = false; m_Pressed[KEYPRESS_RIGHT] = false; }

	if (m_Pressed[KEYPRESS_UP])		m_CurrentState = CHARSTATE_WALK_UP;
	if (m_Pressed[KEYPRESS_DOWN])	m_CurrentState = CHARSTATE_WALK_DOWN;
	if (m_Pressed[KEYPRESS_LEFT])	m_CurrentState = CHARSTATE_WALK_LEFT;
	if (m_Pressed[KEYPRESS_RIGHT])	m_CurrentState = CHARSTATE_WALK_RIGHT;

	if (!m_Pressed[KEYPRESS_UP] && !m_Pressed[KEYPRESS_DOWN] && !m_Pressed[KEYPRESS_LEFT] && !m_Pressed[KEYPRESS_RIGHT])
	{
		switch (m_CurrentState)
		{
		case CHARSTATE_WALK_UP:		m_CurrentState = CHARSTATE_IDLE_UP;		break;
		case CHARSTATE_WALK_DOWN:	m_CurrentState = CHARSTATE_IDLE_DOWN;	break;
		case CHARSTATE_WALK_LEFT:	m_CurrentState = CHARSTATE_IDLE_LEFT;	break;
		case CHARSTATE_WALK_RIGHT:	m_CurrentState = CHARSTATE_IDLE_RIGHT;	break;
		}
	}
}

inline void TopDownExample::UpdateCharacter()
{
	if (m_Pressed[KEYPRESS_UP]) m_CharacterY -= int(frameSeconds * 110.0f);
	if (m_Pressed[KEYPRESS_DOWN]) m_CharacterY += int(frameSeconds * 110.0f);
	if (m_Pressed[KEYPRESS_LEFT]) m_CharacterX -= int(frameSeconds * 110.0f);
	if (m_Pressed[KEYPRESS_RIGHT]) m_CharacterX += int(frameSeconds * 110.0f);
}

inline void TopDownExample::RenderCharacter(int xOffset, int yOffset)
{
	if (m_Character[m_CurrentState] == nullptr) return;
	m_Character[m_CurrentState]->Render(xOffset + m_CharacterX, yOffset + m_CharacterY);
}