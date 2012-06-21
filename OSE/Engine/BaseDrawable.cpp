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
	if (mRenderWindow == NULL)
	{
		return sf::Vector2f(0,0);
	}
	sf::Vector2<double> dPos;
	dPos.x = Pos.x;
	dPos.y = Pos.y;
	dPos.y *= -1;
	sf::Vector2<double> ScreenCentre = sf::Vector2<double>(gGlobals.GameWidth / 2, gGlobals.GameHeight / 2);

	dPos = dPos + ScreenCentre;

	Pos.x = dPos.x;
	Pos.y = dPos.y;

	return Pos.SF();
}

