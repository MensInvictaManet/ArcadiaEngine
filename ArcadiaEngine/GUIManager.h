#pragma once

#include "SDL2/SDL_opengl.h"
#pragma comment(lib, "opengl32.lib")

#include <deque>
#include <tuple>

#include "InputManager.h"

class GUIObjectNode
{
private:
	std::deque<GUIObjectNode*> m_Children;

public:
	GUIObjectNode();
	virtual ~GUIObjectNode() {}

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
	GUIButton() {}
	virtual ~GUIButton() {}

	virtual void Input() override;
};

class GUIManager
{
public:
	static GUIManager& GetInstance() { static GUIManager INSTANCE; return INSTANCE; }

	GUIObjectNode* GetBaseNode() { return &m_BaseNode; }
	void Input();
	void Render();

private:
	GUIManager();
	~GUIManager();

	GUIObjectNode m_BaseNode;
};

inline GUIObjectNode::GUIObjectNode() :
	m_ZOrder(0),
	m_X(0),
	m_Y(0),
	m_Width(0),
	m_Height(0),
	m_TextureID(-1)
{

}

inline void GUIObjectNode::Input()
{
	//  TODO: Take Input

	for (auto iter = m_Children.begin(); iter != m_Children.end(); ++iter) (*iter)->Input();
}

inline void GUIObjectNode::Render()
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

inline void GUIButton::Input()
{
	auto leftButtonState = inputManager.GetMouseButtonLeft();
	auto x = inputManager.GetMouseX();
	auto y = inputManager.GetMouseY();

	if ((leftButtonState == MOUSE_BUTTON_PRESSED) && (x > m_X) && (x < m_X + m_Width) && (y > m_Y) && (y < m_Y + m_Height))
	{
		m_X++;
	}

	//  Take base node input
	GUIObjectNode::Input();
}

inline void GUIManager::Input()
{
	m_BaseNode.Input();
}

inline void GUIManager::Render()
{
	m_BaseNode.Render();
}

inline GUIManager::GUIManager()
{

}

inline GUIManager::~GUIManager()
{

}

//  Instance to be utilized by anyone including this header
GUIManager& guiManager = GUIManager::GetInstance();