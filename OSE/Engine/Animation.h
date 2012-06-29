#pragma once

#define ANIMATION_MAX_FRAMES

class Animation
{
private:
	int mSequence[ANIMATION_MAX_FRAMES];
	float mFrameRate;
	int mRow;
	
	bool mIsPlaying; //Are we playing right now?
	int mCurFrame; //Frame that is current being displayed
public:
	Animation(void);
	~Animation(void);
};

