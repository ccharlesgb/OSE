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
		shape->setPointCount(point_count);
		shape->setPoint(0, Vector2().SF());
		for (int i = 1; i < point_count; i++)
		{
			float circle_progress = i * ig::DegToRad(mSpreadAngle);
			Vector2 pos = Vector2(std::cos(circle_progress) * mRadius, std::sin(circle_progress) * mRadius);

			std::cout << "ADDING POINT: " << pos.ToString() << "\n";
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


