#pragma once

#include "Engine/ArcadiaEngine.h"

#include "Engine/GLMCamera.h"
#include "Engine/Shader.h"
#include "Engine/Program.h"

#include "Engine/GUIMoveable.h"
#include "Engine/GUIDropDown.h"
#include "Engine/GUILabel.h"
#include "Engine/GUIButton.h"

#include "UIShowcase.h"
#include "MemoryShowcase.h"
#include "SoundShowcase.h"
#include "TopDownExample.h"
#include "WinsockShowcase.h"
#include "World3DExample.h"

GUIObjectNode* currentDialogue;

void CreateTestData()
{
	//  Load some basic fonts
	fontManager.SetFontFolder("Assets/Fonts/");
	fontManager.LoadFont("Arial");
	fontManager.LoadFont("Arial-12-White");

	//  Set the font on the Debug Console
	debugConsole->SetFont(fontManager.GetFont("Arial-12-White"));

	//  Create the first test dialogue and add it to the scene
	currentDialogue = new World3DExample;
	guiManager.GetBaseNode()->AddChild(currentDialogue);

	//  Create the container that holds the showcase choice drop-down and button
	auto showcaseChoiceContainer = GUIMoveable::CreateTemplatedMoveable("Standard", 744, -16, 280, 140, 0, 0, 0, 0);
	showcaseChoiceContainer->SetMoveable(false);
	guiManager.AddChild(showcaseChoiceContainer);

	//  Create the showcase choice introduction label
	auto showcaseChoiceIntroductionLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Choose which showcase to display.", 10, 60, 260, 22);
	showcaseChoiceContainer->AddChild(showcaseChoiceIntroductionLabel);

	//  Create the drop-down that allows us to pick the next dialogue to display
	auto showcaseDropdown = GUIDropDown::CreateTemplatedDropDown("Standard", 10, 26, 200, 24, 180, 4, 16, 16);
	showcaseDropdown->SetObjectName("DialogueDropdown");
	showcaseDropdown->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "3D World Example", 10, 4, 160, 20));
	showcaseDropdown->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "GUI Manager", 10, 4, 160, 20));
	showcaseDropdown->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Sound Wrapper", 10, 4, 160, 20));
	showcaseDropdown->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Memory Manager", 10, 4, 160, 20));
	showcaseDropdown->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "TopDown Example", 10, 4, 160, 20));
	showcaseChoiceContainer->AddChild(showcaseDropdown);

	//  Create the button that allows us to move to a new dialogue based on the drop-down setting
	auto showcaseGoButton = GUIButton::CreateTemplatedButton("Standard", 220, 26, 50, 24);
	showcaseGoButton->SetObjectName("DialogueGoButton");
	showcaseGoButton->SetFont(fontManager.GetFont("Arial"));
	showcaseGoButton->SetText("Go");
	showcaseGoButton->SetLeftClickCallback([=](GUIObjectNode*)
	{
		debugConsole->SetVisible(false);

		currentDialogue->SetToDestroy(guiManager.GetDestroyList());
		switch (showcaseDropdown->GetSelectedIndex())
		{
		case 0:			currentDialogue = new World3DExample;				break;
		case 1:			currentDialogue = new UIShowcaseDialogue;			break;
		case 2:			currentDialogue = new SoundShowcaseDialogue;		break;
		case 3:			currentDialogue = new MemoryShowcaseDialogue;		break;
		case 4:			currentDialogue = new TopDownExample;				break;
		default:break;
		}
		guiManager.GetBaseNode()->AddChild(currentDialogue);
	});
	showcaseChoiceContainer->AddChild(showcaseGoButton);

	//autoplayManager.LoadAutoplayScript("Assets/TestMenuAutoplay.xml");
}

void AppMain()
{
	if (!InitializeEngine())
	{
		ShutdownEngine();
		throw std::runtime_error("InitializeEngine failed");
	}

	//  Create test data for different systems to ensure they work as they should
	CreateTestData();

	//  Load the Primary Loop
	PrimaryLoop();

	//  Free resources and close SDL before exiting
	ShutdownEngine();
}