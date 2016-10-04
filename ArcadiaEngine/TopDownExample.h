#pragma once

#include "Engine/GUIObjectNode.h"
#include "Engine/TextureAnimation.h"
#include "Engine/InputManager.h"
#include "Engine/TimeSlice.h"

class TopDownCharacter
{
public:
	enum CharacterState { CHARSTATE_IDLE_UP, CHARSTATE_IDLE_DOWN, CHARSTATE_IDLE_LEFT, CHARSTATE_IDLE_RIGHT, CHARSTATE_WALK_UP, CHARSTATE_WALK_DOWN, CHARSTATE_WALK_LEFT, CHARSTATE_WALK_RIGHT, CHARSTATE_SWING_UP, CHARSTATE_SWING_DOWN, CHARSTATE_SWING_LEFT, CHARSTATE_SWING_RIGHT, CHARSTATE_COUNT };

	TopDownCharacter(int x, int y) :
		m_X(x),
		m_Y(y),
		m_CurrentState(CHARSTATE_IDLE_DOWN)
	{
		for (auto i = 0; i < CHARSTATE_COUNT; ++i) m_StateAnimation[i] = nullptr;
	}

	//  Accessors
	CharacterState GetCurrentState() const { return m_CurrentState; }
	int GetCharacterX() const { return m_X; }
	int GetCharacterY() const { return m_Y; }
	int GetCharacterWidth() const { return m_StateAnimation[m_CurrentState]->GetCurrentFrame()->m_W; }
	int GetCharacterHeight() const { return m_StateAnimation[m_CurrentState]->GetCurrentFrame()->m_H; }
	bool IsCharacterSwinging() const { return (m_CurrentState == CHARSTATE_SWING_UP || m_CurrentState == CHARSTATE_SWING_DOWN || m_CurrentState == CHARSTATE_SWING_LEFT || m_CurrentState == CHARSTATE_SWING_RIGHT); }

	//  Modifiers
	void LoadCharacterAnimation(CharacterState charState, const char* animationFile) { m_StateAnimation[charState] = TextureAnimation::CreateTextureAnimation(animationFile); }
	void SetCharacterState(CharacterState newState) { if (m_CurrentState != newState) m_StateAnimation[newState]->ResetAnimation(); m_CurrentState = newState; }
	void SetCharacterX(int x) { m_X = x; }
	void SetCharacterY(int y) { m_Y = y; }

	void AddAnimationCallback(CharacterState charState, std::string callbackTrigger, TextureAnimation::TextureAnimationCallback callback)
	{
		if (m_StateAnimation[charState] == nullptr) return;
		m_StateAnimation[charState]->AddAnimationCallback(callbackTrigger, callback);
	}

	void FinishedSwing()
	{
		switch (m_CurrentState)
		{
		case CHARSTATE_SWING_UP:
			m_CurrentState = CHARSTATE_IDLE_UP;
			break;
		case CHARSTATE_SWING_DOWN:
			m_CurrentState = CHARSTATE_IDLE_DOWN;
			break;
		case CHARSTATE_SWING_LEFT:
			m_CurrentState = CHARSTATE_IDLE_LEFT;
			break;
		case CHARSTATE_SWING_RIGHT:
			m_CurrentState = CHARSTATE_IDLE_RIGHT;
			break;
		default:break;
		}
	}

	void Update() const
	{
		m_StateAnimation[m_CurrentState]->Update();
	}

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
	void RenderCharacter(int xOffset, int yOffset) const;

	void HitCheck(int xOffset, int yOffset)
	{
		//  Check if the position collides with the second player and if so, damage them
		auto halfWidth = m_Characters[1]->GetCharacterWidth() / 2;
		auto halfHeight = m_Characters[1]->GetCharacterHeight() / 2;
		if (std::abs(m_Characters[1]->GetCharacterX() + halfWidth - xOffset) > halfWidth) return;
		if (std::abs(m_Characters[1]->GetCharacterY() + halfHeight - yOffset) > halfHeight) return;

		//  Damage the second character
		m_Characters[1]->SetCharacterState(TopDownCharacter::CHARSTATE_IDLE_DOWN);
	}

