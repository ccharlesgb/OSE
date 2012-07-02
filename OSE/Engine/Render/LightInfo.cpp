#include "LightInfo.h"


LightInfo::LightInfo(void)
{
	mColour = sf::Color(200,200,200);
	mRadius = 512.f;
	UpdateLightSprite();
}

LightInfo::~LightInfo(void)
{
}

void LightInfo::UpdateLightSprite()
{
	sf::CircleShape mLightShape;
	mLightShape.setRadius(mRadius);
	mLightShape.setFillColor(mColour);
	mLightShape.setOutlineThickness(0.f);
	//mLightShape.setOrigin();

	rendertex.create(mRadius * 2,mRadius * 2,false);
	rendertex.clear(sf::Color(0,0,0,0));
	rendertex.draw(mLightShape);
	mLightSprite.setTexture(rendertex.getTexture());
	mLightSprite.setOrigin(mRadius, mRadius);
}


