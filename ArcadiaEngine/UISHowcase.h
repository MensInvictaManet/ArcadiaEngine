#pragma once

#include "Engine/GUIManager.h"
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
	//  Create the label that acts as an explanation of the current showcase UI
	auto introductionLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "This is a basic showcase of the GUI Manager system.", 10, 10, 300, 32);
	AddChild(introductionLabel);

	//  Create the container that holds all of the main objects in the scene
	auto container1 = GUIMoveable::CreateTemplatedMoveable("Standard", 10, 40, 600, 400, 0, 0, 400, 16);
	AddChild(container1);

	//  Create the label that acts as an explanation of the container / moveable
	auto containerExplanationLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "This is a basic container. Click and drag it with the top bar.", 10, 24, 600, 22);
	container1->AddChild(containerExplanationLabel);

	//  Create the label that our checkbox and button will manipulate
	auto callbackLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "This text appears and disappears with a button callback.", 10, 100, 300, 32);
	container1->AddChild(callbackLabel);

	//  Create the label that acts as an explanation of the listbox
	auto listboxLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "This is a basic listbox that you can throw GUI object into.", 10, 130, 300, 32);
	container1->AddChild(listboxLabel);

	//  Create the listbox that shows actions taken in the UI
	auto listbox1 = GUIListBox::CreateTemplatedListBox("Standard", 10, 150, 580, 240, 560, 4, 16, 16, 16, 16, 16, 22, 2);
	container1->AddChild(listbox1);

	//  Create the checkbox that allows us to disable the button's ability to alter the label
	auto checkbox1 = GUICheckbox::CreateTemplatedCheckbox("Standard", 140, 60, 20, 20);
	checkbox1->SetCheckCallback([=](GUIObjectNode*)
	{
		listbox1->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), checkbox1->GetChecked() ? "Checkbox CHECKED" : "Checkbox UNCHECKED", 10, 6, 300, 22));

		callbackLabel->SetText("Checkbox callbacks work, and this one locks the text to visible.");
		callbackLabel->SetVisible(checkbox1->GetChecked());
	});
	container1->AddChild(checkbox1);

	//  Create the button that allows us to alter a label's visibility and text
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
}