#pragma once

#include "Engine/GUIObjectNode.h"
#include "Engine/TextureAnimation.h"
#include "Engine/InputManager.h"
#include "Engine/TimeSlice.h"

class TopDownCharacter
{
public:
	enum CharacterState { CHARSTATE_IDLE_UP, CHARSTATE_IDLE_DOWN, CHARSTATE_IDLE_LEFT, CHARSTATE_IDLE_RIGHT, CHARSTATE_WALK_UP, CHARSTATE_WALK_DOWN, CHARSTATE_WALK_LEFT, CHARSTATE_WALK_RIGHT, CHARSTATE_COUNT };

	TopDownCharacter(int x, int y) :
		m_X(x),
		m_Y(y),
		m_CurrentState(CHARSTATE_IDLE_DOWN)
	{
		for (auto i = 0; i < CHARSTATE_COUNT; ++i) m_StateAnimation[i] = nullptr;
	}

	inline CharacterState GetCurrentState() const { return m_CurrentState; }
	inline int GetCharacterX() const { return m_X; }
	inline int GetCharacterY() const { return m_Y; }

	inline void LoadCharacterAnimation(CharacterState charState, const char* animationFile) { m_StateAnimation[charState] = TextureAnimation::CreateTextureAnimation(animationFile); }
	inline void SetCharacterState(CharacterState newState) { m_CurrentState = newState; }
	inline void SetCharacterX(int x) { m_X = x; }
	inline void SetCharacterY(int y) { m_Y = y; }

	void Render(int xOffset, int yOffset) const
	{
		if (m_StateAnimation[m_CurrentState] == nullptr) return;
		m_StateAnimation[m_CurrentState]->Render(xOffset + m_X, yOffset + m_Y);
	}

private:
	int m_X;
	int m_Y;
	CharacterState m_CurrentState;
	TextureAnimation* m_StateAnimation[CHARSTATE_COUNT];
};

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

	TopDownCharacter* m_Characters[2];

	enum KeyPress { KEYPRESS_UP, KEYPRESS_DOWN, KEYPRESS_LEFT, KEYPRESS_RIGHT, KEYPRESS_COUNT };
	bool m_Pressed[KEYPRESS_COUNT];
};

inline TopDownExample::TopDownExample()
{
	//  Load the player character
	m_Characters[0] = new TopDownCharacter(512, 384);
	m_Characters[0]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_IDLE_UP, "Assets/Sprites/LinkUpIdle.xml");
	m_Characters[0]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_IDLE_DOWN, "Assets/Sprites/LinkDownIdle.xml");
	m_Characters[0]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_IDLE_LEFT, "Assets/Sprites/LinkLeftIdle.xml");
	m_Characters[0]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_IDLE_RIGHT, "Assets/Sprites/LinkRightIdle.xml");
	m_Characters[0]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_WALK_UP, "Assets/Sprites/LinkUpWalk.xml");
	m_Characters[0]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_WALK_DOWN, "Assets/Sprites/LinkDownWalk.xml");
	m_Characters[0]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_WALK_LEFT, "Assets/Sprites/LinkLeftWalk.xml");
	m_Characters[0]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_WALK_RIGHT, "Assets/Sprites/LinkRightWalk.xml");

	//  Load the enemy character
	m_Characters[1] = new TopDownCharacter(700, 384);
	m_Characters[1]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_IDLE_UP, "Assets/Sprites/GuardUpIdle.xml");
	m_Characters[1]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_IDLE_DOWN, "Assets/Sprites/GuardDownIdle.xml");
	m_Characters[1]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_IDLE_LEFT, "Assets/Sprites/GuardLeftIdle.xml");
	m_Characters[1]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_IDLE_RIGHT, "Assets/Sprites/GuardRightIdle.xml");
	m_Characters[1]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_WALK_UP, "Assets/Sprites/GuardUpWalk.xml");
	m_Characters[1]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_WALK_DOWN, "Assets/Sprites/GuardDownWalk.xml");
	m_Characters[1]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_WALK_LEFT, "Assets/Sprites/GuardLeftWalk.xml");
	m_Characters[1]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_WALK_RIGHT, "Assets/Sprites/GuardRightWalk.xml");
}

