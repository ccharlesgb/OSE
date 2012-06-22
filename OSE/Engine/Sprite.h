#pragma once

#include <SFML/Graphics.hpp>
#include "BaseDrawable.h"

class Colour
{
public:
	int r;
	int g;
	int b;
	int a;
	Colour() {r=g=b=a=255;};
	Colour(int R, int G, int B) {r=R; g = G; b = B; a = 255;};
	Colour(int R, int G, int B, int A) {r=R; g = G; b = B; a = A;};
	Colour(const Colour& col) {r = col.r; g = col.g; b = col.b; a = col.a;};
};

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

