#pragma once

#include <vector>
#include "Sprite.h"

class Animation;

class SpriteAnimating : public Sprite
{
private:
	std::vector<Animation*> mAnimations;
public:
	SpriteAnimating(void);
	~SpriteAnimating(void);
};

