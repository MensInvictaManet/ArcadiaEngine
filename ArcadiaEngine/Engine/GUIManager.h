#pragma once

#include "GUIObjectNode.h"
#include <map>

class GUIManager
{
public:
	static GUIManager& GetInstance() { static GUIManager INSTANCE; return INSTANCE; }

	GUIObjectNode* GetBaseNode() { return &m_BaseNode; }

	void DestroyNode(GUIObjectNode* nodeToDestroy);
	void Input();
	void Update();
	void Render();
	void Render3D();
	void EndStep();
	void Shutdown();

private:
	GUIManager();
	~GUIManager();

	GUIObjectNode m_BaseNode;
	std::map<GUIObjectNode*, bool> m_NodesToDestroy;
};

inline void GUIManager::DestroyNode(GUIObjectNode* nodeToDestroy)
{
	if (nodeToDestroy->m_SetToDestroy == true) return;

	nodeToDestroy->SetToDestroy(m_NodesToDestroy);
}

inline void GUIManager::Input()
{
	m_BaseNode.Input();
}

inline void GUIManager::Update()
{
	m_BaseNode.Update();
}

inline void GUIManager::Render()
{
	m_BaseNode.Render();
}

inline void GUIManager::Render3D()
{
	m_BaseNode.Render3D();
}

inline GUIManager::GUIManager()
{

}

inline GUIManager::~GUIManager()
{

}

inline void GUIManager::EndStep()
{
	for (auto iter = m_NodesToDestroy.begin(); iter != m_NodesToDestroy.end(); ++iter)
	{
		(*iter).first->Destroy();
		delete (*iter).first;
	}

	m_NodesToDestroy.clear();
}

inline void GUIManager::Shutdown()
{
	DestroyNode(&m_BaseNode);
	m_NodesToDestroy.erase(m_NodesToDestroy.find(&m_BaseNode));
	m_BaseNode.m_SetToDestroy = false;
	EndStep();
}

//  Instance to be utilized by anyone including this header
GUIManager& guiManager = GUIManager::GetInstance();