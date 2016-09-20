#pragma once

#include "Engine/GUIObjectNode.h"
#include "Engine/GUIListBox.h"
#include "Engine/GUIMoveable.h"
#include "Engine/GUILabel.h"
#include "Engine/GUICheckbox.h"
#include "Engine/GUIEditBox.h"
#include "Engine/GUIButton.h"
#include "Engine/GUIDropDown.h"
#include "Engine/SoundWrapper.h"
#include "Engine/WinsockWrapper.h"

class WinsockShowcaseDialogue : public GUIObjectNode
{
public:
	WinsockShowcaseDialogue();
	~WinsockShowcaseDialogue();

	void Input(int xOffset = 0, int yOffset = 0) override;
	void Update() override;
	void Render(int xOffset = 0, int yOffset = 0) override;
	void Render3D() override;

private:
	void UpdateMemoryListBox();

	bool m_UseOggFiles;
	GUIListBox* m_MemoryDataListBox;
	int m_CoinSoundID[4];
	int m_BackgroundMusicID;
	bool m_Display3DTest;
};

inline WinsockShowcaseDialogue::WinsockShowcaseDialogue() :
	m_UseOggFiles(false),
	m_MemoryDataListBox(nullptr),
	m_Display3DTest(false)
{
	m_CoinSoundID[0] = soundWrapper.loadSoundFile("Assets/CoinPickup1.ogg", -1);
	m_CoinSoundID[1] = soundWrapper.loadSoundFile("Assets/CoinPickup2.ogg", -1);
	m_CoinSoundID[2] = soundWrapper.loadSoundFile("Assets/CoinPickup1.wav", -1);
	m_CoinSoundID[3] = soundWrapper.loadSoundFile("Assets/CoinPickup2.wav", -1);
	soundWrapper.playMusicFile(m_BackgroundMusicID = soundWrapper.loadMusicFile("Assets/BackgroundMusic.ogg", -1));

	m_MemoryDataListBox = GUIListBox::CreateTemplatedListBox("Standard", 320, 60, 640, 120, 620, 6, 16, 16, 16, 16, 16, 22, 2);
	m_MemoryDataListBox->SetSelectable(false);
	AddChild(m_MemoryDataListBox);

	auto moveable = GUIMoveable::CreateMoveable("Assets/ContainerTest.png", 60, 60, 240, 240, 0, 0, 256, 25);
	AddChild(moveable);

	auto label7 = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "", 10, 6, 200, 30);
	label7->SetText("Move This UI Around");
	moveable->AddChild(label7);

	auto checkbox1 = GUICheckbox::CreateCheckbox("Assets/CheckboxTest1.png", "Assets/CheckboxTest2.png", 20, 40, 50, 50);
	checkbox1->SetCheckCallback([=](GUIObjectNode* node)
	{
		m_UseOggFiles = checkbox1->GetChecked();
	});
	moveable->AddChild(checkbox1);

	auto checkbox2 = GUICheckbox::CreateTemplatedCheckbox("Standard", 20, 100, 50, 50);
	checkbox2->SetCheckCallback([=](GUIObjectNode* node)
	{
		m_UseOggFiles = checkbox2->GetChecked();
	});
	moveable->AddChild(checkbox2);

	auto button1 = GUIButton::CreateButton("Assets/ButtonTest1.png", 120, 40, 100, 50);
	button1->SetFont(fontManager.GetFont("Arial"));
	button1->SetText("Sound 1");
	button1->SetLeftClickCallback([=](GUIObjectNode* node)
	{
		soundWrapper.playSoundFile(m_CoinSoundID[m_UseOggFiles ? 0 : 2]);
	});
	moveable->AddChild(button1);

	auto button2 = GUIButton::CreateTemplatedButton("Standard", 120, 100, 100, 50);
	button2->SetFont(fontManager.GetFont("Arial-12-White"));
	button2->SetText("Sound 2");
	button2->SetLeftClickCallback([=](GUIObjectNode* node)
	{
		soundWrapper.playSoundFile(m_CoinSoundID[m_UseOggFiles ? 1 : 3]);
	});
	moveable->AddChild(button2);

	auto editbox1 = GUIEditBox::CreateTemplatedEditBox("Standard", 20, 160, 200, 30);
	editbox1->SetFont(fontManager.GetFont("Arial"));
	editbox1->SetText("EDIT THIS TEXT");
	moveable->AddChild(editbox1);

	auto dropDown1 = GUIDropDown::CreateTemplatedDropDown("Standard", 20, 400, 200, 32, 176, 8, 16, 16);
	dropDown1->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Dropdown 1", 10, 8, 80, 22));
	dropDown1->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Dropdown 2", 10, 8, 80, 22));
	dropDown1->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Dropdown 3", 10, 8, 80, 22));
	AddChild(dropDown1);

	//  Test the Winsock Wrapper
	auto googleIP = Socket::GetHostIP("www.google.com");
	auto ipAddress = winsockWrapper.ConvertIPtoUINT(googleIP.c_str());
	auto ipString = winsockWrapper.ConvertUINTtoIP(ipAddress);
}

inline WinsockShowcaseDialogue::~WinsockShowcaseDialogue()
{

}

inline void WinsockShowcaseDialogue::Input(int xOffset, int yOffset)
{
	// Set a flag to render a 3D object if we're holding the DOWN key
	m_Display3DTest = inputManager.GetKeyDown(SDL_SCANCODE_DOWN);

	GUIObjectNode::Input(xOffset, yOffset);
}

inline void WinsockShowcaseDialogue::Update()
{
	//  Update the memory list box based on data from the memory manager
	UpdateMemoryListBox();

	GUIObjectNode::Update();
}

inline void WinsockShowcaseDialogue::Render(int xOffset, int yOffset)
{
	GUIObjectNode::Render(xOffset, yOffset);
}

inline void WinsockShowcaseDialogue::Render3D()
{
	//  Render the 3D quad if the bool is set to true
	if (m_Display3DTest)
	{
		glBegin(GL_QUADS);
		glVertex3f(-5.7f, -1.5f, -50.0f);
		glVertex3f(-4.3f, -1.5f, -50.0f);
		glVertex3f(-4.6f, -0.5f, -50.0f);
		glVertex3f(-5.4f, -0.5f, -50.0f);
		glEnd();
	}

	GUIObjectNode::Render3D();
}

inline void WinsockShowcaseDialogue::UpdateMemoryListBox()
{
	if (m_MemoryDataListBox == nullptr) return;
	if (MEMORY_MANAGER_ACTIVE == false) return;

	static unsigned int memoryPoolCount = 0;
	auto currentMemoryPoolCount = memoryManager.GetMemoryPoolCount();

	//  Make sure we add to the amount of Labels until we have enough to show all of the current memory pools
	while (memoryPoolCount < currentMemoryPoolCount)
	{
		auto newLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "", 10, 4, 300, 22);
		m_MemoryDataListBox->AddItem(newLabel);
		memoryPoolCount++;
	}

	for (unsigned int i = 0; i < memoryPoolCount; ++i)
	{
		m_MemoryDataListBox->SetSelectedIndex(i);
		auto label = static_cast<GUILabel*>(m_MemoryDataListBox->GetSelectedItem());

		char amountStringBuffer[32];
		_itoa_s(memoryManager.GetMemoryPoolAmountAtIndex(i), amountStringBuffer, 10);
		label->SetText(std::string(memoryManager.GetMemoryPoolNameAtIndex(i) + std::string(":  ") + std::string(amountStringBuffer)));
	}
}