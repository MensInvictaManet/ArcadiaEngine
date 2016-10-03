#include "Engine/ArcadiaEngine.h"

#include "UIShowcase.h"
#include "SoundShowcase.h"
#include "MemoryShowcase.h"
#include "TopDownExample.h"

GUIObjectNode* currentDialogue;

void CreateTestData()
{
	//  Load some basic fonts
	fontManager.SetFontFolder("Assets/Fonts/");
	fontManager.LoadFont("Arial");
	fontManager.LoadFont("Arial-12-White");

	//  Create the first test dialogue and add it to the scene
	currentDialogue = new TopDownExample;
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
	showcaseDropdown->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "GUI Manager", 10, 4, 160, 20));
	showcaseDropdown->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Sound Wrapper", 10, 4, 160, 20));
	showcaseDropdown->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Memory Manager", 10, 4, 160, 20));
	showcaseDropdown->AddItem(GUILabel::CreateLabel(fontManager.GetFont("Arial"), "TopDown Example", 10, 4, 160, 20));
	showcaseChoiceContainer->AddChild(showcaseDropdown);

	//  Create the button that allows us to move to a new dialogue based on the drop-down setting
	auto showcaseGoButton = GUIButton::CreateTemplatedButton("Standard", 220, 26, 50, 24);
	showcaseGoButton->SetFont(fontManager.GetFont("Arial"));
	showcaseGoButton->SetText("Go");
	showcaseGoButton->SetLeftClickCallback([=](GUIObjectNode*)
	{
		switch (showcaseDropdown->GetSelectedIndex())
		{
		case 0: //  GUI Manager Showcase
			currentDialogue->SetToDestroy(guiManager.GetDestroyList());
			currentDialogue = new UIShowcaseDialogue;
			guiManager.GetBaseNode()->AddChild(currentDialogue);
			break;
		case 1: //  Sound Wrapper Showcase
			currentDialogue->SetToDestroy(guiManager.GetDestroyList());
			currentDialogue = new SoundShowcaseDialogue;
			guiManager.GetBaseNode()->AddChild(currentDialogue);
			break;
		case 2: //  Memory Manager Showcase
			currentDialogue->SetToDestroy(guiManager.GetDestroyList());
			currentDialogue = new MemoryShowcaseDialogue;
			guiManager.GetBaseNode()->AddChild(currentDialogue);
			break;
		case 3: //  Top Down Example
			currentDialogue->SetToDestroy(guiManager.GetDestroyList());
			currentDialogue = new TopDownExample;
			guiManager.GetBaseNode()->AddChild(currentDialogue);
			break;
		default:break;
		}
	});
	showcaseChoiceContainer->AddChild(showcaseGoButton);
}

int main(int argc, char* args[])
{
	//  Attempt to initialize OpenGL and SDL -- Close the program if that fails
	if (!InitializeEngine())
	{
		ShutdownEngine();
		return 1;
	}

	//  Create test data for different systems to ensure they work as they should
	CreateTestData();

	//  Begin the primary loop, and continue until it exits
	PrimaryLoop();

	//  Free resources and close SDL before exiting
	ShutdownEngine();
	return 0;
}