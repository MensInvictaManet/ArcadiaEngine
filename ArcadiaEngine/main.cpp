#include "Engine/ArcadiaEngine.h"

#include "UIShowcase.h"

void CreateTestData()
{
	//  Load some basic fonts
	fontManager.SetFontFolder("Assets/Fonts/");
	fontManager.LoadFont("Arial");
	fontManager.LoadFont("Arial-12-White");

	//  Create the first test dialogue and add it to the scene
	guiManager.GetBaseNode()->AddChild(new UIShowcaseDialogue);
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