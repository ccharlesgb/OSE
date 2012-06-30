#pragma once

#include <vector>
#include "Sprite.h"

struct Animation;

class SpriteAnimating : public Sprite
{
private:
	std::vector<Animation*> mAnimations;
public:	
	SpriteAnimating(sf::RenderWindow *rend);
	~SpriteAnimating(void);
};

