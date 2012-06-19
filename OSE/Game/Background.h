#pragma once

#include "baserenderable.h"

#define MAX_PASSES 3
#define TEXTURE_SIZE 4096
#define TEXTURE_SCALE 2.f
#define SLOP 5.f //Overlap
#define TEXTURE_SIZE_TRUE TEXTURE_SIZE * TEXTURE_SCALE

class BGRender : public BaseRender
{
private:
	sf::RenderTexture mTexture[MAX_PASSES];
	sf::Sprite mSprite[MAX_PASSES];
	sf::Texture mStarTex;
	sf::Sprite mStar;
	float ARR_SIZES[MAX_PASSES];
	double mNextMeteor;
	bool mDrawingMeteor;
	Vector2 mMeteor;
	sf::Texture mMeteorTex;
	sf::Sprite mMeteorSprite;
public:
	BGRender(void);
	~BGRender(void);

	sf::Drawable *GetDrawable()
	{
		return &mSprite[0];
	}
	void PreDraw(sf::RenderWindow *pRender);
	void Draw(sf::RenderWindow *pRender);
	void SetSize(Vector2 size) {}; //TODO:IMPLEMENT RESIZE
	void SetColour(sf::Color col) {mSprite[0].setColor(col); mColour = col;};
};

class Background : public BaseRenderable
{
private:

public:
	Background(void);
	~Background(void);
	void Spawn();
	void Think();
};

