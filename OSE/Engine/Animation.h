#pragma once

#define ANIMATION_MAX_FRAMES 16

struct Animation
{
	std::string mName;
	int mSequence[ANIMATION_MAX_FRAMES];
	int mFrameCount; //How many unique frames are in the animation?
	int mLength; //How many frames are in one cycle of the animation?
	float mFrameRate;
	int mRow;
	int mFrameSize;
};