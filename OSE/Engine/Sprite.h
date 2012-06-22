#pragma once

#include <SFML/Graphics.hpp>
#include "BaseDrawable.h"

class Sprite : public BaseDrawable
{
private:
	sf::Sprite mSprite;
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
	Vector2 GetTextureCentre() {return Vector2(mSprite.getTexture()->getSize().x / 2, mSprite.getTexture()->getSize().y / 2);};
	void Draw();
	void SetTexture(const char* path);
	void SetColour(Colour col) {mSprite.setColor(sf::Color(col.r, col.g,col.b,col.a));};
};