	void SwingCheck(TopDownCharacter::CharacterState charState, int xOffset, int yOffset, int hitIdentifier)
	{
		switch (charState)
		{
		case TopDownCharacter::CHARSTATE_SWING_UP:
			switch (hitIdentifier)
			{
			case 1: HitCheck(xOffset + 21, yOffset + 15); break;
			case 2: HitCheck(xOffset + 13, yOffset - 4); break;
			case 3: HitCheck(xOffset + 3, yOffset - 5); break;
			case 4: HitCheck(xOffset - 4, yOffset - 5); break;
			case 5: HitCheck(xOffset - 4, yOffset + 1); break;
			default:break;
			}
			break;
		case TopDownCharacter::CHARSTATE_SWING_DOWN:
			switch (hitIdentifier)
			{
			case 1: HitCheck(xOffset + 0, yOffset + 0); break;
			case 2: HitCheck(xOffset + 0, yOffset + 0); break;
			case 3: HitCheck(xOffset + 0, yOffset + 0); break;
			case 4: HitCheck(xOffset + 0, yOffset + 0); break;
			case 5: HitCheck(xOffset + 0, yOffset + 0); break;
			case 6: HitCheck(xOffset + 0, yOffset + 0); break;
			default:break;
			}
			break;
		case TopDownCharacter::CHARSTATE_SWING_LEFT:
			switch (hitIdentifier)
			{
			case 1: HitCheck(xOffset + 0, yOffset + 0); break;
			case 2: HitCheck(xOffset + 0, yOffset + 0); break;
			case 3: HitCheck(xOffset + 0, yOffset + 0); break;
			case 4: HitCheck(xOffset + 0, yOffset + 0); break;
			case 5: HitCheck(xOffset + 0, yOffset + 0); break;
			case 6: HitCheck(xOffset + 0, yOffset + 0); break;
			default:break;
			}
			break;
		case TopDownCharacter::CHARSTATE_SWING_RIGHT:
			switch (hitIdentifier)
			{
			case 1: HitCheck(xOffset + 0, yOffset + 0); break;
			case 2: HitCheck(xOffset + 0, yOffset + 0); break;
			case 3: HitCheck(xOffset + 0, yOffset + 0); break;
			case 4: HitCheck(xOffset + 0, yOffset + 0); break;
			case 5: HitCheck(xOffset + 0, yOffset + 0); break;
			case 6: HitCheck(xOffset + 0, yOffset + 0); break;
			default:break;
			}
			break;
		default:break;
		}
	}

	TopDownCharacter* m_Characters[2];

	enum KeyPress { KEYPRESS_UP, KEYPRESS_DOWN, KEYPRESS_LEFT, KEYPRESS_RIGHT, KEYPRESS_CTRL, KEYPRESS_COUNT };
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

