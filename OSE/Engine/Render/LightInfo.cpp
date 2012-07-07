#include "LightInfo.h"


LightInfo::LightInfo(void)
{
	mColour = sf::Color(200,200,200);
	mRadius = 128.f;
	mSpreadAngle = 360.f; //Default to full circle;
}

LightInfo::~LightInfo(void)
{
}

//Call when the SHAPE of the light sprite changes
void LightInfo::UpdateLightSprite()
{
	std::cout << "Rendering light ssprite texture\n";
	mRealTimeTexture.create(mRadius, mRadius, false);
	mRealTimeSprite.setTexture(mRealTimeTexture.getTexture());
	mRealTimeSprite.setScale(2.f,2.f);
	mRealTimeSprite.setOrigin(mRadius / 2, mRadius / 2);

	sf::Shape* mLightShape;
	if (ig::Abs(mSpreadAngle - 360.f) < 0.5f) //We are omnidirectional
	{
		sf::CircleShape* shape = new sf::CircleShape();
		shape->setRadius(mRadius / 2);
		mLightShape = shape;
	}
	else //We are cone shaped!
	{
		sf::ConvexShape* shape = new sf::ConvexShape();
		int point_count = std::ceil(ig::DegToRad(mSpreadAngle) * mRadius * 0.01f);
		shape->setPointCount(point_count + 2);
		shape->setPoint(0, Vector2(mRadius / 2, mRadius /2).SF());
		for (int i = 1; i < point_count + 2; i++)
		{
			float circle_progress = ((float)(i - 1) / (float)point_count) * ig::DegToRad(mSpreadAngle);
			circle_progress += ig::DegToRad(180 - mSpreadAngle) * 0.5f;
			Vector2 pos = Vector2(std::cos(circle_progress) * mRadius * 0.5f, std::sin(circle_progress) * -mRadius * 0.5f);
			pos = pos + Vector2(mRadius / 2, mRadius / 2);
			shape->setPoint(i, pos.SF());
		}
		mLightShape = shape;
	}
	mLightShape->setFillColor(sf::Color::White);
	mLightShape->setOutlineThickness(0.f);
	rendertex.create(mRadius,mRadius,false);
	rendertex.clear(sf::Color::Black);
	rendertex.draw(*mLightShape);
	rendertex.display();
	mLightSprite.setTexture(rendertex.getTexture());
	mLightSprite.setOrigin(mRadius / 2, mRadius / 2);
	mLightSprite.setPosition(mRadius / 2,mRadius / 2);
}


