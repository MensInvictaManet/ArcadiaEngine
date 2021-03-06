#pragma once

#include "Engine/GUIObjectNode.h"
#include "Engine/GUIListBox.h"
#include "Engine/GUILabel.h"

class MemoryShowcaseDialogue : public GUIObjectNode
{
public:
	MemoryShowcaseDialogue();

	void Update() override;

private:
	void UpdateMemoryListBox();

	GUIListBox* m_MemoryDataListBox;
	unsigned int m_MemoryPoolDisplayCount;
};

inline MemoryShowcaseDialogue::MemoryShowcaseDialogue() :
	m_MemoryDataListBox(nullptr),
	m_MemoryPoolDisplayCount(0)
{
	//  Create the label that acts as an explanation of the current showcase UI
	auto introductionLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "This is a basic showcase of the Memory Manager system.", 10, 10, 300, 32);
	AddChild(introductionLabel);

	//  Create the label that acts as an explanation of the current showcase UI
	auto memoryExplanationLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "The Memory Manager allows you to track memory usage across sections of your code", 10, 40, 300, 32);
	AddChild(memoryExplanationLabel);

	m_MemoryDataListBox = GUIListBox::CreateTemplatedListBox("Standard", 10, 90, 640, 300, 620, 6, 16, 16, 16, 16, 16, 22, 2);
	m_MemoryDataListBox->SetSelectable(false);
	AddChild(m_MemoryDataListBox);

	//  Create shutdownExplanationLabel label that acts as an explanation of the current showcase UI
	auto shutdownExplanationLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "At shutdown, it will let you know if you have a memory leak.", 10, 396, 300, 32);
	AddChild(shutdownExplanationLabel);
}

inline void MemoryShowcaseDialogue::Update()
{
	//  Update the memory list box based on data from the memory manager
	UpdateMemoryListBox();

	GUIObjectNode::Update();
}

inline void MemoryShowcaseDialogue::UpdateMemoryListBox()
{
	if (m_MemoryDataListBox == nullptr) return;

#if MEMORY_MANAGER_ACTIVE
	auto currentMemoryPoolCount = memoryManager.GetMemoryPoolCount();

	//  Make sure we add to the amount of Labels until we have enough to show all of the current memory pools
	while (m_MemoryPoolDisplayCount < currentMemoryPoolCount)
	{
		auto newLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "", 10, 4, 300, 22);
		m_MemoryDataListBox->AddItem(newLabel);
		m_MemoryPoolDisplayCount++;
	}

	for (unsigned int i = 0; i < m_MemoryPoolDisplayCount; ++i)
	{
		m_MemoryDataListBox->SetSelectedIndex(i);
		auto label = static_cast<GUILabel*>(m_MemoryDataListBox->GetSelectedItem());

		char amountStringBuffer[32];
		_itoa_s(memoryManager.GetMemoryPoolAmountAtIndex(i), amountStringBuffer, 10);
		label->SetText(std::string(memoryManager.GetMemoryPoolNameAtIndex(i) + std::string(":  ") + std::string(amountStringBuffer)));
	}
#endif
}