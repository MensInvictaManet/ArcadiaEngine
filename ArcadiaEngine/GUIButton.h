#pragma once

#include "GUIObjectNode.h"
#include "InputManager.h"
#include "FontManager.h"

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
	void SetFont(const Font* font) { m_Font = font; }
	void SetText(const std::string text) { m_Text = text; }

	void Input(int xOffset = 0, int yOffset = 0) override;
	void Render(int xOffset = 0, int yOffset = 0) override;

private:
	GUIButtonCallback	m_LeftClickCallback;
	GUIButtonCallback	m_MiddleClickCallback;
	GUIButtonCallback	m_RightClickCallback;
	bool m_Pressed;
	const Font* m_Font;
	std::string m_Text;
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
	m_Pressed(false),
	m_Font(nullptr),
	m_Text("")
{
	
}


inline GUIButton::~GUIButton()
{
	
}


inline void GUIButton::Input(int xOffset, int yOffset)
{
	auto leftButtonState = inputManager.GetMouseButtonLeft();
	auto middleButtonState = inputManager.GetMouseButtonMiddle();
	auto rightButtonState = inputManager.GetMouseButtonRight();
	auto x = inputManager.GetMouseX();
	auto y = inputManager.GetMouseY();

	if (leftButtonState == MOUSE_BUTTON_UNPRESSED) m_Pressed = false;
	if ((x > xOffset + m_X) && (x < xOffset + m_X + m_Width) && (y > yOffset + m_Y) && (y < yOffset + m_Y + m_Height))
	{
		if (leftButtonState == MOUSE_BUTTON_PRESSED)  m_LeftClickCallback(this);
		if (middleButtonState == MOUSE_BUTTON_PRESSED) m_MiddleClickCallback(this);
		if (rightButtonState == MOUSE_BUTTON_PRESSED) m_RightClickCallback(this);

		if (leftButtonState == MOUSE_BUTTON_PRESSED) m_Pressed = true;
	}
	else m_Pressed = false;

	//  Take base node input
	GUIObjectNode::Input(xOffset, yOffset);
}

inline void GUIButton::Render(int xOffset, int yOffset)
{
	//  Render the object if we're able
	if (!m_SetToDestroy && m_Visible && m_TextureID >= 0 && m_Width > 0 && m_Height > 0)
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		auto pressedWidthDelta = m_Pressed ? int(m_Width * 0.05f) : 0;
		auto pressedHeightDelta = m_Pressed ? int(m_Height * 0.05f) : 0;

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex2i(xOffset + m_X + pressedWidthDelta, yOffset + m_Y + pressedHeightDelta);
		glTexCoord2f(1.0f, 0.0f); glVertex2i(xOffset + m_X + m_Width - pressedWidthDelta, yOffset + m_Y + pressedHeightDelta);
		glTexCoord2f(1.0f, 1.0f); glVertex2i(xOffset + m_X + m_Width - pressedWidthDelta, yOffset + m_Y + m_Height - pressedHeightDelta);
		glTexCoord2f(0.0f, 1.0f); glVertex2i(xOffset + m_X + pressedWidthDelta, yOffset + m_Y + m_Height - pressedHeightDelta);
		glEnd();

		if (m_Font != nullptr && !m_Text.empty())
		{
			m_Font->RenderText(m_Text.c_str(), xOffset + m_X + m_Width / 2, yOffset + m_Y + m_Height / 2, true, true, m_Pressed ? 0.95f : 1.0f, m_Pressed ? 0.95f : 1.0f);
		}
	}

	//  Pass the render call to all children
	for (auto iter = m_Children.begin(); iter != m_Children.end(); ++iter) (*iter)->Render(xOffset + m_X, yOffset + m_Y);
}