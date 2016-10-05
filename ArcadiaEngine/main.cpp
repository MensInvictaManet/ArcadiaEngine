#include "Engine/ArcadiaEngine.h"

#include "UIShowcase.h"
#include "SoundShowcase.h"
#include "MemoryShowcase.h"
#include "TopDownExample.h"
#include "Engine/GUIMoveable.h"
#include "Engine/GUIDropDown.h"
#include "Engine/GUILabel.h"
#include "Engine/GUIButton.h"

GUIObjectNode* currentDialogue;

void AddDebugConsoleCommands()
{
	//  MOVE_MOUSE_OVER: Automatically moves the mouse to a UI object click position
	debugConsole->AddDebugCommand("MOVE_MOUSE_OVER", [=](std::string& commandString) -> bool
	{
		std::string args[3];
		auto argCount = 0;

		for (auto i = 0; i < 3; ++i)
		{
			if (commandString.empty()) break;
			int firstSpace = commandString.find_first_of(' ');
			if (firstSpace == -1) firstSpace = commandString.length();
			args[i] = commandString.substr(0, firstSpace);
			argCount = i + 1;
			commandString = (firstSpace == commandString.length()) ? "" : commandString.substr(firstSpace + 1, commandString.length());
		}

		auto mouseX = 0;
		auto mouseY = 0;
		auto objectFound = guiManager.GetClickPosition(args[0], mouseX, mouseY);
		switch (argCount)
		{
		case 1:
			InputManager::SetMousePosition(mouseX, mouseY);
			break;

		case 2: //  A float for time was provided
		{
			auto time = float(atof(args[1].c_str()));
			inputManager.SetMousePositionTarget(mouseX, mouseY, time);
			break;
		}
		break;

		case 3: //  Two ints for speed were provided
		{
			auto speedX = atoi(args[1].c_str());
			auto speedY = atoi(args[2].c_str());
			inputManager.SetMousePositionTarget(mouseX, mouseY, speedX, speedY);
			break;
		}
		break;

		default: break;
		}

		return objectFound;
	});

	//  CLICK_MOUSE_LEFT: Simulates a left click
	debugConsole->AddDebugCommand("CLICK_MOUSE_LEFT", [=](std::string& commandString) -> bool
	{
		inputManager.SetSimulatedMouseButtonLeft(MOUSE_BUTTON_PRESSED);
		return true;
	});

	//  CLICK_MOUSE_MIDDLE: Simulates a middle click
	debugConsole->AddDebugCommand("CLICK_MOUSE_MIDDLE", [=](std::string& commandString) -> bool
	{
		inputManager.SetSimulatedMouseButtonMiddle(MOUSE_BUTTON_PRESSED);
		return true;
	});

	//  CLICK_MOUSE_LEFT: Simulates a Right click
	debugConsole->AddDebugCommand("CLICK_MOUSE_RIGHT", [=](std::string& commandString) -> bool
	{
		inputManager.SetSimulatedMouseButtonRight(MOUSE_BUTTON_PRESSED);
		return true;
	});
}

void CreateTestData()
{
	//  Load some basic fonts
	fontManager.SetFontFolder("Assets/Fonts/");
	fontManager.LoadFont("Arial");
	fontManager.LoadFont("Arial-12-White");

	//  Set the font on the Debug Console
	debugConsole->SetFont(fontManager.GetFont("Arial-12-White"));

	AddDebugConsoleCommands();

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
	showcaseDropdown->SetObjectName("DialogueDropdown");
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