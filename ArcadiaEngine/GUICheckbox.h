#pragma once

#include "GUIObjectNode.h"
#include "InputManager.h"

#include <functional>

class GUICheckbox : public GUIObjectNode
{
public:
	typedef std::function<void(GUIObjectNode*)> GUICheckboxCallback;

	static GUICheckbox* CreateCheckbox(const char* imageFile, const char* checkFile, int x = 0, int y = 0, int w = 0, int h = 0);

	GUICheckbox();
	virtual ~GUICheckbox();

	bool GetChecked() const { return m_Checked; }

	void SetChecked(bool checked);
	void SetCheckTextureID(int textureID) { m_CheckTextureID = textureID; }
	void SetCheckCallback(const GUICheckboxCallback& callback) { m_CheckCallback = callback; }

	void Input(int xOffset = 0, int yOffset = 0) override;
	void Render(int xOffset = 0, int yOffset = 0) override;

private:
	void ToggleCheck();
	void Check();
	void Uncheck();

	GLuint m_CheckTextureID;
	GUICheckboxCallback	m_CheckCallback;
	bool m_Checked;
};

inline GUICheckbox* GUICheckbox::CreateCheckbox(const char* imageFile, const char* checkFile, int x, int y, int w, int h)
{
	auto newButton = new GUICheckbox;
	newButton->SetTextureID(textureManager.LoadTextureGetID(imageFile));
	newButton->SetCheckTextureID(textureManager.LoadTextureGetID(checkFile));
	newButton->SetX(x);
	newButton->SetY(y);
	newButton->SetWidth(w);
	newButton->SetHeight(h);
	return newButton;
}

inline GUICheckbox::GUICheckbox() :
	m_CheckTextureID(-1),
	m_CheckCallback(nullptr),
	m_Checked(false)
{

}


inline GUICheckbox::~GUICheckbox()
{

}


inline void GUICheckbox::SetChecked(bool checked)
{
	if (checked == m_Checked) return;
	else (checked ? Check() : Uncheck());
}


inline void GUICheckbox::Input(int xOffset, int yOffset)
{
	auto leftButtonState = inputManager.GetMouseButtonLeft();
	auto middleButtonState = inputManager.GetMouseButtonMiddle();
	auto rightButtonState = inputManager.GetMouseButtonRight();
	auto x = inputManager.GetMouseX();
	auto y = inputManager.GetMouseY();

	if ((x > xOffset + m_X) && (x < xOffset + m_X + m_Width) && (y > yOffset + m_Y) && (y < yOffset + m_Y + m_Height))
	{
		if (leftButtonState == MOUSE_BUTTON_PRESSED) ToggleCheck();
	}

	//  Take base node input
	GUIObjectNode::Input(xOffset, yOffset);
}

inline void GUICheckbox::Render(int xOffset, int yOffset)
{
	//  Render the object if we're able
	if (!m_SetToDestroy && m_Visible && m_TextureID >= 0 && m_Width > 0 && m_Height > 0)
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3i(xOffset + m_X, yOffset + m_Y, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3i(xOffset + m_X + m_Width, yOffset + m_Y, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex3i(xOffset + m_X + m_Width, yOffset + m_Y + m_Height, 0);
		glTexCoord2f(0.0f, 1.0f); glVertex3i(xOffset + m_X, yOffset + m_Y + m_Height, 0);
		glEnd();

		if (m_Checked)
		{
			glBindTexture(GL_TEXTURE_2D, m_CheckTextureID);

			glBegin(GL_QUADS);
			glTexCoord2f(0.0f, 0.0f); glVertex3i(xOffset + m_X, yOffset + m_Y, 0);
			glTexCoord2f(1.0f, 0.0f); glVertex3i(xOffset + m_X + m_Width, yOffset + m_Y, 0);
			glTexCoord2f(1.0f, 1.0f); glVertex3i(xOffset + m_X + m_Width, yOffset + m_Y + m_Height, 0);
			glTexCoord2f(0.0f, 1.0f); glVertex3i(xOffset + m_X, yOffset + m_Y + m_Height, 0);
			glEnd();
		}
	}

	//  Pass the render call to all children
	for (auto iter = m_Children.begin(); iter != m_Children.end(); ++iter) (*iter)->Render(xOffset + m_X, yOffset + m_Y);
}

inline void GUICheckbox::ToggleCheck()
{
	m_Checked ? Uncheck() : Check();
}

inline void GUICheckbox::Check()
{
	m_Checked = true;
	m_CheckCallback(this);
}

inline void GUICheckbox::Uncheck()
{
	m_Checked = false;
	m_CheckCallback(this);
}