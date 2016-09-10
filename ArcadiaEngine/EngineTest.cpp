#include "ArcadiaEngine.h"

//  Test variables
GUIButton* g_TestButton1;
int g_CoinSound[4];
int g_BackgroundMusic;
SDL_Texture* g_TextTexture = nullptr;
GLuint g_TextTextureID = 0;

int testVarFlag = 0;

void HandleInput()
{
	//Set texture based on current keystate
	testVarFlag = 0;
	if (inputManager.GetKeyDown(SDL_SCANCODE_UP))
	{
		testVarFlag |= 1;
	}
	if (inputManager.GetKeyDown(SDL_SCANCODE_DOWN))
	{
		testVarFlag |= 2;
	}
}

void RenderScreen()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	//  Initiate the 3D Rendering Context
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, GLdouble(SCREEN_WIDTH) / GLdouble(SCREEN_HEIGHT), 1.0, 200.0);

	glDisable(GL_TEXTURE_2D);

	//  Render the 3D quad if the bool is set to true
	if (!(testVarFlag & 1))
	{
		glBegin(GL_QUADS);
			glVertex3f(-5.7f, -1.5f, -50.0f);
			glVertex3f(-4.3f, -1.5f, -50.0f);
			glVertex3f(-4.6f, -0.5f, -50.0f);
			glVertex3f(-5.4f, -0.5f, -50.0f);
		glEnd();
	}

	//  Initiate the 2D Rendering Context
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, GLdouble(SCREEN_WIDTH), GLdouble(SCREEN_HEIGHT), 0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	//  Render the 2D GUI through the GUIManager
	guiManager.Render();
}

void PrimaryLoop()
{
	//  The event handler
	SDL_Event e;

	//  Main loop flag
	auto quit = false;

	//  While application is running
	while (!quit)
	{
		DetermineTimeSlice();

		//  Get the current state of mouse and keyboard input
		inputManager.GetInputForFrame();

		//  Update the sound wrapper
		soundWrapper.Update();

		//  Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			//  User requests quit
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
			//  Handle keypress with current mouse position
			else if (e.type == SDL_TEXTINPUT)
			{
				inputManager.AddKeyToString(e.text.text[0]);
			}
			else
			{
				if (/*e.type == SDL_MOUSEMOTION || */e.type == SDL_MOUSEBUTTONDOWN || e.type == SDL_MOUSEBUTTONUP)
				{
					switch (e.button.button)
					{
					case 1: inputManager.SetMouseButtonLeft((e.type == SDL_MOUSEBUTTONDOWN)); break;
					case 2: inputManager.SetMouseButtonMiddle((e.type == SDL_MOUSEBUTTONDOWN)); break;
					case 3: inputManager.SetMouseButtonRight((e.type == SDL_MOUSEBUTTONDOWN)); break;
					default: break;
					}
				}
				windowManager.HandleEvent(e);
			}
		}

		//  Input
		HandleInput();
		guiManager.Input();

		//  Update
		//  ?

		//  Render
		RenderScreen();
		windowManager.Render();

		//  End Step
		guiManager.EndStep();
	}
}

