#include "Text.h"
#include "Resource.h"
#include <SFML/System/String.hpp>

Text::Text(sf::RenderWindow *rend) : BaseDrawable(rend)
{
	mText = sf::Text();
	mText.setCharacterSize(30);
}


void Text::SetFont(const char *path)
{
	mText.setFont(*FontResource::GetFont(path));
}

void Text::SetPosition(const Vector2 position)
{
	Vector2 pos = ig::GameToSFML(position);
	mText.setPosition(pos.SF());
}


void Text::Draw()
{
	mRenderWindow->draw(mText);
}