inline TopDownExample::~TopDownExample()
{
	delete m_Characters[0];
	delete m_Characters[1];
}

inline void TopDownExample::Input(int xOffset, int yOffset)
{
	// Take input for the top-down player character
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
	m_Pressed[KEYPRESS_UP] = inputManager.GetKeyDown(SDL_SCANCODE_UP);
	m_Pressed[KEYPRESS_DOWN] = inputManager.GetKeyDown(SDL_SCANCODE_DOWN);
	m_Pressed[KEYPRESS_LEFT] = inputManager.GetKeyDown(SDL_SCANCODE_LEFT);
	m_Pressed[KEYPRESS_RIGHT] = inputManager.GetKeyDown(SDL_SCANCODE_RIGHT);

	if (m_Pressed[KEYPRESS_UP] && m_Pressed[KEYPRESS_DOWN]) { m_Pressed[KEYPRESS_UP] = false; m_Pressed[KEYPRESS_DOWN] = false; }
	if (m_Pressed[KEYPRESS_LEFT] && m_Pressed[KEYPRESS_RIGHT]) { m_Pressed[KEYPRESS_LEFT] = false; m_Pressed[KEYPRESS_RIGHT] = false; }

	TopDownCharacter::CharacterState currentState = m_Characters[0]->GetCurrentState();
	if (m_Pressed[KEYPRESS_UP])		currentState = TopDownCharacter::CHARSTATE_WALK_UP;
	if (m_Pressed[KEYPRESS_DOWN])	currentState = TopDownCharacter::CHARSTATE_WALK_DOWN;
	if (m_Pressed[KEYPRESS_LEFT])	currentState = TopDownCharacter::CHARSTATE_WALK_LEFT;
	if (m_Pressed[KEYPRESS_RIGHT])	currentState = TopDownCharacter::CHARSTATE_WALK_RIGHT;

	if (!m_Pressed[KEYPRESS_UP] && !m_Pressed[KEYPRESS_DOWN] && !m_Pressed[KEYPRESS_LEFT] && !m_Pressed[KEYPRESS_RIGHT])
	{
		switch (currentState)
		{
		case TopDownCharacter::CHARSTATE_WALK_UP:		currentState = TopDownCharacter::CHARSTATE_IDLE_UP;		break;
		case TopDownCharacter::CHARSTATE_WALK_DOWN:		currentState = TopDownCharacter::CHARSTATE_IDLE_DOWN;	break;
		case TopDownCharacter::CHARSTATE_WALK_LEFT:		currentState = TopDownCharacter::CHARSTATE_IDLE_LEFT;	break;
		case TopDownCharacter::CHARSTATE_WALK_RIGHT:	currentState = TopDownCharacter::CHARSTATE_IDLE_RIGHT;	break;
		}
	}

	m_Characters[0]->SetCharacterState(currentState);
	m_Characters[1]->SetCharacterState(currentState);
}

inline void TopDownExample::UpdateCharacter()
{
	int x = m_Characters[0]->GetCharacterX();
	int y = m_Characters[0]->GetCharacterY();
	if (m_Pressed[KEYPRESS_UP]) y -= int(frameSeconds * 110.0f);
	if (m_Pressed[KEYPRESS_DOWN]) y += int(frameSeconds * 110.0f);
	if (m_Pressed[KEYPRESS_LEFT]) x -= int(frameSeconds * 110.0f);
	if (m_Pressed[KEYPRESS_RIGHT]) x += int(frameSeconds * 110.0f);

	m_Characters[0]->SetCharacterX(x);
	m_Characters[0]->SetCharacterY(y);
	m_Characters[1]->SetCharacterX(x + 100);
	m_Characters[1]->SetCharacterY(y - 4);
}

inline void TopDownExample::RenderCharacter(int xOffset, int yOffset)
{
	if (m_Characters[0] != nullptr) m_Characters[0]->Render(xOffset, yOffset);
	if (m_Characters[1] != nullptr) m_Characters[1]->Render(xOffset, yOffset);
}