void CreateTestData()
{
	//  Load some test sounds
	g_CoinSound[0] = soundWrapper.loadSoundFile("CoinPickup1.ogg", SOUNDFILETYPE_OGG);
	g_CoinSound[1] = soundWrapper.loadSoundFile("CoinPickup2.ogg", SOUNDFILETYPE_OGG);
	g_CoinSound[2] = soundWrapper.loadSoundFile("CoinPickup1.wav", SOUNDFILETYPE_WAV);
	g_CoinSound[3] = soundWrapper.loadSoundFile("CoinPickup2.wav", SOUNDFILETYPE_WAV);
	soundWrapper.playSoundFile(g_BackgroundMusic = soundWrapper.loadSoundFile("BackgroundMusic.ogg", SOUNDFILETYPE_OGG));

	fontManager.SetFontFolder("Fonts/");
	fontManager.LoadFont("Arial");
	fontManager.LoadFont("Arial-12-White");

	//  Create some test UI (2 buttons and 1 checkbox)
	static auto useOggFiles = true;

	auto listbox1 = GUIListBox::CreateTemplatedListBox("Standard", 320, 60, 640, 76, 620, 6, 16, 16, 16, 16, 16, 22, 2);
	guiManager.GetBaseNode()->AddChild(listbox1);

	GUIObjectNode* listEntryNode = new GUIObjectNode;
	listbox1->AddItem(listEntryNode);

	auto label1 = GUILabel::CreateLabel("", 0, 0, 200, 30);
	label1->SetFont(fontManager.GetFont("Arial"));
	label1->SetText("Test string 1 A");
	listEntryNode->AddChild(label1);

	auto label2 = GUILabel::CreateLabel("", 200, 0, 200, 30);
	label2->SetFont(fontManager.GetFont("Arial"));
	label2->SetText("Test string 1 B");
	listEntryNode->AddChild(label2);


	auto label3 = GUILabel::CreateLabel("", 0, 0, 200, 30);
	label3->SetFont(fontManager.GetFont("Arial"));
	label3->SetText("Test string 2");
	listbox1->AddItem(label3);

	auto label4 = GUILabel::CreateLabel("", 0, 0, 200, 30);
	label4->SetFont(fontManager.GetFont("Arial"));
	label4->SetText("Test string 3");
	listbox1->AddItem(label4);

	auto label5 = GUILabel::CreateLabel("", 0, 0, 200, 30);
	label5->SetFont(fontManager.GetFont("Arial"));
	label5->SetText("Test string 4");
	listbox1->AddItem(label5);

	auto label6 = GUILabel::CreateLabel("", 0, 0, 200, 30);
	label6->SetFont(fontManager.GetFont("Arial"));
	label6->SetText("Test string 5");
	listbox1->AddItem(label6);

	auto moveable = GUIMoveable::CreateMoveable("ContainerTest.png", 60, 60, 240, 240, 0, 0, 256, 25);
	guiManager.GetBaseNode()->AddChild(moveable);

	auto label7 = GUILabel::CreateLabel("", 0, 0, 200, 30);
	label7->SetFont(fontManager.GetFont("Arial"));
	label7->SetText("Move This UI Around");
	moveable->AddChild(label7);

	auto checkbox1 = GUICheckbox::CreateCheckbox("CheckboxTest1.png", "CheckboxTest2.png", 20, 40, 50, 50);
	checkbox1->SetCheckCallback([=](GUIObjectNode* node)
	{
		useOggFiles = checkbox1->GetChecked();
	});
	moveable->AddChild(checkbox1);

	auto checkbox2 = GUICheckbox::CreateTemplatedCheckbox("Standard", 20, 100, 50, 50);
	checkbox2->SetCheckCallback([=](GUIObjectNode* node)
	{
		useOggFiles = checkbox2->GetChecked();
	});
	moveable->AddChild(checkbox2);

	g_TestButton1 = GUIButton::CreateButton("ButtonTest1.png", 120, 40, 100, 50);
	g_TestButton1->SetFont(fontManager.GetFont("Arial"));
	g_TestButton1->SetText("Sound 1");
	g_TestButton1->SetLeftClickCallback([=](GUIObjectNode* node)
	{
		soundWrapper.playSoundFile(g_CoinSound[useOggFiles ? 0 : 2]);
	});
	moveable->AddChild(g_TestButton1);

	auto button2 = GUIButton::CreateTemplatedButton("Standard", 120, 100, 100, 50);
	button2->SetFont(fontManager.GetFont("Arial-12-White"));
	button2->SetText("Sound 2");
	button2->SetLeftClickCallback([=](GUIObjectNode* node)
	{
		soundWrapper.playSoundFile(g_CoinSound[useOggFiles ? 1 : 3]);
	});
	moveable->AddChild(button2);

	auto editbox1 = GUIEditBox::CreateTemplatedEditBox("Standard", 20, 160, 200, 30);
	editbox1->SetFont(fontManager.GetFont("Arial"));
	editbox1->SetText("EDIT THIS TEXT");
	moveable->AddChild(editbox1);

	//  Test the Winsock Wrapper
	winsockWrapper.WinsockInitialize();
	auto googleIP = Socket::GetHostIP("www.google.com");
	auto ipAddress = winsockWrapper.ConvertIPtoUINT(googleIP.c_str());
	auto ipString = winsockWrapper.ConvertUINTtoIP(ipAddress);
	winsockWrapper.WinsockShutdown();
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