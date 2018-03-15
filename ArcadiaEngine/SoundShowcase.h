#pragma once

#include "Engine/GUIObjectNode.h"
#include "Engine/GUIMoveable.h"
#include "Engine/GUILabel.h"
#include "Engine/GUIButton.h"
#include "Engine/SoundWrapper.h"

class SoundShowcaseDialogue : public GUIObjectNode
{
public:
	SoundShowcaseDialogue();
	~SoundShowcaseDialogue();

private:

	int backgroundMusicID;
	int coin1SoundID;
	int coin2SoundID;
	int coin3SoundID;
	int coin4SoundID;
};

inline SoundShowcaseDialogue::SoundShowcaseDialogue()
{
	//  Create the label that acts as an explanation of the current showcase UI
	auto introductionLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "This is a basic showcase of the Sound Wrapper system.", 10, 10, 300, 32);
	AddChild(introductionLabel);

	//  Create the container that holds all of the main objects in the scene
	auto container = GUIMoveable::CreateTemplatedMoveable("Standard", 10, 40, 700, 210, 0, 0, 0, 0);
	container->SetMoveable(false);
	AddChild(container);

	//  Create the label that acts as an explanation of the sound wrapper
	auto soundExplanationLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "The sound wrapper allows you to easily load and play sound effects and music.", 10, 20, 984, 22);
	container->AddChild(soundExplanationLabel);

	//  Load the music file and sound effects
	backgroundMusicID = soundWrapper.loadMusicFile("Assets/Audio/BackgroundMusic.ogg", -1);
	coin1SoundID = soundWrapper.loadSoundFile("Assets/Audio/CoinPickup1.ogg", -1);
	coin2SoundID = soundWrapper.loadSoundFile("Assets/Audio/CoinPickup2.ogg", -1);
	coin3SoundID = soundWrapper.loadSoundFile("Assets/Audio/CoinPickup1.wav", -1);
	coin4SoundID = soundWrapper.loadSoundFile("Assets/Audio/CoinPickup2.wav", -1);

	//  Play the music file
	soundWrapper.playMusicFile(backgroundMusicID);

	//  Create the label that describes the current status of the background music
	auto bgMusicStatusLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Background Music: PLAYING", 250, 60, 984, 22);
	container->AddChild(bgMusicStatusLabel);

	//  Create the label that describes the current status of the first sound effect
	auto sound1StatusLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Coin Sound 1 (OGG)", 250, 90, 984, 22);
	container->AddChild(sound1StatusLabel);

	//  Create the label that describes the current status of the second sound effect
	auto sound2StatusLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Coin Sound 2 (OGG)", 250, 120, 984, 22);
	container->AddChild(sound2StatusLabel);

	//  Create the label that describes the current status of the third sound effect
	auto sound3StatusLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Coin Sound 1 (WAV)", 250, 150, 984, 22);
	container->AddChild(sound3StatusLabel);

	//  Create the label that describes the current status of the fourth sound effect
	auto sound4StatusLabel = GUILabel::CreateLabel(fontManager.GetFont("Arial"), "Coin Sound 2 (WAV)", 250, 180, 984, 22);
	container->AddChild(sound4StatusLabel);

	//  Create the PLAY button for the background music
	auto bgMusicPlayButton = GUIButton::CreateTemplatedButton("Standard", 10, 56, 60, 24);
	bgMusicPlayButton->SetFont(fontManager.GetFont("Arial"));
	bgMusicPlayButton->SetText("PLAY");
	bgMusicPlayButton->SetLeftClickCallback([=](GUIObjectNode*)
	{
		bgMusicStatusLabel->SetText("Background Music: PLAYING");
		soundWrapper.playMusicFile(backgroundMusicID);
	});
	container->AddChild(bgMusicPlayButton);

	//  Create the PAUSE button for the background music
	auto bgMusicPauseButton = GUIButton::CreateTemplatedButton("Standard", 80, 56, 60, 24);
	bgMusicPauseButton->SetFont(fontManager.GetFont("Arial"));
	bgMusicPauseButton->SetText("PAUSE");
	bgMusicPauseButton->SetLeftClickCallback([=](GUIObjectNode*)
	{
		if (soundWrapper.GetMusicPlaying()) bgMusicStatusLabel->SetText("Background Music: PAUSED");
		soundWrapper.pauseMusicFile(backgroundMusicID);
	});
	container->AddChild(bgMusicPauseButton);

	//  Create the PAUSE button for the background music
	auto bgMusicStopButton = GUIButton::CreateTemplatedButton("Standard", 150, 56, 60, 24);
	bgMusicStopButton->SetFont(fontManager.GetFont("Arial"));
	bgMusicStopButton->SetText("STOP");
	bgMusicStopButton->SetLeftClickCallback([=](GUIObjectNode*)
	{
		bgMusicStatusLabel->SetText("Background Music: STOPPED");
		soundWrapper.stopMusicFile(backgroundMusicID);
	});
	container->AddChild(bgMusicStopButton);

	//  Create the PLAY button for the first sound effect
	auto sound1PlayButton = GUIButton::CreateTemplatedButton("Standard", 10, 86, 60, 24);
	sound1PlayButton->SetFont(fontManager.GetFont("Arial"));
	sound1PlayButton->SetText("PLAY");
	sound1PlayButton->SetLeftClickCallback([=](GUIObjectNode*)
	{
		soundWrapper.playSoundFile(coin1SoundID);
	});
	container->AddChild(sound1PlayButton);

	//  Create the PLAY button for the second sound effect
	auto sound2PlayButton = GUIButton::CreateTemplatedButton("Standard", 10, 116, 60, 24);
	sound2PlayButton->SetFont(fontManager.GetFont("Arial"));
	sound2PlayButton->SetText("PLAY");
	sound2PlayButton->SetLeftClickCallback([=](GUIObjectNode*)
	{
		soundWrapper.playSoundFile(coin2SoundID);
	});
	container->AddChild(sound2PlayButton);

	//  Create the PLAY button for the third sound effect
	auto sound3PlayButton = GUIButton::CreateTemplatedButton("Standard", 10, 146, 60, 24);
	sound3PlayButton->SetFont(fontManager.GetFont("Arial"));
	sound3PlayButton->SetText("PLAY");
	sound3PlayButton->SetLeftClickCallback([=](GUIObjectNode*)
	{
		soundWrapper.playSoundFile(coin3SoundID);
	});
	container->AddChild(sound3PlayButton);

	//  Create the PLAY button for the fourth sound effect
	auto sound4PlayButton = GUIButton::CreateTemplatedButton("Standard", 10, 176, 60, 24);
	sound4PlayButton->SetFont(fontManager.GetFont("Arial"));
	sound4PlayButton->SetText("PLAY");
	sound4PlayButton->SetLeftClickCallback([=](GUIObjectNode*)
	{
		soundWrapper.playSoundFile(coin4SoundID);
	});
	container->AddChild(sound4PlayButton);
}

inline SoundShowcaseDialogue::~SoundShowcaseDialogue()
{
	soundWrapper.stopMusicFile(backgroundMusicID);
}