	m_Characters[0]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_SWING_UP, "Assets/Sprites/LinkUpSwing.xml");
	m_Characters[0]->AddAnimationCallback(TopDownCharacter::CHARSTATE_SWING_UP, "Finished", [=](TextureAnimation*) { m_Characters[0]->FinishedSwing(); });
	m_Characters[0]->AddAnimationCallback(TopDownCharacter::CHARSTATE_SWING_UP, "UpSwingHit1", [=](TextureAnimation*) { SwingCheck(m_Characters[0]->GetCurrentState(), m_Characters[0]->GetCharacterX(), m_Characters[0]->GetCharacterY(), 1); });
	m_Characters[0]->AddAnimationCallback(TopDownCharacter::CHARSTATE_SWING_UP, "UpSwingHit2", [=](TextureAnimation*) { SwingCheck(m_Characters[0]->GetCurrentState(), m_Characters[0]->GetCharacterX(), m_Characters[0]->GetCharacterY(), 2); });
	m_Characters[0]->AddAnimationCallback(TopDownCharacter::CHARSTATE_SWING_UP, "UpSwingHit3", [=](TextureAnimation*) { SwingCheck(m_Characters[0]->GetCurrentState(), m_Characters[0]->GetCharacterX(), m_Characters[0]->GetCharacterY(), 3); });
	m_Characters[0]->AddAnimationCallback(TopDownCharacter::CHARSTATE_SWING_UP, "UpSwingHit4", [=](TextureAnimation*) { SwingCheck(m_Characters[0]->GetCurrentState(), m_Characters[0]->GetCharacterX(), m_Characters[0]->GetCharacterY(), 4); });
	m_Characters[0]->AddAnimationCallback(TopDownCharacter::CHARSTATE_SWING_UP, "UpSwingHit5", [=](TextureAnimation*) { SwingCheck(m_Characters[0]->GetCurrentState(), m_Characters[0]->GetCharacterX(), m_Characters[0]->GetCharacterY(), 5); });
	m_Characters[0]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_SWING_DOWN, "Assets/Sprites/LinkDownSwing.xml");
	m_Characters[0]->AddAnimationCallback(TopDownCharacter::CHARSTATE_SWING_DOWN, "Finished", [=](TextureAnimation*) { m_Characters[0]->FinishedSwing(); });
	m_Characters[0]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_SWING_LEFT, "Assets/Sprites/LinkLeftSwing.xml");
	m_Characters[0]->AddAnimationCallback(TopDownCharacter::CHARSTATE_SWING_LEFT, "Finished", [=](TextureAnimation*) { m_Characters[0]->FinishedSwing(); });
	m_Characters[0]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_SWING_RIGHT, "Assets/Sprites/LinkRightSwing.xml");
	m_Characters[0]->AddAnimationCallback(TopDownCharacter::CHARSTATE_SWING_RIGHT, "Finished", [=](TextureAnimation*) { m_Characters[0]->FinishedSwing(); });

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
	m_Characters[1]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_SWING_UP, "Assets/Sprites/GuardUpWalk.xml");
	m_Characters[1]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_SWING_DOWN, "Assets/Sprites/GuardDownWalk.xml");
	m_Characters[1]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_SWING_LEFT, "Assets/Sprites/GuardLeftWalk.xml");
	m_Characters[1]->LoadCharacterAnimation(TopDownCharacter::CHARSTATE_SWING_RIGHT, "Assets/Sprites/GuardRightWalk.xml");
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
	m_Pressed[KEYPRESS_CTRL] = inputManager.GetKeyDown(SDL_SCANCODE_LCTRL) | inputManager.GetKeyDown(SDL_SCANCODE_RCTRL);

	if (m_Pressed[KEYPRESS_UP] && m_Pressed[KEYPRESS_DOWN]) { m_Pressed[KEYPRESS_UP] = false; m_Pressed[KEYPRESS_DOWN] = false; }
	if (m_Pressed[KEYPRESS_LEFT] && m_Pressed[KEYPRESS_RIGHT]) { m_Pressed[KEYPRESS_LEFT] = false; m_Pressed[KEYPRESS_RIGHT] = false; }

	auto currentState = m_Characters[0]->GetCurrentState();
	if (!m_Characters[0]->IsCharacterSwinging())
	{
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
			default:break;
			}
		}

		if (m_Pressed[KEYPRESS_CTRL])
		{
			switch (currentState)
			{
			case TopDownCharacter::CHARSTATE_WALK_UP:
			case TopDownCharacter::CHARSTATE_IDLE_UP:
				currentState = TopDownCharacter::CHARSTATE_SWING_UP;
				break;
			case TopDownCharacter::CHARSTATE_WALK_DOWN:
			case TopDownCharacter::CHARSTATE_IDLE_DOWN:
				currentState = TopDownCharacter::CHARSTATE_SWING_DOWN;
				break;
			case TopDownCharacter::CHARSTATE_WALK_LEFT:
			case TopDownCharacter::CHARSTATE_IDLE_LEFT:
				currentState = TopDownCharacter::CHARSTATE_SWING_LEFT;
				break;
			case TopDownCharacter::CHARSTATE_WALK_RIGHT:
			case TopDownCharacter::CHARSTATE_IDLE_RIGHT:
				currentState = TopDownCharacter::CHARSTATE_SWING_RIGHT;
				break;
			default:break;
			}
		}
	}

	m_Characters[0]->SetCharacterState(currentState);
	//m_Characters[1]->SetCharacterState(currentState);
}

inline void TopDownExample::UpdateCharacter()
{
	auto x = m_Characters[0]->GetCharacterX();
	auto y = m_Characters[0]->GetCharacterY();
	if (m_Pressed[KEYPRESS_UP]) y -= int(frameSeconds * 110.0f);
	if (m_Pressed[KEYPRESS_DOWN]) y += int(frameSeconds * 110.0f);
	if (m_Pressed[KEYPRESS_LEFT]) x -= int(frameSeconds * 110.0f);
	if (m_Pressed[KEYPRESS_RIGHT]) x += int(frameSeconds * 110.0f);

	m_Characters[0]->SetCharacterX(x);
	m_Characters[0]->SetCharacterY(y);
	if (m_Characters[0] != nullptr) m_Characters[0]->Update();

	//m_Characters[1]->SetCharacterX(x + 100);
	//m_Characters[1]->SetCharacterY(y - 4);
	//if (m_Characters[1] != nullptr) m_Characters[1]->Update();
}

inline void TopDownExample::RenderCharacter(int xOffset, int yOffset) const
{
	if (m_Characters[0] != nullptr) m_Characters[0]->Render(xOffset, yOffset);
	if (m_Characters[1] != nullptr) m_Characters[1]->Render(xOffset, yOffset);
}