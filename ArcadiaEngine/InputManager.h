#pragma once

#include "SDL2/SDL.h"
#pragma comment(lib, "SDL2/SDL2.lib")

enum MouseButtonState
{
	MOUSE_BUTTON_UNPRESSED,
	MOUSE_BUTTON_PRESSED,
	MOUSE_BUTTON_HELD
};

class InputManager
{
public:
	static InputManager& GetInstance() { static InputManager INSTANCE; return INSTANCE; }

	void GetInputForFrame();

	int GetMouseX() const { return m_MouseX; }
	int GetMouseY() const { return m_MouseY; }
	MouseButtonState GetMouseButtonLeft() const { return m_MouseButtonLeft; }
	MouseButtonState GetMouseButtonMiddle() const { return m_MouseButtonMiddle; }
	MouseButtonState GetMouseButtonRight() const { return m_MouseButtonRight; }
	bool GetKeyDown(const Uint8& sdlKey) const { return (m_KeyStates != nullptr) ? (m_KeyStates[sdlKey] != 0) : false; }

	void SetMouseButtonLeft(bool setting) { if (setting != (m_MouseButtonLeft != MOUSE_BUTTON_UNPRESSED)) m_MouseButtonLeft = (setting ? MOUSE_BUTTON_PRESSED : MOUSE_BUTTON_UNPRESSED); }
	void SetMouseButtonMiddle(bool setting) { if (setting != (m_MouseButtonMiddle != MOUSE_BUTTON_UNPRESSED)) m_MouseButtonMiddle = (setting ? MOUSE_BUTTON_PRESSED : MOUSE_BUTTON_UNPRESSED); }
	void SetMouseButtonRight(bool setting) { if (setting != (m_MouseButtonRight != MOUSE_BUTTON_UNPRESSED)) m_MouseButtonRight = (setting ? MOUSE_BUTTON_PRESSED : MOUSE_BUTTON_UNPRESSED); }

private:
	InputManager();
	~InputManager();

	int m_MouseX;
	int m_MouseY;
	MouseButtonState m_MouseButtonLeft;
	MouseButtonState m_MouseButtonMiddle;
	MouseButtonState m_MouseButtonRight;
	const Uint8* m_KeyStates;
};

inline void InputManager::GetInputForFrame()
{
	SDL_GetMouseState(&m_MouseX, &m_MouseY);
	if (m_MouseButtonLeft == MOUSE_BUTTON_PRESSED) m_MouseButtonLeft = MOUSE_BUTTON_HELD;
	if (m_MouseButtonMiddle == MOUSE_BUTTON_PRESSED) m_MouseButtonMiddle = MOUSE_BUTTON_HELD;
	if (m_MouseButtonRight == MOUSE_BUTTON_PRESSED) m_MouseButtonRight = MOUSE_BUTTON_HELD;
	m_KeyStates = SDL_GetKeyboardState(nullptr);
}


inline InputManager::InputManager() :
	m_MouseX(0),
	m_MouseY(0),
	m_MouseButtonLeft(MOUSE_BUTTON_UNPRESSED),
	m_MouseButtonMiddle(MOUSE_BUTTON_UNPRESSED),
	m_MouseButtonRight(MOUSE_BUTTON_UNPRESSED),
	m_KeyStates(nullptr)
{

}

inline InputManager::~InputManager()
{

}

//  Instance to be utilized by anyone including this header
InputManager& inputManager = InputManager::GetInstance();