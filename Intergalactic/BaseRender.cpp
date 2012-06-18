#include "BaseRender.h"
#include "Renderer.h"

BaseRender::BaseRender()
{
	mDrawOrder = 0;
	mDoVisChecks = true;
}

void BaseRender::PreDraw(sf::RenderWindow *pRender)
{
	GetDrawable()->setPosition(RenderPos(pRender));
	GetDrawable()->setRotation(GetAngle());
}

Vector2 BaseRender::ToScreen(Vector2 pos)
{
	return RENDERER->ToScreen(pos);
}

sf::Vector2f BaseRender::GameToSFML(Vector2 Pos, sf::RenderWindow *pRender)
{
	if (pRender == NULL)
	{
		return sf::Vector2f(0,0);
	}
	sf::Vector2<double> dPos;
	dPos.x = Pos.x;
	dPos.y = Pos.y;
	dPos.y *= -1;
	sf::Vector2<double> ScreenCentre = sf::Vector2<double>(pRender->getSize().x / 2, pRender->getSize().y / 2);

	dPos = dPos + ScreenCentre;

	Pos.x = dPos.x;
	Pos.y = dPos.y;

	return Pos.SF();
}

ShapeRender::ShapeRender()
{
}

void ShapeRender::Circle(float Radius, sf::Color Colour, float Outline, sf::Color OutlineColour)
{
	mShape = new sf::CircleShape(Radius * RENDER_SCALE, Radius / 5.f);
	mShape->setFillColor(Colour);
	mShape->setOutlineThickness(Outline);
	mShape->setOutlineColor(OutlineColour);
}

void ShapeRender::Rectangle(Vector2 min, Vector2 max, sf::Color Colour, float Outline, sf::Color OutlineColour)
{
	min = min * RENDER_SCALE;
	max = max * RENDER_SCALE;
	mShape = new sf::RectangleShape(); //BROKEN?
	mShape->setFillColor(Colour);
	mShape->setOutlineThickness(Outline);
	mShape->setOutlineColor(OutlineColour);
}

void ShapeRender::Polygon(Vector2* vertices, int vertexcount, sf::Color Colour, float Outline, sf::Color OutlineColour)
{
	mShape = new sf::ConvexShape(vertexcount);
	sf::ConvexShape* mPol = dynamic_cast<sf::ConvexShape*>(mShape);
	for (int i=0; i < vertexcount; i++)
	{
		vertices[i].y *= -1;
		mPol->setPoint(i, vertices[i].SF());;
	}
	mShape->setOutlineThickness(Outline);
}

SpriteRender::SpriteRender()
{

}

void SpriteRender::SetTexture(const char* FileName, bool Smooth)
{
	std::string sFileName(FileName);
	if (!mTex.loadFromFile("images/"+ sFileName))
	{
		std::cout << "Invalid filename: " << sFileName << "\n";
	}
	mSprite.setTexture(mTex);
	mTex.setSmooth(Smooth);
	mSprite.setOrigin(mTex.getSize().x / 2.f, mTex.getSize().y / 2.f);
}

TextRender::TextRender()
{

}
