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
	Vector2 GetSize()
	{
		Vector2 size;
		size.x = mSprite.getTexture()->getSize().x;
		size.y = mSprite.getTexture()->getSize().y;
		return size;
	};
	Vector2 GetTextureCentre() {return Vector2(mTexture.getSize().x / 2, mTexture.getSize().y / 2);};
	void Draw();
	void SetTexture(const char* path);
};

