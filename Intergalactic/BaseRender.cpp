#include "BaseRender.h"
#include "Renderer.h"

BaseRender::BaseRender()
{
	mDrawOrder = 0;
	mDoVisChecks = true;
}

void BaseRender::PreDraw(sf::RenderWindow *pRender)
{
	GetDrawable()->SetPosition(RenderPos(pRender));
	GetDrawable()->SetRotation(GetAngle());
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
	sf::Vector2<double> ScreenCentre = sf::Vector2<double>(pRender->GetWidth() / 2, pRender->GetHeight() / 2);

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
	mShape = sf::Shape::Circle(sf::Vector2f(), Radius * RENDER_SCALE, Colour, Outline, OutlineColour);
}

void ShapeRender::Rectangle(Vector2 min, Vector2 max, sf::Color Colour, float Outline, sf::Color OutlineColour)
{
	min = min * RENDER_SCALE;
	max = max * RENDER_SCALE;
	mShape = sf::Shape::Rectangle(min.x, min.y, max.x - min.x, max.y - min.y, Colour, Outline, OutlineColour); 
}

void ShapeRender::Polygon(Vector2* vertices, int vertexcount, sf::Color Colour, float Outline, sf::Color OutlineColour)
{
	for (int i=0; i < vertexcount; i++)
	{
		vertices[i].y *= -1;
		mShape.AddPoint(vertices[i].SF(), Colour, OutlineColour);
	}
	mShape.SetOutlineThickness(Outline);
}

SpriteRender::SpriteRender()
{

}

void SpriteRender::SetTexture(const char* FileName, bool Smooth)
{
	std::string sFileName(FileName);
	if (!mTex.LoadFromFile("images/"+ sFileName))
	{
		std::cout << "Invalid filename: " << sFileName << "\n";
	}
	mSprite.SetTexture(mTex);
	mTex.SetSmooth(Smooth);
	mSprite.SetOrigin(mTex.GetWidth() / 2.f, mTex.GetHeight() / 2.f);
}

TextRender::TextRender()
{

}
