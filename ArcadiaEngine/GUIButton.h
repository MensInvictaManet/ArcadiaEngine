#pragma once

#include "GUIObjectNode.h"
#include "InputManager.h"

#include <functional>

class GUIButton : public GUIObjectNode
{
public:
	typedef std::function<void(GUIObjectNode*)> GUIButtonCallback;

	static GUIButton* CreateButton(const char* imageFile, int x = 0, int y = 0, int w = 0, int h = 0);

	GUIButton();
	virtual ~GUIButton();

	void SetLeftClickCallback(const GUIButtonCallback& callback) { m_LeftClickCallback = callback; }
	void SetMiddleClickCallback(const GUIButtonCallback& callback) { m_MiddleClickCallback = callback; }
	void SetRightClickCallback(const GUIButtonCallback& callback) { m_RightClickCallback = callback; }
	
	void Input() override;
	void Render() override;

private:
	GUIButtonCallback	m_LeftClickCallback;
	GUIButtonCallback	m_MiddleClickCallback;
	GUIButtonCallback	m_RightClickCallback;
	bool m_Pressed;
};

inline GUIButton* GUIButton::CreateButton(const char* imageFile, int x, int y, int w, int h)
{
	auto newButton = new GUIButton;
	newButton->SetTextureID(textureManager.LoadTextureGetID(imageFile));
	newButton->SetX(x);
	newButton->SetY(y);
	newButton->SetWidth(w);
	newButton->SetHeight(h);
	return newButton;
}

inline GUIButton::GUIButton() : 
	m_LeftClickCallback(nullptr),
	m_MiddleClickCallback(nullptr),
	m_RightClickCallback(nullptr),
	m_Pressed(false)
{
	
}


inline GUIButton::~GUIButton()
{
	
}


inline void GUIButton::Input()
{
	auto leftButtonState = inputManager.GetMouseButtonLeft();
	auto middleButtonState = inputManager.GetMouseButtonMiddle();
	auto rightButtonState = inputManager.GetMouseButtonRight();
	auto x = inputManager.GetMouseX();
	auto y = inputManager.GetMouseY();

	if (leftButtonState == MOUSE_BUTTON_UNPRESSED) m_Pressed = false;
	if ((x > m_X) && (x < m_X + m_Width) && (y > m_Y) && (y < m_Y + m_Height))
	{
		if (leftButtonState == MOUSE_BUTTON_PRESSED)  m_LeftClickCallback(this);
		if (middleButtonState == MOUSE_BUTTON_PRESSED) m_MiddleClickCallback(this);
		if (rightButtonState == MOUSE_BUTTON_PRESSED) m_RightClickCallback(this);

		if (leftButtonState == MOUSE_BUTTON_PRESSED) m_Pressed = true;
	}
	else m_Pressed = false;

	//  Take base node input
	GUIObjectNode::Input();
}

inline void GUIButton::Render()
{
	//  Render the object if we're able
	if (!m_SetToDestroy && m_Visible && m_TextureID >= 0 && m_Width > 0 && m_Height > 0)
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		auto pressedWidthDelta = m_Pressed ? int(m_Width * 0.05f) : 0;
		auto pressedHeightDelta = m_Pressed ? int(m_Height * 0.05f) : 0;

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3i(m_X + pressedWidthDelta, m_Y + pressedHeightDelta, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3i(m_X + m_Width - pressedWidthDelta, m_Y + pressedHeightDelta, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex3i(m_X + m_Width - pressedWidthDelta, m_Y + m_Height - pressedHeightDelta, 0);
		glTexCoord2f(0.0f, 1.0f); glVertex3i(m_X + pressedWidthDelta, m_Y + m_Height - pressedHeightDelta, 0);
		glEnd();
	}

	//  Pass the render call to all children
	for (auto iter = m_Children.begin(); iter != m_Children.end(); ++iter) (*iter)->Render();
}