#include "Text.h"
#include "Resource.h"
#include <SFML/System/String.hpp>

Font::Font(const char *path)
{
	mFont = *Resource::RequestFont(path);
};

Font::Font(sf::Font font)
{
	mFont = font;
};

Font::~Font()
{

}

Font Font::GetDefaultFont()
{
	return Font(sf::Font::getDefaultFont());
}



Text::Text(sf::RenderWindow *rend) : BaseDrawable(rend)
{
	mText = sf::Text("test");
	// mText.setFont(Font::GetDefaultFont().mFont);
	mText.setCharacterSize(30);
}

/**
 * Set the font position on screen, convert Engine position to SF.
 */
void Text::SetPosition(const Vector2 position)
{
	Vector2 pos = ig::GameToSFML(position);
	mText.setPosition(pos.SF());
}

void Text::Draw()
{
	mRenderWindow->draw(mText);
}
