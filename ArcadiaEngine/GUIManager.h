#pragma once

#include "SDL2/SDL_opengl.h"
#pragma comment(lib, "opengl32.lib")

#include <deque>
#include <tuple>

class GUIObjectNode
{
private:
	std::deque<GUIObjectNode*> m_Children;

public:
	GUIObjectNode();
	virtual void Input();
	virtual void Render();

	void AddChild(GUIObjectNode* newChild) { m_Children.push_back(newChild); }

	int m_ZOrder;
	int m_X;
	int m_Y;
	int m_Width;
	int m_Height;
	GLuint m_TextureID;
};

class GUIButton : public GUIObjectNode
{
public:
	virtual void Input();
};

class GUIManager
{
	friend class GUIButton;
protected:
	enum MouseButtonID
	{
		MOUSE_BUTTON_LEFT,
		MOUSE_BUTTON_MIDDLE,
		MOUSE_BUTTON_RIGHT
	};

	const int& mouseX;
	const int& mouseY;
	const bool& mouseButtonLeft;
	const bool& mouseButtonMiddle;
	const bool& mouseButtonRight;
	const Uint8*& currentKeyStates;
public:
	static GUIManager& GetInstance(const int& mouse_x, const int& mouse_y, const bool& mouse_button_left, const bool& mouse_button_middle, const bool& mouse_button_right, const Uint8*& current_key_states) { static GUIManager INSTANCE(mouse_x, mouse_y, mouse_button_left, mouse_button_middle, mouse_button_right, current_key_states); return INSTANCE; }

	GUIObjectNode* GetBaseNode() { return &m_BaseNode; }
	void Input();
	void Render();

private:
	GUIManager(const int& mouse_x, const int& mouse_y, const bool& mouse_button_left, const bool& mouse_button_middle, const bool& mouse_button_right, const Uint8*& current_key_states);
	~GUIManager();

	GUIObjectNode m_BaseNode;
};

GUIObjectNode::GUIObjectNode() :
	m_ZOrder(0),
	m_X(0),
	m_Y(0),
	m_Width(0),
	m_Height(0),
	m_TextureID(-1)
{

}

void GUIObjectNode::Input()
{
	//  TODO: Take Input

	for (auto iter = m_Children.begin(); iter != m_Children.end(); ++iter) (*iter)->Input();
}

void GUIObjectNode::Render()
{
	//  Render the object if able
	if (m_TextureID >= 0 && m_Width > 0 && m_Height > 0)
	{
		glBindTexture(GL_TEXTURE_2D, m_TextureID);

		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3i(m_X, m_Y, 0);
		glTexCoord2f(1.0f, 0.0f); glVertex3i(m_X + m_Width, m_Y, 0);
		glTexCoord2f(1.0f, 1.0f); glVertex3i(m_X + m_Width, m_Y + m_Height, 0);
		glTexCoord2f(0.0f, 1.0f); glVertex3i(m_X, m_Y + m_Height, 0);
		glEnd();
	}

	for (auto iter = m_Children.begin(); iter != m_Children.end(); ++iter) (*iter)->Render();
}

void GUIButton::Input()
{
	int x = GUIManager::GetInstance().mouseX;
}

void GUIManager::Input()
{
	currentKeyStates = SDL_GetKeyboardState(NULL);
	m_BaseNode.Input();
}

void GUIManager::Render()
{
	m_BaseNode.Render();
}

GUIManager::GUIManager(const int& mouse_x, const int& mouse_y, const bool& mouse_button_left, const bool& mouse_button_middle, const bool& mouse_button_right, const Uint8*& current_key_states) : 
	mouseX(mouse_x),
	mouseY(mouse_y),
	mouseButtonLeft(mouse_button_left),
	mouseButtonMiddle(mouse_button_middle),
	mouseButtonRight(mouse_button_right),
	currentKeyStates(current_key_states)

{

}

GUIManager::~GUIManager()
{

}

