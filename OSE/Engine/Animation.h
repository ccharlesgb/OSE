#pragma once

#define ANIMATION_MAX_FRAMES 16

struct Animation
{
	const char* mName;
	int mSequence[ANIMATION_MAX_FRAMES];
	float mFrameRate;
	int mRow;
};