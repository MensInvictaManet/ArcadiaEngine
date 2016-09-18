#pragma once

#include "MemoryManager.h"

#include "SDL2\SDL_mixer.h"

class SoundWrapper
{
public:
	static SoundWrapper& GetInstance() { static SoundWrapper INSTANCE; return INSTANCE; }

	int loadSoundFile(const char* soundFileName, int identifier = -1);
	int loadMusicFile(const char* soundFileName, int identifier = -1);
	bool playSoundFile(int identifier);
	bool playMusicFile(int identifier);
	bool unloadSoundFile(int identifier);
	bool unloadMusicFile(int identifier);
	void Shutdown();

private:
	SoundWrapper() {}
	~SoundWrapper();

	int FindFirstFreeSoundIndex();
	int FindFirstFreeMusicIndex();
	void unloadAllSoundFiles();

	std::map< int, std::pair<std::string, Mix_Chunk*> > soundDataList;
	std::map< int, std::pair<std::string, Mix_Music*> > musicDataList;
};

inline int SoundWrapper::loadSoundFile(const char* soundFileName, int identifier)
{
	if (identifier == -1) identifier = FindFirstFreeSoundIndex();

	auto nameString = std::string(soundFileName);
	auto iterID = soundDataList.find(identifier);
	if (iterID != soundDataList.end()) return ((*iterID).second.first.compare(nameString ) == 0 ? identifier : -1);

	MANAGE_MEMORY_NEW("SoundWrapper", sizeof(Mix_Chunk));
	auto newChunk = Mix_LoadWAV(soundFileName);
	if (newChunk == nullptr)
	{
		printf("Failed to load sound file: %s\n", soundFileName);
		return -1;
	}
	else
	{
		soundDataList[identifier] = std::pair<std::string, Mix_Chunk*>(nameString, newChunk);
		return identifier;
	}
}

inline int SoundWrapper::loadMusicFile(const char* soundFileName, int identifier)
{
	if (identifier == -1) identifier = FindFirstFreeMusicIndex();

	auto nameString = std::string(soundFileName);
	auto iterID = musicDataList.find(identifier);
	if (iterID != musicDataList.end()) return ((*iterID).second.first.compare(nameString) == 0 ? identifier : -1);

	MANAGE_MEMORY_NEW("SoundWrapper", 100);
	auto newMusic = Mix_LoadMUS(soundFileName);
	if (newMusic == nullptr)
	{
		printf("Failed to load music file: %s\n", soundFileName);
		return -1;
	}
	else
	{
		musicDataList[identifier] = std::pair<std::string, Mix_Music*>(nameString, newMusic);
		return identifier;
	}
}

inline bool SoundWrapper::playSoundFile(int identifier)
{
	auto findIter = soundDataList.find(identifier);
	if (findIter == soundDataList.end()) return false;

	return (Mix_PlayChannel(-1, (*findIter).second.second, 0) != -1);
}

inline bool SoundWrapper::playMusicFile(int identifier)
{
	auto findIter = musicDataList.find(identifier);
	if (findIter == musicDataList.end()) return false;

	return (Mix_PlayMusic((*findIter).second.second, -1) != -1);
}

inline bool SoundWrapper::unloadSoundFile(int identifier)
{
	auto findIter = soundDataList.find(identifier);
	if (findIter == soundDataList.end()) return false;

	MANAGE_MEMORY_DELETE("SoundWrapper", sizeof(Mix_Chunk));
	Mix_FreeChunk((*findIter).second.second);
	soundDataList.erase(findIter);
	return true;
}

inline bool SoundWrapper::unloadMusicFile(int identifier)
{
	auto findIter = musicDataList.find(identifier);
	if (findIter == musicDataList.end()) return false;

	MANAGE_MEMORY_DELETE("SoundWrapper", 100);
	Mix_FreeMusic((*findIter).second.second);
	musicDataList.erase(findIter);
	return true;
}

inline void SoundWrapper::Shutdown()
{
	unloadAllSoundFiles();
}

inline SoundWrapper::~SoundWrapper()
{
	Shutdown();
}

inline int SoundWrapper::FindFirstFreeSoundIndex()
{
	for (auto i = 0; ; ++i)
	{
		auto findIter = soundDataList.find(i);
		if (findIter == soundDataList.end()) return i;
	}
}

inline int SoundWrapper::FindFirstFreeMusicIndex()
{
	for (auto i = 0; ; ++i)
	{
		auto findIter = musicDataList.find(i);
		if (findIter == musicDataList.end()) return i;
	}
}

inline void SoundWrapper::unloadAllSoundFiles()
{
	while (!soundDataList.empty()) unloadSoundFile(soundDataList.begin()->first);
	while (!musicDataList.empty()) unloadMusicFile(musicDataList.begin()->first);
}

SoundWrapper& soundWrapper = SoundWrapper::GetInstance();