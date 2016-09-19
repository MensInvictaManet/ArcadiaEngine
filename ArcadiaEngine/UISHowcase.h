#pragma once

#include "Engine/GUIObjectNode.h"
#include "Engine/GUIListBox.h"
#include "Engine/GUIMoveable.h"
#include "Engine/GUILabel.h"
#include "Engine/GUICheckbox.h"
#include "Engine/GUIButton.h"
#include "Engine/GUIDropDown.h"

class UIShowcaseDialogue : public GUIObjectNode
{
public:
	UIShowcaseDialogue();
	~UIShowcaseDialogue() {}
};

inline UIShowcaseDialogue::UIShowcaseDialogue()
{
	auto introductionLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "This is a basic showcase of the GUI Manager system.", 10, 10, 300, 32);
	AddChild(introductionLabel);

	auto container1 = GUIMoveable::CreateTemplatedMoveable("Standard", 10, 40, 600, 400, 0, 0, 400, 16);
	AddChild(container1);

	auto containerExplanationLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "This is a basic container. Click and drag it with the top bar.", 10, 24, 600, 22);
	container1->AddChild(containerExplanationLabel);

	auto callbackLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "This text appears and disappears with a button callback.", 10, 100, 300, 32);
	container1->AddChild(callbackLabel);

	auto listboxLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "This is a basic listbox that you can throw GUI object into.", 10, 130, 300, 32);
	container1->AddChild(listboxLabel);

	auto listbox1 = GUIListBox::CreateTemplatedListBox("Standard", 10, 150, 580, 240, 560, 4, 16, 16, 16, 16, 16, 22, 2);
	container1->AddChild(listbox1);

	auto checkbox1 = GUICheckbox::CreateTemplatedCheckbox("Standard", 140, 60, 20, 20);
	checkbox1->SetCheckCallback([=](GUIObjectNode*)
	{
		listbox1->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), checkbox1->GetChecked() ? "Checkbox CHECKED" : "Checkbox UNCHECKED", 10, 6, 300, 22));

		callbackLabel->SetText("Checkbox callbacks work, and this one locks the text to visible.");
		callbackLabel->SetVisible(checkbox1->GetChecked());
	});
	container1->AddChild(checkbox1);

	auto button1 = GUIButton::CreateTemplatedButton("Standard", 10, 50, 120, 40);
	button1->SetFont(fontManager.GetFont("Arial"));
	button1->SetText("Click Me");
	button1->SetLeftClickCallback([=](GUIObjectNode*)
	{
		listbox1->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Button clicked (left)", 10, 6, 300, 22));

		if (checkbox1->GetChecked()) return;
		callbackLabel->SetText("This text appears and disappears with a callback.");
		callbackLabel->SetVisible(!callbackLabel->GetVisible());
	});
	button1->SetRightClickCallback([=](GUIObjectNode*)
	{
		listbox1->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Button clicked (right)", 10, 6, 300, 22));

		if (checkbox1->GetChecked()) return;
		callbackLabel->SetText("Right click callbacks also exist (and middle!).");
		callbackLabel->SetVisible(!callbackLabel->GetVisible());
	});
	container1->AddChild(button1);

	auto showcaseDropdown = GUIDropDown::CreateTemplatedDropDown("Standard", 754, 10, 200, 24, 180, 4, 16, 16);
	showcaseDropdown->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "GUI Manager", 10, 4, 160, 20));
	showcaseDropdown->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Sound Wrapper", 10, 4, 160, 20));
	showcaseDropdown->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Memory Manager", 10, 4, 160, 20));
	AddChild(showcaseDropdown);

	auto showcaseGoButton = GUIButton::CreateTemplatedButton("Standard", 964, 10, 50, 24);
	showcaseGoButton->SetFont(fontManager.GetFont("Arial"));
	showcaseGoButton->SetText("Go");
	showcaseGoButton->SetLeftClickCallback([=](GUIObjectNode*)
	{
		switch (showcaseDropdown->GetSelectedIndex())
		{
		case 0: //  GUI Manager Showcase
			guiManager.GetBaseNode()->AddChild(new UIShowcaseDialogue);
			break;
		case 1: //  Sound Wrapper Showcase
			break;
			
		case 2: //  Memory Manager Showcase
			break;
		}
		SetToDestroy(guiManager.GetDestroyList());
	});
	AddChild(showcaseGoButton);
}