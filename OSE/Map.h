#pragma once

#include "baserenderable.h"

#define TEXTURE_SIZE 1024
#define TEXTURE_SCALE 2.f
#define SLOP 5.f //Overlap
#define TEXTURE_SIZE_TRUE TEXTURE_SIZE * TEXTURE_SCALE

class MapRender : public BaseRender
{
private:
	sf::Sprite mSprite;
	sf::Texture mGrassTex;
public:
	MapRender(void);
	~MapRender(void);

	sf::Drawable *GetDrawable()
	{
		return &mSprite;
	}
	void PreDraw(sf::RenderWindow *pRender);
	void Draw(sf::RenderWindow *pRender);
	void SetSize(Vector2 size) {}; //TODO: IMPLEMENT
	void SetColour(sf::Color col) {mSprite.setColor(col); mColour = col;};
};

class Map : public BaseRenderable
{
private:

public:
	Map(void);
	~Map(void);
	void Spawn();
	void Think();
};

