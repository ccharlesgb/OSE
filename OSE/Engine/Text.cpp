#include "Text.h"
#include "Resource.h"
#include <SFML/System/String.hpp>

Font::Font(const char *path)
{
	mFont = *Resource::RequestFont(path);
};

Font::~Font()
{
	// TODO: We might have to destroy mFont in case we created it.
	// Not sure of a good way to do this, we don't want to destroy it if we loaded it.
}

Text::Text(sf::RenderWindow *rend) : BaseDrawable(rend)
{
	mText = sf::Text();
	mText.setCharacterSize(30);
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
