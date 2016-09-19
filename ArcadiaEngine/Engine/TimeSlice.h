#pragma once

#include <algorithm>

Uint32 gameTicksUint = 0;
Uint32 frameTicksUint = 0;
float gameSeconds = 0.0f;
float frameSeconds = 0.0f;

#define TICKS_TO_SECONDS(ticks) float(ticks) / 1000.0f
#define SECONDS_TO_TICKS(seconds) Uint32(seconds * 1000.0f)

inline void DetermineTimeSlice()
{
	// Get the time slice
	static Uint32 lastGameTicksUint = 0;

	gameTicksUint = SDL_GetTicks();
	gameSeconds = TICKS_TO_SECONDS(gameTicksUint);
	frameTicksUint = std::min<Uint32>(gameTicksUint - lastGameTicksUint, 1000);
	frameSeconds = TICKS_TO_SECONDS(frameTicksUint);
	lastGameTicksUint = gameTicksUint;
}