#pragma once

#include <SFML/Graphics.hpp>
#include "BaseDrawable.h"

class Sprite : public BaseDrawable
{
private:
	sf::Sprite mSprite;
	sf::Texture mTexture; //This shouldn't be here
public:
	Sprite(sf::RenderWindow *rend);
	~Sprite(void);
	void Draw();
	void SetTexture(const char* path);
};

