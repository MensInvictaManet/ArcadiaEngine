#define USING_SDL			true
#define USING_SDL_IMAGE		true
#define	USING_OPENGL		true
#define	USING_GLU			true
#define USING_SDL_TTF		false
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

#if USING_SDL_TTF
#endif

#if USING_SDL_MIXER
#endif

#include "WindowManager.h"
#include "TextureManager.h"
#include "GUIManager.h"
#include "GUIButton.h"
#include "GUICheckbox.h"
#include "InputManager.h"

//  Test variables
GUIButton* g_TestButton;
GUICheckbox* g_TestCheckbox;

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
	glClearColor(0.f, 0.f, 0.f, 1.f);

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

bool Initialize()
{
	//  Initialization flag
	auto success = true;

	if (!InitializeSDL())
	{
		printf("Unable to initialize SDL!\n");
		return false;
	}

	auto windowIndex = windowManager.CreateNewWindow("Arcadia Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (windowIndex == -1) return false;

	// TEST 2nd WINDOW
	windowIndex = windowManager.CreateNewWindow("Arcadia Engine 2", 960, 100, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);
	if (windowIndex == -1) return false;
	// TEST 2nd WINDOW

	//  Initialize OpenGL
	if (!InitializeOpenGL())
	{
		printf("Unable to initialize OpenGL!\n");
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

#if USING_SDL
	//  Disable text input
	SDL_StopTextInput();

	SDL_Quit();
#endif
}

void HandleTextInput(unsigned char key)
{
	//  Toggle quad rendering
	if (key == 'q' && g_TestButton != nullptr)
	{
		guiManager.DestroyNode(g_TestButton);
		g_TestButton = nullptr;
	}
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

	//  Render the 2D GUI through the GUIManager
	guiManager.Render();

	//  Render the 2D quad if the bool is set to true
	if (!(testVarFlag & 2))
	{
		glBegin(GL_QUADS);
		glVertex2f(100.0f, 100.0f);
		glVertex2f(200.0f, 100.0f);
		glVertex2f(200.0f, 200.0f);
		glVertex2f(100.0f, 200.0f);
		glEnd();
	}
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
		//  Get the current state of mouse and keyboard input
		inputManager.GetInputForFrame();

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
				HandleTextInput(e.text.text[0]);
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

int main(int argc, char* args[])
{
	//  Attempt to initialize OpenGL and SDL. Close the program if that fails.
	if (!Initialize())
	{
		CloseProgram();
		return 1;
	}

	//  Create some test UI (A button and a checkbox)
	g_TestButton = GUIButton::CreateButton("ButtonTest.png", 100, 100, 100, 100);
	g_TestButton->SetLeftClickCallback([=](GUIObjectNode* node)
	{
		if (g_TestCheckbox != nullptr) g_TestCheckbox->SetChecked(false);
	});
	guiManager.GetBaseNode()->AddChild(g_TestButton);

	g_TestCheckbox = GUICheckbox::CreateCheckbox("CheckboxTest1.png", "CheckboxTest2.png", 400, 100, 100, 100);
	g_TestCheckbox->SetCheckCallback([=](GUIObjectNode* node) {});
	guiManager.GetBaseNode()->AddChild(g_TestCheckbox);

	//  Begin the primary loop, and continue until it exits
	PrimaryLoop();

	//  Free resources and close SDL
	CloseProgram();

	return 0;
}