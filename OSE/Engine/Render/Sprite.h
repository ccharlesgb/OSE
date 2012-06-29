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
		size.x = (float)mSprite.getTextureRect().width;
		size.y = (float)mSprite.getTextureRect().height;
		return size;
	};
	Vector2 GetTextureCentre() {return Vector2(mSprite.getTextureRect().width / 2, mSprite.getTextureRect().height / 2);};
	void Draw();
	void SetTexture(const char* path);
	void SetColour(const Colour &col) {mSprite.setColor(sf::Color(col.r, col.g,col.b,col.a));};

	void SetTextureRect(const sf::IntRect &rect) {mSprite.setTextureRect(rect);};
};

