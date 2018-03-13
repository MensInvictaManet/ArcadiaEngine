#pragma once

struct Color
{
	Color()
	{
		colorValues[0] = 1.0f;
		colorValues[1] = 1.0f;
		colorValues[2] = 1.0f;
		colorValues[3] = 1.0f;
	}

	Color(float r, float g, float b, float a)
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