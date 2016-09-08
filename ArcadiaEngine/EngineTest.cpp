#define USING_SDL			true
#define USING_SDL_IMAGE		true
#define	USING_OPENGL		true
#define	USING_GLU			true
#define USING_SDL_MIXER		false

#if USING_SDL
#include "SDL2/SDL.h"
#undef main
#pragma comment(lib, "SDL2/SDL2.lib")
#endif

#if USING_SDL_IMAGE
#include "SDL2/SDL_image.h"
#pragma comment(lib, "SDL2/SDL2_image.lib")
#endif

#if USING_OPENGL
#include "SDL2/SDL_opengl.h"
#pragma comment(lib, "opengl32.lib")
#endif

#if USING_GLU
#include <GL/GLU.h>
#pragma comment(lib, "glu32.lib")
#endif

#if USING_SDL_MIXER
#endif

#include "WindowManager.h"
#include "TextureManager.h"
#include "GUIManager.h"
#include "GUIMoveable.h"
#include "GUIButton.h"
#include "GUICheckbox.h"
#include "GUIEditBox.h"
#include "InputManager.h"
#include "SoundWrapper.h"
#include "FontManager.h"
#include "TimeSlice.h"

//  Test variables
GUIButton* g_TestButton1;
int g_CoinSound[4];
int g_BackgroundMusic;
SDL_Texture* g_TextTexture = nullptr;
GLuint g_TextTextureID = 0;

int testVarFlag = 0;

//  Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

bool InitializeSDL()
{
	//  Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
		return false;
	}

	//  Set SDL to use OpenGL 2.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

	//  Set the OpenGL attributes for multisampling
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);

	//  Set the texture filtering to linear
	if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
	{
		printf("Warning: Linear texture filtering not enabled!");
	}

	//  Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	if (!(IMG_Init(imgFlags) & imgFlags))
	{
		printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
		return false;
	}

	//  Enable text input
	SDL_StartTextInput();

	return true;
}

bool InitializeOpenGL()
{
	auto success = true;
	GLenum error = GL_NO_ERROR;

	//  Initialize the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//  Check for an error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	//  Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//  Check for an error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	//  Initialize clear color
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	//  Check for an error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return success;
}

unsigned int power_two_floor(unsigned int val)
{
	unsigned int power = 2, nextVal = power * 2;

	while ((nextVal *= 2) <= val)
		power *= 2;

	return power * 2;
}

bool Initialize()
{
	//  Initialization flag
	auto success = true;

	//  Initialize SDL
	if (!InitializeSDL())
	{
		printf("Unable to initialize SDL!\n");
		return false;
	}

	auto windowIndex = windowManager.CreateNewWindow("Arcadia Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (windowIndex == -1) return false;

	SDL_SetRenderDrawColor(windowManager.GetRenderer(windowIndex), 0xFF, 0xFF, 0xFF, 0xFF);

	//  Initialize OpenGL
	if (!InitializeOpenGL())
	{
		printf("Unable to initialize OpenGL!\n");
		return false;
	}

	//  Initialize the sound wrapper
	if (soundWrapper.Initialize() == false)
	{
		printf("Unable to initialize SoundWrapper!\n");
		return false;
	}

	return true;
}

void ResizeWindow(void)
{
	glViewport(0, 0, GLsizei(SCREEN_WIDTH), GLsizei(SCREEN_WIDTH));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, GLdouble(SCREEN_WIDTH) / GLdouble(SCREEN_WIDTH), 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void CloseProgram()
{
	//  Quit all SDL sub-systems
#if USING_SDL_IMAGE
	IMG_Quit();
#endif

	//  Shutdown the manager classes that need it
	textureManager.Shutdown();
	windowManager.Shutdown();
	soundWrapper.Shutdown();

#if USING_SDL
	//  Disable text input
	SDL_StopTextInput();

	SDL_Quit();
#endif
}

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
	fontManager.GetFont("Arial")->RenderText("TESTing! This Font :)", 100, 400, true, false);
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

	auto moveable = GUIMoveable::CreateMoveable("ContainerTest.png", 250, 200, 256, 256, 0, 0, 256, 25);
	guiManager.GetBaseNode()->AddChild(moveable);

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

	g_TestButton1 = GUIButton::CreateButton("ButtonTest1.png", 100, 40, 100, 50);
	g_TestButton1->SetFont(fontManager.GetFont("Arial"));
	g_TestButton1->SetText("Sound 1");
	g_TestButton1->SetLeftClickCallback([=](GUIObjectNode* node)
	{
		soundWrapper.playSoundFile(g_CoinSound[useOggFiles ? 0 : 2]);
	});
	moveable->AddChild(g_TestButton1);

	auto button2 = GUIButton::CreateTemplatedButton("Standard", 100, 100, 100, 50);
	button2->SetFont(fontManager.GetFont("Arial-12-White"));
	button2->SetText("Sound 2");
	button2->SetLeftClickCallback([=](GUIObjectNode* node)
	{
		soundWrapper.playSoundFile(g_CoinSound[useOggFiles ? 1 : 3]);
	});
	moveable->AddChild(button2);

	auto editbox1 = GUIEditBox::CreateTemplatedEditBox("Standard", 40, 160, 200, 30);
	editbox1->SetFont(fontManager.GetFont("Arial"));
	editbox1->SetText("EDIT THIS TEXT");
	moveable->AddChild(editbox1);
}

int main(int argc, char* args[])
{
	//  Attempt to initialize OpenGL and SDL. Close the program if that fails.
	if (!Initialize())
	{
		CloseProgram();
		return 1;
	}

	//  Create test data for different systems to ensure they work as they should
	CreateTestData();

	//  Begin the primary loop, and continue until it exits
	PrimaryLoop();

	//  Free resources and close SDL
	CloseProgram();

	return 0;
}