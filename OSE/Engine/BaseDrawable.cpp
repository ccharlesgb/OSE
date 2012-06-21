#include "BaseDrawable.h"
#include "GameGlobals.h"


BaseDrawable::BaseDrawable(sf::RenderWindow *rend)
{
	mRenderWindow = rend;
	mAngle = 0.f;
	mScale = 1.f;
	mPos = Vector2();
	mOrigin = Vector2();
	mDirtyTransform = true;
}

BaseDrawable::~BaseDrawable(void)
{

}

Vector2 BaseDrawable::GameToSFML(Vector2 Pos)
{
	return ig::GameToSFML(Pos);
}

