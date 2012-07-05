#include "LightInfo.h"


LightInfo::LightInfo(void)
{
	mColour = sf::Color(200,200,200);
	mRadius = 512.f;
	mSpreadAngle = 90.f; //Default to full circle;
}

LightInfo::~LightInfo(void)
{
}

void LightInfo::UpdateLightSprite()
{
	std::cout << "Rendering light sprite texture\n";
	sf::Shape* mLightShape;
	if (ig::Abs(mSpreadAngle - 360.f) < 0.5f) //We are omnidirectional
	{
		sf::CircleShape* shape = new sf::CircleShape();
		shape->setRadius(mRadius);
		mLightShape = shape;
	}
	else //We are cone shaped!
	{
		sf::ConvexShape* shape = new sf::ConvexShape();
		int point_count = std::ceil(ig::DegToRad(mSpreadAngle) * mRadius * 0.01f);
		shape->setPointCount(point_count + 2);
		shape->setPoint(0, Vector2(mRadius, mRadius).SF());
		for (int i = 1; i < point_count + 2; i++)
		{
			float circle_progress = ((float)(i - 1) / (float)point_count) * ig::DegToRad(mSpreadAngle);
			circle_progress += ig::DegToRad(180 - mSpreadAngle) * 0.5f;
			Vector2 pos = Vector2(std::cos(circle_progress) * mRadius, std::sin(circle_progress) * -mRadius);
			pos = pos + Vector2(mRadius, mRadius);
			shape->setPoint(i, pos.SF());
		}
		mLightShape = shape;
	}
	mLightShape->setFillColor(mColour);
	mLightShape->setOutlineThickness(0.f);
	rendertex.create(mRadius * 2,mRadius * 2,false);
	rendertex.clear(sf::Color(0,0,0,0));
	rendertex.draw(*mLightShape);
	rendertex.display();
	mLightSprite.setTexture(rendertex.getTexture());
	mLightSprite.setOrigin(mRadius, mRadius);
}


