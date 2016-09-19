#pragma once

#define AUDIO_ENABLED		true
#define CONSOLE_DISABLED	true

#define USING_SDL			true
#define USING_SDL_IMAGE		true
#define USING_SDL_MIXER		true
#define	USING_OPENGL		true
#define	USING_GLU			true

#if USING_SDL
#include "./SDL2/SDL.h"
#undef main
#pragma comment(lib, "Engine/SDL2/SDL2.lib")
#endif

#if USING_SDL_IMAGE
#include "./SDL2/SDL_image.h"
#pragma comment(lib, "Engine/SDL2/SDL2_image.lib")
#endif

#if AUDIO_ENABLED
#if USING_SDL_MIXER
#include "./SDL2/SDL_mixer.h"
#pragma comment(lib, "Engine/SDL2/SDL2_mixer.lib")
#endif
#endif

#if USING_OPENGL
#include "./SDL2/SDL_opengl.h"
#pragma comment(lib, "opengl32.lib")
#endif

#if USING_GLU
#include <GL/GLU.h>
#pragma comment(lib, "glu32.lib")
#endif

#if CONSOLE_DISABLED
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include "WindowManager.h"
#include "TextureManager.h"
#include "GUIManager.h"
#include "GUIMoveable.h"
#include "GUILabel.h"
#include "GUIButton.h"
#include "GUICheckbox.h"
#include "GUIEditBox.h"
#include "GUIListBox.h"
#include "GUIDropDown.h"
#include "InputManager.h"
#include "FontManager.h"
#include "TimeSlice.h"
#include "WinsockWrapper.h"
#include "MemoryManager.h"

#if AUDIO_ENABLED
#include "SoundWrapper.h"
#endif

//  Screen dimension constants (change per project)
#define	SCREEN_WIDTH	1024
#define	SCREEN_HEIGHT	768

inline void ResizeWindow(void)
{
	glViewport(0, 0, GLsizei(SCREEN_WIDTH), GLsizei(SCREEN_WIDTH));

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f, GLdouble(SCREEN_WIDTH) / GLdouble(SCREEN_WIDTH), 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

inline bool InitializeSDL()
{
	//  Initialize SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
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

inline bool InitializeOpenGL()
{
	auto success = true;
	GLenum error;

	//  Initialize the Projection Matrix
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//  Check for an error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %p\n", gluErrorString(error));
		success = false;
	}

	//  Initialize Modelview Matrix
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//  Check for an error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %p\n", gluErrorString(error));
		success = false;
	}

	//  Initialize clear color
	glClearColor(0.5f, 0.5f, 0.5f, 1.f);

	//  Check for an error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %p\n", gluErrorString(error));
		success = false;
	}

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return success;
}

inline bool InitializeEngine()
{
	//  Initialize SDL
	if (!InitializeSDL())
	{
		printf("Unable to initialize SDL!\n");
		return false;
	}

	auto windowIndex = windowManager.CreateNewWindow("Arcadia Engine", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (windowIndex == -1) return false;

	SDL_SetRenderDrawColor(windowManager.GetRenderer(windowIndex), 0xFF, 0xFF, 0xFF, 0xFF);

	//  Initialize Winsock
	winsockWrapper.WinsockInitialize();

	//  Initialize OpenGL
	if (!InitializeOpenGL())
	{
		printf("Unable to initialize OpenGL!\n");
		return false;
	}

#if AUDIO_ENABLED
#if USING_SDL_MIXER
	//Initialize SDL_mixer 
	if (Mix_OpenAudio(22050, MIX_DEFAULT_FORMAT, 2, 4096) == -1)
	{
		printf("Unable to initialize SDL_mixer!\n");
		return false;
	}
#endif
#endif

	return true;
}

inline void ShutdownEngine()
{

	//  Quit all SDL sub-systems
#if USING_SDL_IMAGE
	IMG_Quit();
#endif

	//  Shutdown the manager classes that need it
	winsockWrapper.WinsockShutdown();
	textureManager.Shutdown();
	windowManager.Shutdown();
	xmlWrapper.Shutdown();
	guiManager.Shutdown();

#if AUDIO_ENABLED
#if USING_SDL_MIXER
	soundWrapper.Shutdown();
	Mix_CloseAudio();
#endif
#endif

	memoryManager.Shutdown();

#if USING_SDL
	//  Disable text input
	SDL_StopTextInput();

	SDL_Quit();
#endif
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

	guiManager.Render3D();

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
		guiManager.Update();

		//  Handle events on queue
		while (SDL_PollEvent(&e) != 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				quit = true;
				break;

			case SDL_TEXTINPUT:
				inputManager.AddKeyToString(e.text.text[0]);
				break;

			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				switch (e.button.button)
				{
				case 1: inputManager.SetMouseButtonLeft((e.type == SDL_MOUSEBUTTONDOWN)); break;
				case 2: inputManager.SetMouseButtonMiddle((e.type == SDL_MOUSEBUTTONDOWN)); break;
				case 3: inputManager.SetMouseButtonRight((e.type == SDL_MOUSEBUTTONDOWN)); break;
				default: break;
				}

			default:
				windowManager.HandleEvent(e);
				break;
			}
		}

		//  Input
		guiManager.Input();

		//  Update
		guiManager.Update();

		//  Render
		RenderScreen();
		windowManager.Render();

		//  End Step
		guiManager.EndStep();
	}
}