#pragma once

struct Color
{
	Color(float r = 1.0f, float g = 1.0f, float b = 1.0f, float a = 1.0f)
	{
		colorValues[0] = r;
		colorValues[1] = g;
		colorValues[2] = b;
		colorValues[3] = a;
	}

	union {
		struct { float R, G, B, A; };
		float colorValues[4];
	};
};